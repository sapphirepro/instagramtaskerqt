#include "mainwindow.h"
#include "preferencesdialog.h"

#include <QApplication>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFileDialog>
#include <QMessageBox>
#include <QFile>
#include <QFileInfo>
#include <QTextStream>
#include <QCloseEvent>
#include <QRandomGenerator>
#include <QIcon>
#include <QCoreApplication>
#include <QLocale>
#include <QDir>
#include <QDirIterator>
#include <QLabel>
#include <QAbstractItemModel>
#include <QRegularExpression>
#include <QSet>
#include <QHeaderView>
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QUuid>
#include <QDateTime>
#include <QDialog>
#include <QDialogButtonBox>
#include <QFormLayout>
#include <QMouseEvent>
#include <QPainter>
#include <QPointer>
#include <QStyledItemDelegate>
#include <algorithm>
#include <random>
#include <limits>
#include <QColor>
#include <QCheckBox>

namespace {
const char *kDefaultInstaloaderPath = "/Data/instaloader/instaloader.py";
const char *kDefaultUserAgent = "Mozilla/5.0 (X11; Linux x86_64; rv:147.0) Gecko/20100101 Firefox/147.0";
constexpr int kMaxProfileRating = 5;

int clampProfileRating(int rating)
{
    return std::max(0, std::min(kMaxProfileRating, rating));
}

QString starsForRating(int rating)
{
    const int clamped = clampProfileRating(rating);
    return QString(clamped, QChar(0x2605)) + QString(kMaxProfileRating - clamped, QChar(0x2606));
}

qint64 parseDataSizeToBytes(const QString &sizeText)
{
    const QString raw = sizeText.trimmed();
    if (raw.isEmpty()) return 0;

    static const QRegularExpression rx("^([0-9]+(?:[\\.,][0-9]+)?)\\s*([A-Za-z]+)$");
    const QRegularExpressionMatch m = rx.match(raw);
    if (!m.hasMatch()) return 0;

    bool ok = false;
    double value = QLocale().toDouble(m.captured(1), &ok);
    if (!ok) {
        QString normalized = m.captured(1);
        normalized.replace(',', '.');
        value = normalized.toDouble(&ok);
    }
    if (!ok) return 0;

    const QString unit = m.captured(2).toUpper();
    qint64 factor = 1;
    if (unit == "KB") factor = 1024LL;
    else if (unit == "MB") factor = 1024LL * 1024LL;
    else if (unit == "GB") factor = 1024LL * 1024LL * 1024LL;
    else if (unit != "B") factor = 1;

    const double bytes = value * static_cast<double>(factor);
    if (bytes <= 0.0) return 0;
    if (bytes >= static_cast<double>(std::numeric_limits<qint64>::max())) {
        return std::numeric_limits<qint64>::max();
    }
    return static_cast<qint64>(bytes + 0.5);
}

class DataSizeTableWidgetItem final : public QTableWidgetItem
{
public:
    explicit DataSizeTableWidgetItem(const QString &text = QString())
        : QTableWidgetItem(text)
    {
        setData(Qt::UserRole, parseDataSizeToBytes(text));
    }

    bool operator<(const QTableWidgetItem &other) const override
    {
        bool leftOk = false;
        bool rightOk = false;
        const qint64 left = data(Qt::UserRole).toLongLong(&leftOk);
        const qint64 right = other.data(Qt::UserRole).toLongLong(&rightOk);

        const qint64 leftBytes = leftOk ? left : parseDataSizeToBytes(text());
        const qint64 rightBytes = rightOk ? right : parseDataSizeToBytes(other.text());
        return leftBytes < rightBytes;
    }
};

class CheckStateTableWidgetItem final : public QTableWidgetItem
{
public:
    CheckStateTableWidgetItem()
        : QTableWidgetItem()
    {
    }

    bool operator<(const QTableWidgetItem &other) const override
    {
        const bool leftChecked = (checkState() == Qt::Checked);
        const bool rightChecked = (other.checkState() == Qt::Checked);
        if (leftChecked != rightChecked) {
            return leftChecked < rightChecked;
        }
        return QTableWidgetItem::operator<(other);
    }
};

class RatingTableWidgetItem final : public QTableWidgetItem
{
public:
    explicit RatingTableWidgetItem(int rating = 0)
        : QTableWidgetItem()
    {
        setData(Qt::UserRole, clampProfileRating(rating));
        setData(Qt::DisplayRole, QString());
    }

    bool operator<(const QTableWidgetItem &other) const override
    {
        bool leftOk = false;
        bool rightOk = false;
        const int left = data(Qt::UserRole).toInt(&leftOk);
        const int right = other.data(Qt::UserRole).toInt(&rightOk);
        if (leftOk && rightOk && left != right) {
            return left < right;
        }
        return QTableWidgetItem::operator<(other);
    }
};

class StarRatingDelegate final : public QStyledItemDelegate
{
public:
    StarRatingDelegate(QAbstractItemView *view, int ratingColumn, QObject *parent = nullptr)
        : QStyledItemDelegate(parent)
        , view_(view)
        , ratingColumn_(ratingColumn)
    {
        if (view_ && view_->viewport()) {
            view_->viewport()->installEventFilter(this);
            view_->viewport()->setMouseTracking(true);
        }
    }

    ~StarRatingDelegate() override
    {
        if (view_ && view_->viewport()) {
            view_->viewport()->removeEventFilter(this);
        }
    }

    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override
    {
        QStyleOptionViewItem opt(option);
        initStyleOption(&opt, index);
        opt.text.clear();
        QStyledItemDelegate::paint(painter, opt, index);

        int rating = clampProfileRating(index.data(Qt::UserRole).toInt());
        if (hoveredIndex_ == index && hoveredStars_ >= 0) {
            rating = hoveredStars_;
        }

        painter->save();
        const QColor starColor(232, 166, 34);
        painter->setPen(starColor);
        painter->drawText(option.rect.adjusted(4, 0, -4, 0), Qt::AlignCenter, starsForRating(rating));
        painter->restore();
    }

protected:
    bool eventFilter(QObject *watched, QEvent *event) override
    {
        if (!view_ || watched != view_->viewport()) {
            return QStyledItemDelegate::eventFilter(watched, event);
        }

        if (event->type() == QEvent::Leave) {
            clearHoverState();
            return false;
        }

        if (event->type() == QEvent::MouseMove) {
            auto *mouseEvent = static_cast<QMouseEvent *>(event);
            const QPoint pos = mouseEvent->pos();
            const QModelIndex index = view_->indexAt(pos);
            if (isRatingIndex(index)) {
                updateHoverState(index, ratingFromPosition(view_->visualRect(index), pos.x()));
            } else {
                clearHoverState();
            }
            return false;
        }

        if (event->type() == QEvent::MouseButtonPress) {
            auto *mouseEvent = static_cast<QMouseEvent *>(event);
            if (mouseEvent->button() != Qt::LeftButton) {
                return QStyledItemDelegate::eventFilter(watched, event);
            }
            const QPoint pos = mouseEvent->pos();
            const QModelIndex index = view_->indexAt(pos);
            if (!isRatingIndex(index)) {
                return false;
            }

            const int clicked = ratingFromPosition(view_->visualRect(index), pos.x());
            const int current = clampProfileRating(index.data(Qt::UserRole).toInt());
            const int next = (clicked == current) ? 0 : clicked;
            commitRating(index, next);
            updateHoverState(index, next);
            return false;
        }

        return QStyledItemDelegate::eventFilter(watched, event);
    }

private:
    bool isRatingIndex(const QModelIndex &index) const
    {
        return index.isValid() && index.column() == ratingColumn_;
    }

    int ratingFromPosition(const QRect &rect, int x) const
    {
        if (rect.width() <= 0) {
            return 0;
        }
        const int relative = x - rect.left();
        if (relative < 0) {
            return 0;
        }
        if (relative >= rect.width()) {
            return kMaxProfileRating;
        }
        return clampProfileRating((relative * kMaxProfileRating) / rect.width() + 1);
    }

    void commitRating(const QModelIndex &index, int rating) const
    {
        if (!view_ || !index.isValid()) {
            return;
        }
        QAbstractItemModel *model = const_cast<QAbstractItemModel *>(index.model());
        const int clamped = clampProfileRating(rating);
        model->setData(index, clamped, Qt::UserRole);
        model->setData(index, QString(), Qt::DisplayRole);
    }

    void updateHoverState(const QModelIndex &index, int stars)
    {
        const int clamped = clampProfileRating(stars);
        if (hoveredIndex_ == index && hoveredStars_ == clamped) {
            return;
        }

        if (hoveredIndex_.isValid()) {
            const QRect oldRect = view_->visualRect(hoveredIndex_);
            view_->viewport()->update(oldRect);
        }
        hoveredIndex_ = QPersistentModelIndex(index);
        hoveredStars_ = clamped;
        if (hoveredIndex_.isValid()) {
            const QRect newRect = view_->visualRect(hoveredIndex_);
            view_->viewport()->update(newRect);
        }
    }

    void clearHoverState()
    {
        if (!hoveredIndex_.isValid()) {
            return;
        }
        const QRect oldRect = view_->visualRect(hoveredIndex_);
        hoveredIndex_ = QPersistentModelIndex();
        hoveredStars_ = -1;
        view_->viewport()->update(oldRect);
    }

    QPointer<QAbstractItemView> view_;
    int ratingColumn_ = -1;
    QPersistentModelIndex hoveredIndex_;
    int hoveredStars_ = -1;
};
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    // Settings for persisting data
    settings = new QSettings("Sapphire", "InstagramTasker", this);

    // Restore window geometry
    restoreGeometry(settings->value("geometry").toByteArray());

    // Language (stored in QSettings as "en"/"de"/... or "system")
    translator.setParent(qApp);

    if (!settings->contains("uiLanguage")) {
        const QString detected = detectSystemLanguage();
        languageCode = detected;
        settings->setValue("uiLanguage", languageCode);
    } else {
        languageCode = settings->value("uiLanguage").toString();
        if (languageCode.isEmpty()) languageCode = "en";
    }

    const QString effectiveLang = resolveEffectiveLanguage(languageCode);
    installLanguageInternal(effectiveLang);

    setWindowTitle(tr("InstagramTasker GUI"));
    resize(1200, 650);

    // Menu bar
    fileMenu = menuBar()->addMenu(tr("File"));
    settingsMenu = menuBar()->addMenu(tr("Settings"));
    serviceMenu = menuBar()->addMenu(tr("Service"));
    helpMenu = menuBar()->addMenu(tr("Help"));

    actLoadProfiles = new QAction(this);
    actLoadProfilesDb = new QAction(this);
    actNewProfiles = new QAction(this);
    actNewProfilesDb = new QAction(this);
    actSaveProfilesDb = new QAction(this);
    actExit = new QAction(this);
    actPreferences = new QAction(this);
    actCountVideoSize = new QAction(this);
    actCountVideoSize->setCheckable(true);
    actCountVideoSize->setChecked(settings->value("service/countVideoSize", false).toBool());
    actRecalculateVideoSize = new QAction(this);
    actAddRated5 = new QAction(this);
    actAddRated4 = new QAction(this);
    actAddRated3 = new QAction(this);
    actAddRated2 = new QAction(this);
    actAddRated1 = new QAction(this);
    actAddAllRated = new QAction(this);
    actAddOldestProfiles = new QAction(this);
    actAbout = new QAction(this);
    serviceRatingMenu = new QMenu(this);

    fileMenu->addAction(actLoadProfiles);
    fileMenu->addAction(actLoadProfilesDb);
    fileMenu->addAction(actNewProfiles);
    fileMenu->addAction(actNewProfilesDb);
    fileMenu->addAction(actSaveProfilesDb);
    fileMenu->addSeparator();
    fileMenu->addAction(actExit);

    settingsMenu->addAction(actPreferences);
    serviceMenu->addMenu(serviceRatingMenu);
    serviceRatingMenu->addAction(actAddRated5);
    serviceRatingMenu->addAction(actAddRated4);
    serviceRatingMenu->addAction(actAddRated3);
    serviceRatingMenu->addAction(actAddRated2);
    serviceRatingMenu->addAction(actAddRated1);
    serviceRatingMenu->addSeparator();
    serviceRatingMenu->addAction(actAddAllRated);
    serviceMenu->addAction(actAddOldestProfiles);
    serviceMenu->addSeparator();
    serviceMenu->addAction(actCountVideoSize);
    serviceMenu->addAction(actRecalculateVideoSize);
    helpMenu->addAction(actAbout);

    connect(actLoadProfiles, &QAction::triggered, this, [this]() { loadProfiles(); });
    connect(actLoadProfilesDb, &QAction::triggered, this, [this]() { loadProfilesFromDatabase(); });
    connect(actNewProfiles, &QAction::triggered, this, &MainWindow::newProfilesList);
    connect(actNewProfilesDb, &QAction::triggered, this, &MainWindow::newProfilesDatabase);
    connect(actSaveProfilesDb, &QAction::triggered, this, &MainWindow::saveProfilesDatabaseAs);
    connect(actExit, &QAction::triggered, qApp, &QApplication::quit);
    connect(actPreferences, &QAction::triggered, this, &MainWindow::openPreferences);
    connect(actCountVideoSize, &QAction::toggled, this, &MainWindow::onCountVideoSizeToggled);
    connect(actRecalculateVideoSize, &QAction::triggered, this, &MainWindow::recalculateVideoSizes);
    connect(actAddRated5, &QAction::triggered, this, [this]() { addProfilesByRatingRange(5, 5); });
    connect(actAddRated4, &QAction::triggered, this, [this]() { addProfilesByRatingRange(4, 4); });
    connect(actAddRated3, &QAction::triggered, this, [this]() { addProfilesByRatingRange(3, 3); });
    connect(actAddRated2, &QAction::triggered, this, [this]() { addProfilesByRatingRange(2, 2); });
    connect(actAddRated1, &QAction::triggered, this, [this]() { addProfilesByRatingRange(1, 1); });
    connect(actAddAllRated, &QAction::triggered, this, [this]() { addProfilesByRatingRange(1, kMaxProfileRating); });
    connect(actAddOldestProfiles, &QAction::triggered, this, &MainWindow::addOldestProfilesToQueue);
    connect(actAbout, &QAction::triggered, this, &MainWindow::showAboutDialog);

    preferencesDialog = nullptr;

    // Load saved working dir or default to app dir
    workingDir = settings->value("workingDir", QCoreApplication::applicationDirPath()).toString();

    // UI components
    sourceTable = new QTableWidget(this);
    sourceTable->setColumnCount(ColCount);
    sourceTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    sourceTable->setSelectionMode(QAbstractItemView::SingleSelection);
    sourceTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    sourceTable->setSortingEnabled(true);
    sourceTable->verticalHeader()->setVisible(false);
    sourceTable->horizontalHeader()->setSectionsMovable(false);
    sourceTable->horizontalHeader()->setStretchLastSection(false);
    sourceTable->horizontalHeader()->setSectionResizeMode(ColEnabled, QHeaderView::ResizeToContents);
    sourceTable->horizontalHeader()->setSectionResizeMode(ColProfile, QHeaderView::Interactive);
    sourceTable->horizontalHeader()->setSectionResizeMode(ColRating, QHeaderView::ResizeToContents);
    sourceTable->horizontalHeader()->setSectionResizeMode(ColVideos, QHeaderView::ResizeToContents);
    sourceTable->horizontalHeader()->setSectionResizeMode(ColStories, QHeaderView::ResizeToContents);
    sourceTable->horizontalHeader()->setSectionResizeMode(ColHighlights, QHeaderView::ResizeToContents);
    sourceTable->horizontalHeader()->setSectionResizeMode(ColTagged, QHeaderView::ResizeToContents);
    sourceTable->horizontalHeader()->setSectionResizeMode(ColReels, QHeaderView::ResizeToContents);
    sourceTable->horizontalHeader()->setSectionResizeMode(ColLastUpdated, QHeaderView::ResizeToContents);
    sourceTable->horizontalHeader()->setSectionResizeMode(ColVideosSize, QHeaderView::ResizeToContents);
    sourceTable->setColumnWidth(ColProfile, settings->value("ui/sourceProfileColumnWidth", 280).toInt());
    sourceTable->setColumnWidth(ColRating, 120);
    sourceTable->setItemDelegateForColumn(ColRating, new StarRatingDelegate(sourceTable, ColRating, sourceTable));
    sourceTable->horizontalHeader()->setContextMenuPolicy(Qt::CustomContextMenu);
    sourceTable->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(sourceTable, &QTableWidget::customContextMenuRequested, this, &MainWindow::showSourceContextMenu);
    connect(sourceTable->horizontalHeader(), &QHeaderView::customContextMenuRequested,
            this, &MainWindow::showSourceHeaderContextMenu);
    connect(sourceTable, &QTableWidget::itemChanged, this, &MainWindow::onSourceTableItemChanged);

    queueList = new QListWidget(this);
    queueList->setSelectionMode(QAbstractItemView::NoSelection);

    logTextEdit = new QTextEdit(this);
    logTextEdit->setReadOnly(true);
    // Custom context menu (Clear Log)
    logTextEdit->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(logTextEdit, &QTextEdit::customContextMenuRequested, this, &MainWindow::showLogContextMenu);

    newProfilesLog = new QTextEdit(this);
    newProfilesLog->setReadOnly(true);
    // Custom context menu (Clear New Profiles)
    newProfilesLog->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(newProfilesLog, &QTextEdit::customContextMenuRequested, this, &MainWindow::showNewProfilesContextMenu);
    // Restore persisted list of newly discovered profiles
    newProfilesLog->setPlainText(settings->value("newProfilesLogText").toString());

    progressBar = new QProgressBar(this);
    progressBar->setRange(0, 100);
    progressBar->setValue(0);

    profileProgressBar = new QProgressBar(this);  // New per-profile progress
    profileProgressBar->setRange(0, 100);
    profileProgressBar->setValue(0);

    removeMissingButton = new QPushButton(this);
    startButton = new QPushButton(this);
    pauseButton = new QPushButton(this);
    cancelButton = new QPushButton(this);
    addButton = new QPushButton(this);
    addToQueueButton = new QPushButton(">>", this);  // Add to queue
    clearQueueButton = new QPushButton(this);

    addProfilesEdit = new QTextEdit(this);
    addProfilesEdit->setPlaceholderText(tr("Paste profiles here (one per line or space-separated)"));

    sourceCountLabel = new QLabel(this);
    sourceSearchLabel = new QLabel(this);
    sourceSearchEdit = new QLineEdit(this);
    sourceSearchEdit->setClearButtonEnabled(true);
    sourceSearchEdit->setMaximumWidth(260);
    connect(sourceSearchEdit, &QLineEdit::textChanged, this, &MainWindow::onSourceSearchTextChanged);

    queueCountLabel = new QLabel(this);

    logLabel = new QLabel(this);
    newProfilesLabel = new QLabel(this);

    addNewProfilesLabel = new QLabel(this);

    allowNewProfilesCheck = new QCheckBox(this);
    allowNewProfilesCheck->setChecked(settings->value("allowNewProfiles", true).toBool());
    connect(allowNewProfilesCheck, &QCheckBox::toggled, this, [this](bool on){
        settings->setValue("allowNewProfiles", on);
    });

    // Profiles layout with splitters:
    // left source profiles | splitter | (>> + active/new profiles area)
    QHBoxLayout *profilesLayout = new QHBoxLayout;
    profilesLayout->setContentsMargins(0, 0, 0, 0);
    profilesLayout->setSpacing(6);

    // Left side: counter + list
    QVBoxLayout *leftLayout = new QVBoxLayout;
    QHBoxLayout *sourceHeaderLayout = new QHBoxLayout;
    sourceHeaderLayout->addWidget(sourceCountLabel);
    sourceHeaderLayout->addStretch();
    sourceHeaderLayout->addWidget(sourceSearchLabel);
    sourceHeaderLayout->addWidget(sourceSearchEdit);
    leftLayout->addLayout(sourceHeaderLayout);
    leftLayout->addWidget(sourceTable);
    QWidget *leftPanel = new QWidget(this);
    leftPanel->setLayout(leftLayout);

    // Middle button column
    QVBoxLayout *middleLayout = new QVBoxLayout;
    middleLayout->addStretch();
    middleLayout->addWidget(addToQueueButton);
    middleLayout->addStretch();
    QWidget *middlePanel = new QWidget(this);
    middlePanel->setLayout(middleLayout);
    middlePanel->setFixedWidth(44);

    // Right side: Active / Add new profiles (50/50 by default)
    QVBoxLayout *rightLayout = new QVBoxLayout;
    rightLayout->addWidget(queueCountLabel);
    rightLayout->addWidget(queueList);
    QWidget *rightPanel = new QWidget(this);
    rightPanel->setLayout(rightLayout);
    rightPanel->setMinimumWidth(220);

    QVBoxLayout *addLayout = new QVBoxLayout;
    addLayout->addWidget(addNewProfilesLabel);
    addLayout->addWidget(addProfilesEdit);
    addLayout->addWidget(allowNewProfilesCheck);
    addLayout->addWidget(addButton);
    QWidget *addPanel = new QWidget(this);
    addPanel->setLayout(addLayout);
    addPanel->setMinimumWidth(220);

    queueAddSplitter = new QSplitter(Qt::Horizontal, this);
    queueAddSplitter->setChildrenCollapsible(false);
    queueAddSplitter->addWidget(rightPanel);
    queueAddSplitter->addWidget(addPanel);
    queueAddSplitter->setStretchFactor(0, 1);
    queueAddSplitter->setStretchFactor(1, 1);

    const QByteArray queueAddState = settings->value("ui/queueAddSplitterState").toByteArray();
    if (!queueAddState.isEmpty()) {
        queueAddSplitter->restoreState(queueAddState);
    } else {
        queueAddSplitter->setSizes({500, 500});
    }

    QWidget *rightComposite = new QWidget(this);
    QHBoxLayout *rightCompositeLayout = new QHBoxLayout(rightComposite);
    rightCompositeLayout->setContentsMargins(0, 0, 0, 0);
    rightCompositeLayout->setSpacing(6);
    rightCompositeLayout->addWidget(middlePanel, 0);
    rightCompositeLayout->addWidget(queueAddSplitter, 1);

    profilesAreaSplitter = new QSplitter(Qt::Horizontal, this);
    profilesAreaSplitter->setChildrenCollapsible(false);
    profilesAreaSplitter->addWidget(leftPanel);
    profilesAreaSplitter->addWidget(rightComposite);
    profilesAreaSplitter->setStretchFactor(0, 2);
    profilesAreaSplitter->setStretchFactor(1, 1);

    const QByteArray profilesState = settings->value("ui/profilesAreaSplitterState").toByteArray();
    if (!profilesState.isEmpty()) {
        profilesAreaSplitter->restoreState(profilesState);
    } else {
        profilesAreaSplitter->setSizes({760, 460});
    }

    profilesLayout->addWidget(profilesAreaSplitter, 1);

    QHBoxLayout *buttonLayout = new QHBoxLayout;
    buttonLayout->addWidget(removeMissingButton);
    buttonLayout->addWidget(startButton);
    buttonLayout->addWidget(pauseButton);
    buttonLayout->addWidget(cancelButton);
    buttonLayout->addWidget(clearQueueButton);

    QWidget *topPane = new QWidget(this);
    QVBoxLayout *topLayout = new QVBoxLayout(topPane);
    topLayout->setContentsMargins(0, 0, 0, 0);
    topLayout->setSpacing(6);
    topLayout->addLayout(profilesLayout);
    topLayout->addLayout(buttonLayout);
    topLayout->addWidget(progressBar);
    topLayout->addWidget(profileProgressBar);  // Add second progress bar

    // Horizontal layout for logs: main log (75%) and new profiles log (25%)
    QHBoxLayout *logsLayout = new QHBoxLayout;

    QVBoxLayout *logCol = new QVBoxLayout;
    logCol->addWidget(logLabel);
    logCol->addWidget(logTextEdit);
    logsLayout->addLayout(logCol, 3);

    QVBoxLayout *newCol = new QVBoxLayout;
    newCol->addWidget(newProfilesLabel);
    newCol->addWidget(newProfilesLog);
    logsLayout->addLayout(newCol, 1);

    QWidget *bottomPane = new QWidget(this);
    bottomPane->setLayout(logsLayout);

    mainSplitter = new QSplitter(Qt::Vertical, this);
    mainSplitter->setChildrenCollapsible(false);
    mainSplitter->addWidget(topPane);
    mainSplitter->addWidget(bottomPane);
    mainSplitter->setStretchFactor(0, 3);
    mainSplitter->setStretchFactor(1, 2);

    const QByteArray splitterState = settings->value("ui/mainSplitterState").toByteArray();
    if (!splitterState.isEmpty()) {
        mainSplitter->restoreState(splitterState);
    } else {
        mainSplitter->setSizes({420, 230});
    }

    QWidget *centralWidget = new QWidget(this);
    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);
    mainLayout->setContentsMargins(6, 6, 6, 6);
    mainLayout->setSpacing(6);
    mainLayout->addWidget(mainSplitter);
    centralWidget->setLayout(mainLayout);
    setCentralWidget(centralWidget);

    // Keep counters in sync even if list changes are triggered from different code paths
    connect(sourceTable->model(), &QAbstractItemModel::rowsInserted, this, &MainWindow::updateSourceCounter);
    connect(sourceTable->model(), &QAbstractItemModel::rowsRemoved,  this, &MainWindow::updateSourceCounter);
    connect(sourceTable->model(), &QAbstractItemModel::modelReset,   this, &MainWindow::updateSourceCounter);

    connect(queueList->model(), &QAbstractItemModel::rowsInserted, this, &MainWindow::updateQueueCounter);
    connect(queueList->model(), &QAbstractItemModel::rowsRemoved,  this, &MainWindow::updateQueueCounter);
    connect(queueList->model(), &QAbstractItemModel::modelReset,   this, &MainWindow::updateQueueCounter);

// Tray icon
    trayIcon = new QSystemTrayIcon(this);
    trayIcon->setIcon(QIcon(":/icon.png"));
    trayIcon->setToolTip("InstagramTasker");

    QMenu *trayMenu = new QMenu(this);
    trayShowAction = new QAction(this);
    trayQuitAction = new QAction(this);
    trayMenu->addAction(trayShowAction);
    trayMenu->addAction(trayQuitAction);

    trayIcon->setContextMenu(trayMenu);
    connect(trayShowAction, &QAction::triggered, this, &MainWindow::showNormal);
    connect(trayQuitAction, &QAction::triggered, qApp, &QApplication::quit);
    connect(trayIcon, &QSystemTrayIcon::activated, this, &MainWindow::trayActivated);

    trayIcon->show();

    // Apply UI texts for current language (and counters)
    // (must be after tray actions are created; also safe due to nullptr guards)
    updateUiTexts();

    // Connections
    connect(startButton, &QPushButton::clicked, this, &MainWindow::startProcessing);
    connect(pauseButton, &QPushButton::clicked, this, &MainWindow::togglePause);
    connect(cancelButton, &QPushButton::clicked, this, &MainWindow::cancelProcessing);
    connect(addButton, &QPushButton::clicked, this, &MainWindow::addNewProfiles);
    connect(addToQueueButton, &QPushButton::clicked, this, &MainWindow::addToQueue);
    connect(clearQueueButton, &QPushButton::clicked, this, &MainWindow::clearQueue);
    connect(removeMissingButton, &QPushButton::clicked, this, &MainWindow::removeMissingProfiles);

    // Process
    process = new QProcess(this);
    connect(process, &QProcess::readyReadStandardOutput, this, &MainWindow::readProcessOutput);
    connect(process, &QProcess::readyReadStandardError, this, &MainWindow::readProcessError);
    connect(process, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished), this, &MainWindow::processFinished);

    // Timer for sleep
    sleepTimer = new QTimer(this);
    sleepTimer->setSingleShot(true);
    connect(sleepTimer, &QTimer::timeout, this, &MainWindow::processNext);

    // Initial states
    paused = false;
    canceled = false;
    isProcessing = false;  // New flag for processing state
    processed = 0;
    success = 0;
    failed = 0;
    skipped = 0;
    usingResume = false;
    checkpointError = false;  // New flag for checkpoint errors
    profilesFilePath = settings->value("lastProfilesFile").toString();
    profilesDbPath = settings->value("lastProfilesDb").toString();
    lastSourceKind = settings->value("lastSourceKind", "file").toString();
    sourceBackedByDatabase = false;

    // Check for resume.txt
    checkResumeFile();

    // Auto-load last source (file or DB)
    if (lastSourceKind == "db") {
        if (!profilesDbPath.isEmpty() && QFile::exists(profilesDbPath)) {
            loadProfilesFromDatabase(profilesDbPath);
        } else if (!profilesFilePath.isEmpty() && QFile::exists(profilesFilePath)) {
            loadProfiles(profilesFilePath);
        }
    } else {
        if (!profilesFilePath.isEmpty() && QFile::exists(profilesFilePath)) {
            loadProfiles(profilesFilePath);
        } else if (!profilesDbPath.isEmpty() && QFile::exists(profilesDbPath)) {
            loadProfilesFromDatabase(profilesDbPath);
        }
    }

    updateSourceCounter();
}

MainWindow::~MainWindow()
{
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    // Persist UI state on any close request (even when we just hide to tray)
    settings->setValue("geometry", saveGeometry());
    if (sourceTable) {
        settings->setValue("ui/sourceProfileColumnWidth", sourceTable->columnWidth(ColProfile));
    }
    if (profilesAreaSplitter) {
        settings->setValue("ui/profilesAreaSplitterState", profilesAreaSplitter->saveState());
    }
    if (queueAddSplitter) {
        settings->setValue("ui/queueAddSplitterState", queueAddSplitter->saveState());
    }
    if (mainSplitter) {
        settings->setValue("ui/mainSplitterState", mainSplitter->saveState());
    }
    settings->setValue("newProfilesLogText", newProfilesLog ? newProfilesLog->toPlainText() : QString());
    saveProfilesDatabase();

    if (!isHidden()) {
        hide();
        event->ignore();
    } else {
        event->accept();
    }
}

void MainWindow::changeEvent(QEvent *event)
{
    if (event && event->type() == QEvent::LanguageChange) {
        updateUiTexts();
    }
    QMainWindow::changeEvent(event);
}


void MainWindow::trayActivated(QSystemTrayIcon::ActivationReason reason)
{
    if (reason == QSystemTrayIcon::Trigger) {
        if (isHidden()) {
            showNormal();
        } else {
            hide();
        }
    }
}

void MainWindow::loadProfiles()
{
    QString fileName = QFileDialog::getOpenFileName(this, "Open Profiles File", "", "Text Files (*.txt)");
    if (fileName.isEmpty()) return;

    loadProfiles(fileName);
}

void MainWindow::loadProfilesFromDatabase()
{
    const QString startDir = profilesDbPath.isEmpty()
            ? QDir::homePath()
            : QFileInfo(profilesDbPath).absolutePath();
    const QString fileName = QFileDialog::getOpenFileName(
            this,
            tr("Load Profiles from DB"),
            startDir,
            "SQLite Files (*.db *.sqlite *.sqlite3);;All Files (*)");
    if (fileName.isEmpty()) return;

    loadProfilesFromDatabase(fileName);
}

void MainWindow::saveProfilesDatabaseAs()
{
    const QString startDir = profilesDbPath.isEmpty()
            ? (profilesFilePath.isEmpty() ? QDir::homePath() : QFileInfo(profilesFilePath).absolutePath())
            : QFileInfo(profilesDbPath).absolutePath();
    QString fileName = QFileDialog::getSaveFileName(
            this,
            tr("Save as Profiles DB"),
            startDir,
            "SQLite Files (*.db *.sqlite *.sqlite3);;All Files (*)");
    if (fileName.isEmpty()) return;

    if (QFileInfo(fileName).suffix().isEmpty()) {
        fileName += ".sqlite";
    }

    if (!ensureProfilesDatabaseSchema(fileName)) {
        return;
    }

    profilesDbPath = fileName;
    settings->setValue("lastProfilesDb", profilesDbPath);
    sourceBackedByDatabase = true;
    lastSourceKind = "db";
    settings->setValue("lastSourceKind", lastSourceKind);

    if (saveProfilesDatabase()) {
        log(tr("Created new profiles list: %1").arg(profilesDbPath), QColor(Qt::green));
    }
}

void MainWindow::loadProfiles(const QString &fileName)
{
    clearSourceTable();
    sourceBackedByDatabase = false;

    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        log(tr("Error opening file: %1").arg(fileName), QColor(Qt::red));
        return;
    }

    // Load skipped.txt and uncheck skipped profiles
    QString skippedPath = QDir(workingDir).filePath("skipped.txt");
    QSet<QString> skippedSet;
    if (QFile::exists(skippedPath)) {
        QFile skippedFile(skippedPath);
        if (skippedFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QTextStream skippedIn(&skippedFile);
            while (!skippedIn.atEnd()) {
                QString skippedProfile = skippedIn.readLine().trimmed().toLower();
                if (!skippedProfile.isEmpty()) {
                    skippedSet.insert(skippedProfile);
                }
            }
            skippedFile.close();
        }
    }

    const ProfileDefaults defaults = profileDefaultsFromSettings();
    QSet<QString> seenProfiles;

    QTextStream in(&file);
    sourceTableUpdating = true;
    while (!in.atEnd()) {
        QString profile = in.readLine().trimmed();
        if (!profile.isEmpty()) {
            const QString key = profile.toLower();
            if (seenProfiles.contains(key)) {
                continue;
            }
            seenProfiles.insert(key);
            const bool enabled = skippedSet.contains(profile.toLower()) ? false : defaults.enabled;
            addSourceRow(profile,
                         enabled,
                         defaults.videos,
                         defaults.stories,
                         defaults.highlights,
                         defaults.tagged,
                         defaults.reels);
        }
    }
    sourceTableUpdating = false;
    file.close();

    // Sort the list alphabetically in ascending order
    sourceTable->sortItems(ColProfile, Qt::AscendingOrder);

    updateSourceCounter();
    removeDisabledProfilesFromQueue();

    log(tr("Loaded %1 profiles from %2").arg(sourceTable->rowCount()).arg(fileName));
    profilesFilePath = fileName;
    settings->setValue("lastProfilesFile", profilesFilePath);
    lastSourceKind = "file";
    settings->setValue("lastSourceKind", lastSourceKind);
}

void MainWindow::updateSourceCounter()
{
    if (!sourceCountLabel) return;
    sourceCountLabel->setText(tr("Profiles: %1").arg(sourceTable ? sourceTable->rowCount() : 0));
}

void MainWindow::updateQueueCounter()
{
    if (!queueCountLabel) return;
    queueCountLabel->setText(tr("Active: %1").arg(queueList->count()));
}


void MainWindow::updateUiTexts()
{
    // Menus
    if (fileMenu) fileMenu->setTitle(tr("File"));
    if (settingsMenu) settingsMenu->setTitle(tr("Settings"));
    if (serviceMenu) serviceMenu->setTitle(tr("Service"));
    if (helpMenu) helpMenu->setTitle(tr("Help"));

    if (actLoadProfiles) actLoadProfiles->setText(tr("Load Profiles..."));
    if (actLoadProfilesDb) actLoadProfilesDb->setText(tr("Load Profiles from DB..."));
    if (actNewProfiles) actNewProfiles->setText(tr("New Profiles List..."));
    if (actNewProfilesDb) actNewProfilesDb->setText(tr("New Profiles DB..."));
    if (actSaveProfilesDb) actSaveProfilesDb->setText(tr("Save as Profiles DB..."));
    if (actExit) actExit->setText(tr("Exit"));
    if (actPreferences) actPreferences->setText(tr("Preferences..."));
    if (actCountVideoSize) actCountVideoSize->setText(tr("Count video size"));
    if (actRecalculateVideoSize) actRecalculateVideoSize->setText(tr("Recalculate video size"));
    if (serviceRatingMenu) serviceRatingMenu->setTitle(tr("Add by rating"));
    if (actAddRated5) actAddRated5->setText(tr("Add 5 stars"));
    if (actAddRated4) actAddRated4->setText(tr("Add 4 stars"));
    if (actAddRated3) actAddRated3->setText(tr("Add 3 stars"));
    if (actAddRated2) actAddRated2->setText(tr("Add 2 stars"));
    if (actAddRated1) actAddRated1->setText(tr("Add 1 star"));
    if (actAddAllRated) actAddAllRated->setText(tr("Add all rated"));
    if (actAddOldestProfiles) actAddOldestProfiles->setText(tr("Add oldest profiles"));
    if (actAbout) actAbout->setText(tr("About..."));

    // Window title
    setWindowTitle(tr("InstagramTasker GUI"));

    // Buttons / labels
    if (removeMissingButton) removeMissingButton->setText(tr("Remove Missing"));
    if (startButton) startButton->setText(tr("Start"));
    if (pauseButton) pauseButton->setText(tr("Pause"));
    if (cancelButton) cancelButton->setText(tr("Cancel"));
    if (clearQueueButton) clearQueueButton->setText(tr("Clear Current Tasks"));
    if (addButton) addButton->setText(tr("Add Profiles"));
    if (allowNewProfilesCheck) allowNewProfilesCheck->setText(tr("Allow adding new"));

    if (logLabel) logLabel->setText(tr("Log:"));
    if (newProfilesLabel) newProfilesLabel->setText(tr("New profiles:"));
    if (addNewProfilesLabel) addNewProfilesLabel->setText(tr("Add new profiles:"));
    if (sourceSearchLabel) sourceSearchLabel->setText(tr("Search:"));
    if (sourceSearchEdit) sourceSearchEdit->setPlaceholderText(tr("Filter profiles..."));

    if (addProfilesEdit) addProfilesEdit->setPlaceholderText(tr("Paste profiles here (one per line or space-separated)"));
    if (sourceTable) {
        sourceTable->setHorizontalHeaderLabels({
            tr("Enabled"),
            tr("Profile"),
            tr("Rating"),
            tr("Video"),
            tr("Stories"),
            tr("Highlights"),
            tr("Tagged"),
            tr("Reels"),
            tr("Last updated"),
            tr("Videos size")
        });
    }

    // Counters
    updateSourceCounter();
    updateQueueCounter();

    // Tray actions
    if (trayShowAction) trayShowAction->setText(tr("Show"));
    if (trayQuitAction) trayQuitAction->setText(tr("Quit"));
}

void MainWindow::applyLanguage(const QString &langCode)
{
    if (langCode.isEmpty()) return;
    languageCode = langCode;
    settings->setValue("uiLanguage", languageCode);

    const QString effectiveLang = resolveEffectiveLanguage(languageCode);
    installLanguageInternal(effectiveLang);

    updateUiTexts();
}


void MainWindow::openPreferences()
{
    if (!preferencesDialog) {
        preferencesDialog = new PreferencesDialog(this);

        connect(preferencesDialog, &PreferencesDialog::workingDirectoryBrowseRequested, this, [this]() {
            QString dir = QFileDialog::getExistingDirectory(this, tr("Preferences..."), workingDir);
            if (!dir.isEmpty()) {
                preferencesDialog->setWorkingDirectory(dir);
            }
        });

        connect(preferencesDialog, &PreferencesDialog::instaloaderBrowseRequested, this, [this]() {
            const QString currentScript = instaloaderScriptPathFromSettings();
            const QString startPath = currentScript.isEmpty()
                    ? workingDir
                    : QFileInfo(currentScript).absolutePath();
            const QString filePath = QFileDialog::getOpenFileName(
                    this,
                    tr("Select Instaloader script"),
                    startPath,
                    "Python Files (*.py);;All Files (*)");
            if (!filePath.isEmpty()) {
                preferencesDialog->setInstaloaderPath(filePath);
            }
        });

        connect(preferencesDialog, &PreferencesDialog::preferencesAccepted, this,
                [this](const QString &dir,
                       const QString &instaloaderPath,
                       const QString &lang,
                       const QString &login,
                       bool noMetadata,
                       bool noCaptions,
                       const QString &userAgent,
                       bool noIphone,
                       bool abort400,
                       bool abort401,
                       bool abort403,
                       bool abort404,
                       bool abort429,
                       bool defEnabled,
                       bool defVideos,
                       bool defStories,
                       bool defHighlights,
                       bool defTagged,
                       bool defReels,
                       int batchQueueLimit) {
                    if (!dir.isEmpty() && dir != workingDir) {
                        workingDir = dir;
                        settings->setValue("workingDir", workingDir);
                        log(tr("Working directory set to: %1").arg(workingDir), QColor(Qt::green));
                    }
                    if (!lang.isEmpty() && lang != languageCode) {
                        applyLanguage(lang);
                    }

                    settings->setValue("instaloader/scriptPath", instaloaderPath.trimmed());
                    settings->setValue("instaloader/login", login.trimmed());
                    settings->setValue("instaloader/noMetadata", noMetadata);
                    settings->setValue("instaloader/noCaptions", noCaptions);
                    settings->setValue("instaloader/userAgent", userAgent.trimmed());
                    settings->setValue("instaloader/noIphone", noIphone);
                    settings->setValue("instaloader/abort400", abort400);
                    settings->setValue("instaloader/abort401", abort401);
                    settings->setValue("instaloader/abort403", abort403);
                    settings->setValue("instaloader/abort404", abort404);
                    settings->setValue("instaloader/abort429", abort429);
                    settings->setValue("service/batchQueueLimit", std::max(1, batchQueueLimit));

                    ProfileDefaults defaults;
                    defaults.enabled = defEnabled;
                    defaults.videos = defVideos;
                    defaults.stories = defStories;
                    defaults.highlights = defHighlights;
                    defaults.tagged = defTagged;
                    defaults.reels = defReels;
                    applyProfileDefaultsToSettings(defaults);
                    settings->sync();
                });
    }

    preferencesDialog->setWorkingDirectory(workingDir);
    preferencesDialog->setInstaloaderPath(instaloaderScriptPathFromSettings());
    preferencesDialog->setLanguageCode(languageCode);
    preferencesDialog->setDownloaderOptions(
            settings->value("instaloader/login", QString()).toString(),
            settings->value("instaloader/noMetadata", true).toBool(),
            settings->value("instaloader/noCaptions", true).toBool(),
            settings->value("instaloader/userAgent", QString::fromLatin1(kDefaultUserAgent)).toString(),
            settings->value("instaloader/noIphone", true).toBool(),
            settings->value("instaloader/abort400", true).toBool(),
            settings->value("instaloader/abort401", true).toBool(),
            settings->value("instaloader/abort403", true).toBool(),
            settings->value("instaloader/abort404", true).toBool(),
            settings->value("instaloader/abort429", true).toBool());
    const ProfileDefaults defaults = profileDefaultsFromSettings();
    preferencesDialog->setDefaultBehaviour(defaults.enabled,
                                           defaults.videos,
                                           defaults.stories,
                                           defaults.highlights,
                                           defaults.tagged,
                                           defaults.reels);
    preferencesDialog->setBatchQueueLimit(batchQueueLimitFromSettings());
    preferencesDialog->show();
    preferencesDialog->raise();
    preferencesDialog->activateWindow();
}

void MainWindow::newProfilesList()
{
    QString fileName = QFileDialog::getSaveFileName(this,
                                                    tr("Save profiles file"),
                                                    profilesFilePath.isEmpty() ? QDir::homePath() : QFileInfo(profilesFilePath).absolutePath(),
                                                    "Text Files (*.txt);;All Files (*)");
    if (fileName.isEmpty()) return;

    profilesFilePath = fileName;
    settings->setValue("lastProfilesFile", profilesFilePath);
    sourceBackedByDatabase = false;
    lastSourceKind = "file";
    settings->setValue("lastSourceKind", lastSourceKind);

    // Reset data
    queueProfiles.clear();
    selectedProfiles.clear();
    clearSourceTable();
    queueList->clear();
    updateSourceCounter();
    updateQueueCounter();

    saveProfilesFile();
    saveResumeFile();
    log(tr("Created new profiles list: %1").arg(profilesFilePath), QColor(Qt::green));
}

void MainWindow::newProfilesDatabase()
{
    const QString startDir = profilesDbPath.isEmpty()
            ? QDir::homePath()
            : QFileInfo(profilesDbPath).absolutePath();
    QString fileName = QFileDialog::getSaveFileName(
            this,
            tr("Save as Profiles DB"),
            startDir,
            "SQLite Files (*.db *.sqlite *.sqlite3);;All Files (*)");
    if (fileName.isEmpty()) return;

    if (QFileInfo(fileName).suffix().isEmpty()) {
        fileName += ".sqlite";
    }

    if (!ensureProfilesDatabaseSchema(fileName)) {
        return;
    }

    profilesDbPath = fileName;
    settings->setValue("lastProfilesDb", profilesDbPath);
    profilesFilePath.clear();
    settings->setValue("lastProfilesFile", QString());
    sourceBackedByDatabase = true;
    lastSourceKind = "db";
    settings->setValue("lastSourceKind", lastSourceKind);

    clearSourceTable();
    queueList->clear();
    queueProfiles.clear();
    selectedProfiles.clear();
    updateSourceCounter();
    updateQueueCounter();
    saveResumeFile();
    saveProfilesDatabase();

    log(tr("Created new profiles list: %1").arg(profilesDbPath), QColor(Qt::green));
}

void MainWindow::saveProfilesFile()
{
    if (profilesFilePath.isEmpty()) return;

    QFile file(profilesFilePath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate)) {
        log(tr("Error saving profiles file: %1").arg(profilesFilePath), QColor(Qt::red));
        return;
    }

    QTextStream out(&file);
    for (int row = 0; row < sourceTable->rowCount(); ++row) {
        const QString profile = sourceProfileAtRow(row);
        if (!profile.isEmpty()) {
            out << profile << "\n";
        }
    }
    file.close();
}

bool MainWindow::ensureProfilesDatabaseSchema(const QString &fileName)
{
    const QString connectionName = QString("profiles_schema_%1")
            .arg(QUuid::createUuid().toString(QUuid::WithoutBraces));
    bool ok = false;
    {
        QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE", connectionName);
        db.setDatabaseName(fileName);
        if (!db.open()) {
            log(tr("Database error: %1").arg(db.lastError().text()), QColor(Qt::red));
        } else {
            QSqlQuery q(db);
            ok = q.exec(
                    "CREATE TABLE IF NOT EXISTS profiles ("
                    "id INTEGER PRIMARY KEY AUTOINCREMENT,"
                    "profile_name TEXT NOT NULL UNIQUE COLLATE NOCASE,"
                    "enabled INTEGER NOT NULL DEFAULT 1,"
                    "videos INTEGER NOT NULL DEFAULT 1,"
                    "stories INTEGER NOT NULL DEFAULT 1,"
                    "highlights INTEGER NOT NULL DEFAULT 1,"
                    "tagged INTEGER NOT NULL DEFAULT 1,"
                    "reels INTEGER NOT NULL DEFAULT 1,"
                    "rating INTEGER NOT NULL DEFAULT 0,"
                    "last_updated TEXT NOT NULL DEFAULT '',"
                    "videos_size TEXT NOT NULL DEFAULT ''"
                    ");");
            if (!ok) {
                log(tr("Database error: %1").arg(q.lastError().text()), QColor(Qt::red));
            } else {
                bool hasRatingColumn = false;
                if (!q.exec("PRAGMA table_info(profiles)")) {
                    log(tr("Database error: %1").arg(q.lastError().text()), QColor(Qt::red));
                    ok = false;
                } else {
                    while (q.next()) {
                        if (q.value(1).toString().compare("rating", Qt::CaseInsensitive) == 0) {
                            hasRatingColumn = true;
                            break;
                        }
                    }
                    if (!hasRatingColumn) {
                        if (!q.exec("ALTER TABLE profiles ADD COLUMN rating INTEGER NOT NULL DEFAULT 0")) {
                            log(tr("Database error: %1").arg(q.lastError().text()), QColor(Qt::red));
                            ok = false;
                        }
                    }
                }
            }
        }
    }
    QSqlDatabase::removeDatabase(connectionName);
    return ok;
}

bool MainWindow::loadProfilesFromDatabase(const QString &fileName)
{
    if (!ensureProfilesDatabaseSchema(fileName)) {
        return false;
    }

    const QString connectionName = QString("profiles_load_%1")
            .arg(QUuid::createUuid().toString(QUuid::WithoutBraces));

    bool ok = false;
    int loadedCount = 0;
    {
        QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE", connectionName);
        db.setDatabaseName(fileName);
        if (!db.open()) {
            log(tr("Database error: %1").arg(db.lastError().text()), QColor(Qt::red));
        } else {
            QSqlQuery q(db);
            if (!q.exec(
                        "SELECT profile_name, enabled, videos, stories, highlights, tagged, reels, "
                        "COALESCE(rating, 0), COALESCE(last_updated, ''), COALESCE(videos_size, '') "
                        "FROM profiles ORDER BY profile_name COLLATE NOCASE")) {
                log(tr("Database error: %1").arg(q.lastError().text()), QColor(Qt::red));
            } else {
                clearSourceTable();
                sourceTableUpdating = true;
                while (q.next()) {
                    addSourceRow(q.value(0).toString(),
                                 q.value(1).toInt() != 0,
                                 q.value(2).toInt() != 0,
                                 q.value(3).toInt() != 0,
                                 q.value(4).toInt() != 0,
                                 q.value(5).toInt() != 0,
                                 q.value(6).toInt() != 0,
                                 q.value(7).toInt(),
                                 q.value(8).toString(),
                                 q.value(9).toString());
                    ++loadedCount;
                }
                sourceTableUpdating = false;

                sourceTable->sortItems(ColProfile, Qt::AscendingOrder);
                updateSourceCounter();
                removeDisabledProfilesFromQueue();

                profilesDbPath = fileName;
                settings->setValue("lastProfilesDb", profilesDbPath);
                profilesFilePath.clear();
                settings->setValue("lastProfilesFile", QString());
                sourceBackedByDatabase = true;
                lastSourceKind = "db";
                settings->setValue("lastSourceKind", lastSourceKind);
                log(tr("Loaded %1 profiles from %2").arg(loadedCount).arg(fileName));
                ok = true;
            }
        }
    }

    QSqlDatabase::removeDatabase(connectionName);
    return ok;
}

bool MainWindow::saveProfilesDatabase()
{
    if (!sourceBackedByDatabase || profilesDbPath.isEmpty()) {
        return true;
    }
    if (!ensureProfilesDatabaseSchema(profilesDbPath)) {
        return false;
    }

    const QString connectionName = QString("profiles_save_%1")
            .arg(QUuid::createUuid().toString(QUuid::WithoutBraces));

    bool ok = false;
    {
        QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE", connectionName);
        db.setDatabaseName(profilesDbPath);
        if (!db.open()) {
            log(tr("Database error: %1").arg(db.lastError().text()), QColor(Qt::red));
        } else {
            QSqlQuery q(db);
            if (!db.transaction()) {
                log(tr("Database error: %1").arg(db.lastError().text()), QColor(Qt::red));
            } else if (!q.exec("DELETE FROM profiles")) {
                log(tr("Database error: %1").arg(q.lastError().text()), QColor(Qt::red));
                db.rollback();
            } else if (!q.prepare(
                           "INSERT OR REPLACE INTO profiles "
                           "(profile_name, enabled, videos, stories, highlights, tagged, reels, rating, last_updated, videos_size) "
                           "VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?)")) {
                log(tr("Database error: %1").arg(q.lastError().text()), QColor(Qt::red));
                db.rollback();
            } else {
                bool insertOk = true;
                auto safeText = [](QTableWidgetItem *it) -> QString {
                    if (!it) return QString("");
                    const QString t = it->text();
                    return t.isNull() ? QString("") : t;
                };
                for (int row = 0; row < sourceTable->rowCount(); ++row) {
                    q.addBindValue(sourceProfileAtRow(row));
                    q.addBindValue(sourceCheckAtRow(row, ColEnabled) ? 1 : 0);
                    q.addBindValue(sourceCheckAtRow(row, ColVideos) ? 1 : 0);
                    q.addBindValue(sourceCheckAtRow(row, ColStories) ? 1 : 0);
                    q.addBindValue(sourceCheckAtRow(row, ColHighlights) ? 1 : 0);
                    q.addBindValue(sourceCheckAtRow(row, ColTagged) ? 1 : 0);
                    q.addBindValue(sourceCheckAtRow(row, ColReels) ? 1 : 0);
                    q.addBindValue(sourceRatingAtRow(row));
                    q.addBindValue(safeText(sourceTable->item(row, ColLastUpdated)));
                    q.addBindValue(safeText(sourceTable->item(row, ColVideosSize)));
                    if (!q.exec()) {
                        log(tr("Database error: %1").arg(q.lastError().text()), QColor(Qt::red));
                        insertOk = false;
                        break;
                    }
                }

                if (insertOk && db.commit()) {
                    ok = true;
                } else if (insertOk) {
                    log(tr("Database error: %1").arg(db.lastError().text()), QColor(Qt::red));
                    db.rollback();
                } else {
                    db.rollback();
                }
            }
        }
    }

    QSqlDatabase::removeDatabase(connectionName);
    return ok;
}

void MainWindow::clearSourceTable()
{
    if (!sourceTable) return;
    sourceTableUpdating = true;
    sourceTable->setRowCount(0);
    sourceTableUpdating = false;
    updateSourceCounter();
}

void MainWindow::addSourceRow(const QString &profile,
                              bool enabled,
                              bool videos,
                              bool stories,
                              bool highlights,
                              bool tagged,
                              bool reels,
                              int rating,
                              const QString &lastUpdated,
                              const QString &videosSize)
{
    if (!sourceTable) return;
    const bool sortingWasEnabled = sourceTable->isSortingEnabled();
    if (sortingWasEnabled) {
        sourceTable->setSortingEnabled(false);
    }

    const int row = sourceTable->rowCount();
    sourceTable->insertRow(row);
    sourceTable->setItem(row, ColEnabled, makeCheckItem(enabled));
    sourceTable->setItem(row, ColProfile, makeTextItem(profile));
    sourceTable->setItem(row, ColRating, makeRatingItem(rating));
    sourceTable->setItem(row, ColVideos, makeCheckItem(videos));
    sourceTable->setItem(row, ColStories, makeCheckItem(stories));
    sourceTable->setItem(row, ColHighlights, makeCheckItem(highlights));
    sourceTable->setItem(row, ColTagged, makeCheckItem(tagged));
    sourceTable->setItem(row, ColReels, makeCheckItem(reels));
    sourceTable->setItem(row, ColLastUpdated, makeTextItem(lastUpdated));
    sourceTable->setItem(row, ColVideosSize, makeSizeItem(videosSize));

    if (sourceSearchEdit && !sourceSearchEdit->text().trimmed().isEmpty()) {
        const bool match = profile.contains(sourceSearchEdit->text().trimmed(), Qt::CaseInsensitive);
        sourceTable->setRowHidden(row, !match);
    }

    if (sortingWasEnabled) {
        sourceTable->setSortingEnabled(true);
    }
}

QTableWidgetItem *MainWindow::makeCheckItem(bool checked) const
{
    auto *item = new CheckStateTableWidgetItem();
    item->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsUserCheckable);
    item->setCheckState(checked ? Qt::Checked : Qt::Unchecked);
    return item;
}

QTableWidgetItem *MainWindow::makeTextItem(const QString &text) const
{
    auto *item = new QTableWidgetItem(text.isNull() ? QString("") : text);
    item->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
    return item;
}

QTableWidgetItem *MainWindow::makeSizeItem(const QString &text) const
{
    auto *item = new DataSizeTableWidgetItem(text.isNull() ? QString("") : text);
    item->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
    return item;
}

QTableWidgetItem *MainWindow::makeRatingItem(int rating) const
{
    auto *item = new RatingTableWidgetItem(rating);
    item->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
    return item;
}

QString MainWindow::sourceProfileAtRow(int row) const
{
    if (!sourceTable || row < 0 || row >= sourceTable->rowCount()) return QString();
    QTableWidgetItem *item = sourceTable->item(row, ColProfile);
    return item ? item->text() : QString();
}

bool MainWindow::sourceCheckAtRow(int row, int column) const
{
    if (!sourceTable || row < 0 || row >= sourceTable->rowCount()) return false;
    QTableWidgetItem *item = sourceTable->item(row, column);
    return item && item->checkState() == Qt::Checked;
}

int MainWindow::sourceRatingAtRow(int row) const
{
    if (!sourceTable || row < 0 || row >= sourceTable->rowCount()) return 0;
    QTableWidgetItem *item = sourceTable->item(row, ColRating);
    if (!item) return 0;

    bool ok = false;
    const int rating = item->data(Qt::UserRole).toInt(&ok);
    if (!ok) {
        return 0;
    }
    return clampProfileRating(rating);
}

int MainWindow::findSourceRowByProfile(const QString &profile) const
{
    if (!sourceTable) return -1;
    for (int row = 0; row < sourceTable->rowCount(); ++row) {
        if (sourceProfileAtRow(row).compare(profile, Qt::CaseInsensitive) == 0) {
            return row;
        }
    }
    return -1;
}

bool MainWindow::isSourceProfileEnabled(const QString &profile) const
{
    const int row = findSourceRowByProfile(profile);
    if (row < 0) {
        return false;
    }
    return sourceCheckAtRow(row, ColEnabled);
}

void MainWindow::disableSourceProfile(const QString &profile)
{
    const int row = findSourceRowByProfile(profile);
    if (row < 0) {
        return;
    }

    if (!sourceCheckAtRow(row, ColEnabled)) {
        return;
    }

    sourceTableUpdating = true;
    if (QTableWidgetItem *enabledItem = sourceTable->item(row, ColEnabled)) {
        enabledItem->setCheckState(Qt::Unchecked);
    }
    sourceTableUpdating = false;

    removeProfileFromQueue(profile);
    saveResumeFile();
    saveProfilesDatabase();

    QString skippedPath = QDir(workingDir).filePath("skipped.txt");
    QSet<QString> skippedSet;
    QFile skippedFile(skippedPath);
    if (skippedFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&skippedFile);
        while (!in.atEnd()) {
            skippedSet.insert(in.readLine().trimmed().toLower());
        }
        skippedFile.close();
    }

    const QString key = profile.toLower();
    if (!skippedSet.contains(key) && skippedFile.open(QIODevice::Append | QIODevice::Text)) {
        QTextStream out(&skippedFile);
        out << profile << "\n";
        skippedFile.close();
    }
}

void MainWindow::removeProfileFromQueue(const QString &profile)
{
    for (int i = queueProfiles.size() - 1; i >= 0; --i) {
        if (queueProfiles[i].compare(profile, Qt::CaseInsensitive) == 0) {
            queueProfiles.remove(i);
        }
    }
    queueList->clear();
    for (const auto &p : queueProfiles) {
        queueList->addItem(p);
    }
    queueList->sortItems(Qt::AscendingOrder);
    updateQueueCounter();
}

void MainWindow::removeSourceRow(int row)
{
    if (!sourceTable || row < 0 || row >= sourceTable->rowCount()) return;
    const QString profile = sourceProfileAtRow(row);

    sourceTableUpdating = true;
    sourceTable->removeRow(row);
    sourceTableUpdating = false;
    updateSourceCounter();

    removeProfileFromQueue(profile);

    if (sourceBackedByDatabase) {
        saveProfilesDatabase();
    } else {
        saveProfilesFile();
    }
    saveResumeFile();
}

bool MainWindow::removeProfileFolder(const QString &profile)
{
    const QString profilePath = QDir(workingDir).filePath(profile);
    QDir dir(profilePath);
    if (!dir.exists()) {
        log(tr("Profile folder does not exist: %1").arg(profilePath), QColor(Qt::red));
        return false;
    }
    if (!dir.removeRecursively()) {
        log(tr("Failed to remove profile folder: %1").arg(profilePath), QColor(Qt::red));
        return false;
    }
    return true;
}

int MainWindow::clearVideoFilesForProfile(const QString &profile)
{
    const QString profilePath = QDir(workingDir).filePath(profile);
    if (!QDir(profilePath).exists()) {
        log(tr("Profile folder does not exist: %1").arg(profilePath), QColor(Qt::red));
        return -1;
    }

    const QSet<QString> exts = {"mp4", "mov", "webm"};
    int removed = 0;

    QDirIterator it(profilePath, QDir::Files, QDirIterator::Subdirectories);
    while (it.hasNext()) {
        const QString filePath = it.next();
        const QFileInfo fi(filePath);
        if (!exts.contains(fi.suffix().toLower())) continue;
        if (QFile::remove(filePath)) {
            ++removed;
        }
    }

    return removed;
}

qint64 MainWindow::calculateVideoSizeBytesForProfile(const QString &profile) const
{
    const QString profilePath = QDir(workingDir).filePath(profile);
    if (!QDir(profilePath).exists()) {
        return 0;
    }

    const QSet<QString> exts = {"mp4", "mov", "webm"};
    qint64 bytes = 0;

    QDirIterator it(profilePath, QDir::Files, QDirIterator::Subdirectories);
    while (it.hasNext()) {
        it.next();
        const QFileInfo fi = it.fileInfo();
        if (!exts.contains(fi.suffix().toLower())) continue;
        bytes += fi.size();
    }

    return bytes;
}

QString MainWindow::formatDataSizeBinary(qint64 bytes) const
{
    if (bytes < 0) return QString();

    static const QStringList units = {QStringLiteral("B"), QStringLiteral("KB"), QStringLiteral("MB"), QStringLiteral("GB")};
    double value = static_cast<double>(bytes);
    int unitIndex = 0;
    while (value >= 1024.0 && unitIndex < units.size() - 1) {
        value /= 1024.0;
        ++unitIndex;
    }

    if (unitIndex == 0) {
        return QString("%1 %2").arg(static_cast<qlonglong>(value)).arg(units[unitIndex]);
    }

    int decimals = 2;
    if (value >= 100.0) {
        decimals = 0;
    } else if (value >= 10.0) {
        decimals = 1;
    }
    return QString("%1 %2").arg(QLocale().toString(value, 'f', decimals)).arg(units[unitIndex]);
}

bool MainWindow::recalculateVideoSizeForProfile(const QString &profile, bool persistNow, bool writeLog)
{
    const int row = findSourceRowByProfile(profile);
    if (row < 0) {
        return false;
    }

    const qint64 bytes = calculateVideoSizeBytesForProfile(profile);
    const QString sizeText = formatDataSizeBinary(bytes);
    sourceTableUpdating = true;
    if (QTableWidgetItem *item = sourceTable->item(row, ColVideosSize)) {
        item->setText(sizeText);
        item->setData(Qt::UserRole, bytes);
    } else {
        auto *sizeItem = makeSizeItem(sizeText);
        sizeItem->setData(Qt::UserRole, bytes);
        sourceTable->setItem(row, ColVideosSize, sizeItem);
    }
    sourceTableUpdating = false;

    if (persistNow) {
        saveProfilesDatabase();
    }

    if (writeLog) {
        log(tr("Video size for %1: %2").arg(profile, sizeText), QColor(Qt::darkCyan));
    }
    return true;
}

void MainWindow::updateLastUpdatedForProfile(const QString &profile, const QString &value)
{
    const int row = findSourceRowByProfile(profile);
    if (row < 0) {
        return;
    }

    sourceTableUpdating = true;
    if (QTableWidgetItem *item = sourceTable->item(row, ColLastUpdated)) {
        item->setText(value);
    } else {
        sourceTable->setItem(row, ColLastUpdated, makeTextItem(value));
    }
    sourceTableUpdating = false;
}

QString MainWindow::instaloaderScriptPathFromSettings() const
{
    const QString scriptPath = settings->value("instaloader/scriptPath",
                                               QString::fromLatin1(kDefaultInstaloaderPath)).toString().trimmed();
    return scriptPath.isEmpty() ? QString::fromLatin1(kDefaultInstaloaderPath) : scriptPath;
}

QStringList MainWindow::instaloaderCommonArgsFromSettings() const
{
    QStringList args;

    const QString login = settings->value("instaloader/login", QString()).toString().trimmed();
    if (!login.isEmpty()) {
        args << QString("--login=%1").arg(login);
    }
    if (settings->value("instaloader/noMetadata", true).toBool()) {
        args << "--no-metadata-json";
    }
    if (settings->value("instaloader/noCaptions", true).toBool()) {
        args << "--no-captions";
    }
    QString userAgent = settings->value("instaloader/userAgent",
                                        QString::fromLatin1(kDefaultUserAgent)).toString().simplified();
    if (!userAgent.isEmpty()) {
        args << QString("--user-agent=%1").arg(userAgent);
    }
    if (settings->value("instaloader/noIphone", true).toBool()) {
        args << "--no-iphone";
    }

    QStringList abortCodes;
    if (settings->value("instaloader/abort400", true).toBool()) abortCodes << "400";
    if (settings->value("instaloader/abort401", true).toBool()) abortCodes << "401";
    if (settings->value("instaloader/abort403", true).toBool()) abortCodes << "403";
    if (settings->value("instaloader/abort404", true).toBool()) abortCodes << "404";
    if (settings->value("instaloader/abort429", true).toBool()) abortCodes << "429";
    if (!abortCodes.isEmpty()) {
        args << QString("--abort-on=%1").arg(abortCodes.join(","));
    }

    return args;
}

int MainWindow::batchQueueLimitFromSettings() const
{
    const int configured = settings->value("service/batchQueueLimit", 50).toInt();
    return (configured > 0) ? configured : 50;
}

bool MainWindow::shouldCountVideoSizeAutomatically() const
{
    if (actCountVideoSize) {
        return actCountVideoSize->isChecked();
    }
    return settings->value("service/countVideoSize", false).toBool();
}

bool MainWindow::promptNewProfileOptions(const QString &profile, ProfileDefaults *outDefaults)
{
    if (!outDefaults) return false;

    QDialog dialog(this);
    dialog.setWindowTitle(tr("New profile options"));
    dialog.setModal(true);
    dialog.resize(320, 260);

    QVBoxLayout *layout = new QVBoxLayout(&dialog);
    QLabel *caption = new QLabel(tr("Select what to download for: %1").arg(profile), &dialog);
    caption->setWordWrap(true);
    layout->addWidget(caption);

    QCheckBox *enabledCheck = new QCheckBox(tr("Enabled"), &dialog);
    QCheckBox *videosCheck = new QCheckBox(tr("Video"), &dialog);
    QCheckBox *storiesCheck = new QCheckBox(tr("Stories"), &dialog);
    QCheckBox *highlightsCheck = new QCheckBox(tr("Highlights"), &dialog);
    QCheckBox *taggedCheck = new QCheckBox(tr("Tagged"), &dialog);
    QCheckBox *reelsCheck = new QCheckBox(tr("Reels"), &dialog);

    enabledCheck->setChecked(outDefaults->enabled);
    videosCheck->setChecked(outDefaults->videos);
    storiesCheck->setChecked(outDefaults->stories);
    highlightsCheck->setChecked(outDefaults->highlights);
    taggedCheck->setChecked(outDefaults->tagged);
    reelsCheck->setChecked(outDefaults->reels);

    layout->addWidget(enabledCheck);
    layout->addWidget(videosCheck);
    layout->addWidget(storiesCheck);
    layout->addWidget(highlightsCheck);
    layout->addWidget(taggedCheck);
    layout->addWidget(reelsCheck);

    QDialogButtonBox *box = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, &dialog);
    connect(box, &QDialogButtonBox::accepted, &dialog, &QDialog::accept);
    connect(box, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);
    layout->addWidget(box);

    if (dialog.exec() != QDialog::Accepted) {
        return false;
    }

    outDefaults->enabled = enabledCheck->isChecked();
    outDefaults->videos = videosCheck->isChecked();
    outDefaults->stories = storiesCheck->isChecked();
    outDefaults->highlights = highlightsCheck->isChecked();
    outDefaults->tagged = taggedCheck->isChecked();
    outDefaults->reels = reelsCheck->isChecked();
    return true;
}

MainWindow::ProfileDefaults MainWindow::profileDefaultsFromSettings() const
{
    ProfileDefaults defaults;
    defaults.enabled = settings->value("defaults/enabled", true).toBool();
    defaults.videos = settings->value("defaults/videos", true).toBool();
    defaults.stories = settings->value("defaults/stories", true).toBool();
    defaults.highlights = settings->value("defaults/highlights", true).toBool();
    defaults.tagged = settings->value("defaults/tagged", true).toBool();
    defaults.reels = settings->value("defaults/reels", true).toBool();
    return defaults;
}

void MainWindow::applyProfileDefaultsToSettings(const ProfileDefaults &defaults)
{
    settings->setValue("defaults/enabled", defaults.enabled);
    settings->setValue("defaults/videos", defaults.videos);
    settings->setValue("defaults/stories", defaults.stories);
    settings->setValue("defaults/highlights", defaults.highlights);
    settings->setValue("defaults/tagged", defaults.tagged);
    settings->setValue("defaults/reels", defaults.reels);
    settings->sync();
}

void MainWindow::removeMissingProfiles()
{
    if (workingDir.isEmpty()) {
        log(tr("Working directory is empty."), QColor(Qt::red));
        return;
    }

    QDir wd(workingDir);
    int removedSource = 0;
    int removedQueue = 0;

    // Source table
    sourceTableUpdating = true;
    for (int i = sourceTable->rowCount() - 1; i >= 0; --i) {
        const QString profile = sourceProfileAtRow(i);
        const QString path = wd.filePath(profile);
        if (!QDir(path).exists()) {
            sourceTable->removeRow(i);
            ++removedSource;
        }
    }
    sourceTableUpdating = false;

    // Queue list + queueProfiles (so it doesn't try to process missing ones)
    for (int i = queueList->count() - 1; i >= 0; --i) {
        QListWidgetItem *item = queueList->item(i);
        const QString profile = item->text();
        const QString path = wd.filePath(profile);
        if (!QDir(path).exists()) {
            delete queueList->takeItem(i);
            for (int j = queueProfiles.size() - 1; j >= 0; --j) {
                if (queueProfiles[j].compare(profile, Qt::CaseInsensitive) == 0) {
                    queueProfiles.remove(j);
                    break;
                }
            }
            ++removedQueue;
        }
    }

    if (removedSource == 0 && removedQueue == 0) {
        log(tr("Remove Missing: nothing to remove (all folders exist)."), QColor(Qt::green));
        return;
    }

    // Keep UI tidy
    sourceTable->sortItems(ColProfile, Qt::AscendingOrder);
    queueList->sortItems(Qt::AscendingOrder);
    updateSourceCounter();
    updateQueueCounter();

    // Persist changes
    if (sourceBackedByDatabase) {
        saveProfilesDatabase();
    } else {
        saveProfilesFile();
    }
    saveResumeFile();

    log(tr("Remove Missing: removed %1 from source and %2 from queue.").arg(removedSource).arg(removedQueue), QColor(Qt::yellow));
}

void MainWindow::checkResumeFile()
{
    QString resumePath = QDir(workingDir).filePath("resume.txt");
    if (QFile::exists(resumePath)) {
        queueProfiles.clear();
        queueList->clear();

        QFile resumeFile(resumePath);
        if (!resumeFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
            log(tr("Error opening resume.txt"), QColor(Qt::red));
            return;
        }

        QTextStream in(&resumeFile);
        while (!in.atEnd()) {
            QString profile = in.readLine().trimmed();
            if (!profile.isEmpty()) {
                queueList->addItem(profile);
                queueProfiles.append(profile);
            }
        }
        resumeFile.close();

        // Sort the queue list
        queueList->sortItems(Qt::AscendingOrder);

        usingResume = true;
        log(tr("Loaded %1 profiles from resume.txt into queue.").arg(queueProfiles.size()));
    }
}

void MainWindow::addToQueue()
{
    QSet<QString> existingQueue;
    for (const auto& p : queueProfiles) {
        existingQueue.insert(p.toLower());
    }

    QStringList added;
    bool sourceChanged = false;
    sourceTableUpdating = true;
    for (int i = 0; i < sourceTable->rowCount(); ++i) {
        if (sourceCheckAtRow(i, ColEnabled)) {
            QString profile = sourceProfileAtRow(i);
            if (!existingQueue.contains(profile.toLower())) {
                queueList->addItem(profile);
                queueProfiles.append(profile);
                added.append(profile);
                existingQueue.insert(profile.toLower());
            }
            if (QTableWidgetItem *enabledItem = sourceTable->item(i, ColEnabled)) {
                enabledItem->setCheckState(Qt::Unchecked);  // Uncheck after adding
                sourceChanged = true;
            }
        }
    }
    sourceTableUpdating = false;

    if (sourceChanged) {
        saveProfilesDatabase();
    }

    if (!added.isEmpty()) {
        queueList->sortItems(Qt::AscendingOrder);
        log(tr("Added %1 profiles to queue.").arg(added.size()));

        // If processing is ongoing, add new to selectedProfiles
        if (isProcessing) {
            for (const auto& np : added) {
                selectedProfiles.append(np);
            }
            totalProfiles += added.size();  // Update total
            progressBar->setRange(0, totalProfiles);
        }

        saveResumeFile();  // Save updated queue to resume.txt
    } else {
        log(tr("No new profiles added to queue (duplicates ignored)."));
    }
}

void MainWindow::addProfilesByRatingRange(int minRating, int maxRating)
{
    if (!sourceTable || sourceTable->rowCount() == 0) {
        log(tr("No source profiles loaded."), QColor(Qt::yellow));
        return;
    }

    const int boundedMin = clampProfileRating(std::min(minRating, maxRating));
    const int boundedMax = clampProfileRating(std::max(minRating, maxRating));

    QSet<QString> existingQueue;
    for (const auto &p : queueProfiles) {
        existingQueue.insert(p.toLower());
    }

    QStringList added;
    for (int row = 0; row < sourceTable->rowCount(); ++row) {
        if (!sourceCheckAtRow(row, ColEnabled)) {
            continue;
        }
        const int rating = sourceRatingAtRow(row);
        if (rating < boundedMin || rating > boundedMax) {
            continue;
        }

        const QString profile = sourceProfileAtRow(row);
        if (profile.isEmpty()) {
            continue;
        }

        const QString key = profile.toLower();
        if (existingQueue.contains(key)) {
            continue;
        }

        queueProfiles.append(profile);
        queueList->addItem(profile);
        existingQueue.insert(key);
        added.append(profile);
    }

    if (added.isEmpty()) {
        log(tr("No new profiles added to queue (duplicates ignored)."));
        return;
    }

    queueList->sortItems(Qt::AscendingOrder);
    updateQueueCounter();

    if (isProcessing) {
        for (const auto &np : added) {
            selectedProfiles.append(np);
        }
        totalProfiles += added.size();
        progressBar->setRange(0, totalProfiles);
        progressBar->setValue(processed);
    }

    saveResumeFile();
    log(tr("Added %1 profiles to queue.").arg(added.size()));
}

void MainWindow::addOldestProfilesToQueue()
{
    if (!sourceTable || sourceTable->rowCount() == 0) {
        log(tr("No source profiles loaded."), QColor(Qt::yellow));
        return;
    }

    struct Candidate {
        QString profile;
        bool hasValidDate = false;
        QDateTime lastUpdated;
    };

    QVector<Candidate> candidates;
    candidates.reserve(sourceTable->rowCount());

    for (int row = 0; row < sourceTable->rowCount(); ++row) {
        if (!sourceCheckAtRow(row, ColEnabled)) {
            continue;
        }
        const QString profile = sourceProfileAtRow(row);
        if (profile.isEmpty()) {
            continue;
        }

        QString lastUpdatedText;
        if (QTableWidgetItem *item = sourceTable->item(row, ColLastUpdated)) {
            lastUpdatedText = item->text().trimmed();
        }

        QDateTime parsedDate;
        bool hasValidDate = false;
        if (!lastUpdatedText.isEmpty()) {
            parsedDate = QDateTime::fromString(lastUpdatedText, "yyyy-MM-dd HH:mm:ss");
            if (!parsedDate.isValid()) {
                parsedDate = QDateTime::fromString(lastUpdatedText, Qt::ISODate);
            }
            if (!parsedDate.isValid()) {
                parsedDate = QDateTime::fromString(lastUpdatedText, Qt::ISODateWithMs);
            }
            hasValidDate = parsedDate.isValid();
        }

        Candidate candidate;
        candidate.profile = profile;
        candidate.hasValidDate = hasValidDate;
        candidate.lastUpdated = parsedDate;
        candidates.append(candidate);
    }

    if (candidates.isEmpty()) {
        log(tr("No source profiles loaded."), QColor(Qt::yellow));
        return;
    }

    std::sort(candidates.begin(), candidates.end(), [](const Candidate &a, const Candidate &b) {
        if (a.hasValidDate != b.hasValidDate) {
            return !a.hasValidDate;
        }
        if (a.hasValidDate && b.hasValidDate && a.lastUpdated != b.lastUpdated) {
            return a.lastUpdated < b.lastUpdated;
        }
        return a.profile.compare(b.profile, Qt::CaseInsensitive) < 0;
    });

    const int limit = batchQueueLimitFromSettings();
    QSet<QString> existingQueue;
    for (const auto &p : queueProfiles) {
        existingQueue.insert(p.toLower());
    }

    QStringList toAdd;
    toAdd.reserve(limit);
    for (const auto &candidate : candidates) {
        const QString key = candidate.profile.toLower();
        if (existingQueue.contains(key)) {
            continue;
        }
        toAdd.append(candidate.profile);
        existingQueue.insert(key);
        if (toAdd.size() >= limit) {
            break;
        }
    }

    if (toAdd.isEmpty()) {
        log(tr("No new profiles added to queue (duplicates ignored)."));
        return;
    }

    for (const auto &profile : toAdd) {
        queueProfiles.append(profile);
        queueList->addItem(profile);
    }

    queueList->sortItems(Qt::AscendingOrder);
    updateQueueCounter();

    if (isProcessing) {
        for (const auto &np : toAdd) {
            selectedProfiles.append(np);
        }
        totalProfiles += toAdd.size();
        progressBar->setRange(0, totalProfiles);
        progressBar->setValue(processed);
    }

    saveResumeFile();
    log(tr("Added %1 oldest profiles to queue (limit: %2).").arg(toAdd.size()).arg(limit));
}

void MainWindow::removeDisabledProfilesFromQueue()
{
    if (!sourceTable || queueProfiles.isEmpty()) {
        return;
    }

    bool changed = false;
    for (int i = queueProfiles.size() - 1; i >= 0; --i) {
        const QString profile = queueProfiles[i];
        const int row = findSourceRowByProfile(profile);
        if (row >= 0 && !sourceCheckAtRow(row, ColEnabled)) {
            queueProfiles.remove(i);
            changed = true;
        }
    }

    if (!changed) {
        return;
    }

    queueList->clear();
    for (const auto &p : queueProfiles) {
        queueList->addItem(p);
    }
    queueList->sortItems(Qt::AscendingOrder);
    updateQueueCounter();
    saveResumeFile();
}

void MainWindow::clearQueue()
{
    queueList->clear();
    queueProfiles.clear();
    QString resumePath = QDir(workingDir).filePath("resume.txt");
    QFile::remove(resumePath);
    log(tr("Cleared current tasks and resume.txt."));
}

void MainWindow::addNewProfiles()
{
    QString text = addProfilesEdit->toPlainText().trimmed();
    if (text.isEmpty()) return;

    const bool allowNew = allowNewProfilesCheck && allowNewProfilesCheck->isChecked();

    // Split by whitespace (spaces or newlines)
    QStringList newProfilesList = text.split(QRegularExpression("\\s+"), Qt::SkipEmptyParts);

    // Existing in source
    QSet<QString> existingSource;
    for (int row = 0; row < sourceTable->rowCount(); ++row) {
        existingSource.insert(sourceProfileAtRow(row).toLower());
    }

    const ProfileDefaults defaults = profileDefaultsFromSettings();

    // Existing in queue
    QSet<QString> existingQueue;
    for (const auto& p : queueProfiles) {
        existingQueue.insert(p.toLower());
    }

    // Load skipped for unchecking new in source
    QString skippedPath = QDir(workingDir).filePath("skipped.txt");
    QSet<QString> skippedSet;
    if (QFile::exists(skippedPath)) {
        QFile skippedFile(skippedPath);
        if (skippedFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QTextStream skippedIn(&skippedFile);
            while (!skippedIn.atEnd()) {
                QString skippedProfile = skippedIn.readLine().trimmed().toLower();
                if (!skippedProfile.isEmpty()) {
                    skippedSet.insert(skippedProfile);
                }
            }
            skippedFile.close();
        }
    }

    QStringList addedToSource;
    QStringList addedToQueue;
    QStringList remainingInBox;
    QStringList ignoredAsNew;
    QStringList invalidProfiles;

    if (allowNew) {
        sourceTableUpdating = true;
    }

    for (auto &np : newProfilesList) {
        np = np.trimmed();
        if (np.isEmpty()) continue;

        // Reject invalid names containing '/'
        if (np.contains('/')) {
            invalidProfiles.append(np);
            remainingInBox.append(np);
            log(tr("Invalid profile name (contains '/'): %1").arg(np), QColor(Qt::red));
            continue;
        }

        const QString key = np.toLower();

        if (allowNew) {
            if (!existingSource.contains(key)) {
                ProfileDefaults rowDefaults = defaults;
                if (!promptNewProfileOptions(np, &rowDefaults)) {
                    remainingInBox.append(np);
                    continue;
                }
                const bool enabled = skippedSet.contains(key) ? false : rowDefaults.enabled;
                addSourceRow(np,
                             enabled,
                             rowDefaults.videos,
                             rowDefaults.stories,
                             rowDefaults.highlights,
                             rowDefaults.tagged,
                             rowDefaults.reels);
                addedToSource.append(np);
                existingSource.insert(key);
            }
            if (isSourceProfileEnabled(np) && !existingQueue.contains(key)) {
                queueList->addItem(np);
                queueProfiles.append(np);
                addedToQueue.append(np);
                existingQueue.insert(key);
            }
        } else {
            // "Allow adding new" is OFF: only add to queue if already exists in source list
            if (existingSource.contains(key) && isSourceProfileEnabled(np)) {
                if (!existingQueue.contains(key)) {
                    queueList->addItem(np);
                    queueProfiles.append(np);
                    addedToQueue.append(np);
                    existingQueue.insert(key);
                }
            } else {
                remainingInBox.append(np);
                ignoredAsNew.append(np);
            }
        }
    }

    if (allowNew) {
        sourceTableUpdating = false;
    }

    // Sort what we might have changed
    if (allowNew) {
        sourceTable->sortItems(ColProfile, Qt::AscendingOrder);
        updateSourceCounter();
    }
    queueList->sortItems(Qt::AscendingOrder);
    updateQueueCounter();

    // Update textbox content depending on mode
    if (allowNew) {
        if (remainingInBox.isEmpty()) {
            addProfilesEdit->clear();
        } else {
            addProfilesEdit->setPlainText(remainingInBox.join("\n"));
        }
    } else {
        addProfilesEdit->setPlainText(remainingInBox.join("\n"));
    }

    // Highlight invalid names in the textbox
    highlightInvalidAddProfiles(invalidProfiles);

    if (!addedToSource.isEmpty() || !addedToQueue.isEmpty()) {
        if (allowNew) {
            log(tr("Added %1 to source and %2 to queue.").arg(addedToSource.size()).arg(addedToQueue.size()));
        } else {
            log(tr("Added %1 existing profiles to queue (new blocked: %2).").arg(addedToQueue.size()).arg(ignoredAsNew.size()));
        }

        // Log new profiles to the new log widget (only when actually adding new)
        if (allowNew) {
            for (const auto &np : addedToSource) {
                newProfilesLog->append(np);
            }
            settings->setValue("newProfilesLogText", newProfilesLog->toPlainText());
            settings->sync();
            saveProfilesDatabase();
        }

        // Append to original profiles file only when we really added to source
        if (allowNew && !sourceBackedByDatabase && !profilesFilePath.isEmpty() && !addedToSource.isEmpty()) {
            QFile file(profilesFilePath);
            if (file.open(QIODevice::Append | QIODevice::Text)) {
                QTextStream out(&file);
                for (const auto &p : addedToSource) {
                    out << p << "\n";
                }
                file.close();
            } else {
                log(tr("Error appending to profiles file: %1").arg(profilesFilePath), QColor(Qt::red));
            }
        }

        // If processing is ongoing, add new to selectedProfiles and update total
        if (isProcessing && !addedToQueue.isEmpty()) {
            for (const auto &np : addedToQueue) {
                selectedProfiles.append(np);
            }
            totalProfiles += addedToQueue.size();
            progressBar->setRange(0, totalProfiles);
            progressBar->setValue(processed);
        }

        // Save updated resume.txt
        saveResumeFile();
    } else {
        if (allowNew) {
            log(tr("No new profiles added (duplicates ignored)."));
        } else {
            log(tr("No profiles added: either duplicates or not present in source list."));
        }
    }
}

void MainWindow::showSourceHeaderContextMenu(const QPoint &pos)
{
    if (!sourceTable || !sourceTable->horizontalHeader()) return;

    QHeaderView *header = sourceTable->horizontalHeader();
    const int column = header->logicalIndexAt(pos);
    const bool isCheckboxColumn =
            column == ColEnabled ||
            column == ColVideos ||
            column == ColStories ||
            column == ColHighlights ||
            column == ColTagged ||
            column == ColReels;
    if (!isCheckboxColumn) return;

    QMenu menu(this);
    QAction *selectAllAct = menu.addAction(tr("Select all"));
    QAction *uncheckAllAct = menu.addAction(tr("Uncheck all"));
    QAction *chosen = menu.exec(header->viewport()->mapToGlobal(pos));
    if (!chosen) return;

    const Qt::CheckState targetState = (chosen == selectAllAct) ? Qt::Checked : Qt::Unchecked;
    const bool sortingWasEnabled = sourceTable->isSortingEnabled();

    if (sortingWasEnabled) {
        sourceTable->setSortingEnabled(false);
    }
    sourceTableUpdating = true;
    for (int row = 0; row < sourceTable->rowCount(); ++row) {
        QTableWidgetItem *item = sourceTable->item(row, column);
        if (!item) continue;
        if (!(item->flags() & Qt::ItemIsUserCheckable)) continue;
        item->setCheckState(targetState);
    }
    sourceTableUpdating = false;
    if (sortingWasEnabled) {
        sourceTable->setSortingEnabled(true);
    }

    saveProfilesDatabase();
}

void MainWindow::showSourceContextMenu(const QPoint &pos)
{
    if (!sourceTable) return;

    const QModelIndex idx = sourceTable->indexAt(pos);
    if (!idx.isValid()) return;

    const int row = idx.row();
    const QString profile = sourceProfileAtRow(row);
    if (profile.isEmpty()) return;

    sourceTable->selectRow(row);

    QMenu menu(this);
    QAction *addToQueueAct = menu.addAction(tr("Add to queue"));
    menu.addSeparator();
    QAction *deleteDbAct = menu.addAction(tr("Delete profile from DB"));
    QAction *deleteDbDiskAct = menu.addAction(tr("Delete profile from DB + disk"));
    menu.addSeparator();
    QAction *openDolphinAct = menu.addAction(tr("Open folder in Dolphin"));
    QAction *openGthumbAct = menu.addAction(tr("Open in Gthumb"));
    QAction *clearVideosAct = menu.addAction(tr("Clear video files"));

    QAction *chosen = menu.exec(sourceTable->viewport()->mapToGlobal(pos));
    if (!chosen) return;

    if (chosen == addToQueueAct) {
        if (!isSourceProfileEnabled(profile)) {
            log(tr("Profile %1 is disabled and cannot be added to queue.").arg(profile), QColor(Qt::yellow));
            return;
        }

        bool alreadyInQueue = false;
        for (const QString &queued : queueProfiles) {
            if (queued.compare(profile, Qt::CaseInsensitive) == 0) {
                alreadyInQueue = true;
                break;
            }
        }

        if (alreadyInQueue) {
            log(tr("Profile %1 is already in queue.").arg(profile), QColor(Qt::yellow));
            return;
        }

        queueProfiles.append(profile);
        queueList->addItem(profile);
        queueList->sortItems(Qt::AscendingOrder);
        updateQueueCounter();
        log(tr("Profile %1 added to queue.").arg(profile), QColor(Qt::green));

        if (isProcessing) {
            selectedProfiles.append(profile);
            totalProfiles += 1;
            progressBar->setRange(0, totalProfiles);
            progressBar->setValue(processed);
        }

        saveResumeFile();
        return;
    }

    if (chosen == deleteDbAct) {
        const auto answer = QMessageBox::question(
                this,
                tr("Delete profile from DB"),
                tr("Delete profile from DB: %1?").arg(profile),
                QMessageBox::Yes | QMessageBox::No,
                QMessageBox::No);
        if (answer == QMessageBox::Yes) {
            const int liveRow = findSourceRowByProfile(profile);
            if (liveRow >= 0) {
                removeSourceRow(liveRow);
            }
        }
        return;
    }

    if (chosen == deleteDbDiskAct) {
        const auto answer = QMessageBox::question(
                this,
                tr("Delete profile from DB + disk"),
                tr("Delete profile from DB + disk: %1?").arg(profile),
                QMessageBox::Yes | QMessageBox::No,
                QMessageBox::No);
        if (answer == QMessageBox::Yes) {
            removeProfileFolder(profile);
            const int liveRow = findSourceRowByProfile(profile);
            if (liveRow >= 0) {
                removeSourceRow(liveRow);
            }
        }
        return;
    }

    const QString profilePath = QDir(workingDir).filePath(profile);
    if (!QDir(profilePath).exists()) {
        log(tr("Profile folder does not exist: %1").arg(profilePath), QColor(Qt::red));
        return;
    }

    if (chosen == openDolphinAct) {
        if (!QProcess::startDetached("dolphin", QStringList() << profilePath)) {
            log(tr("Failed to start Dolphin for %1").arg(profilePath), QColor(Qt::red));
        }
        return;
    }

    if (chosen == openGthumbAct) {
        if (!QProcess::startDetached("gthumb", QStringList() << profilePath)) {
            log(tr("Failed to start Gthumb for %1").arg(profilePath), QColor(Qt::red));
        }
        return;
    }

    if (chosen == clearVideosAct) {
        const auto answer = QMessageBox::question(
                this,
                tr("Clear video files"),
                tr("Delete video files for %1?").arg(profile),
                QMessageBox::Yes | QMessageBox::No,
                QMessageBox::No);
        if (answer == QMessageBox::Yes) {
            const int removed = clearVideoFilesForProfile(profile);
            if (removed >= 0) {
                log(tr("Removed %1 video files for %2").arg(removed).arg(profile), QColor(Qt::yellow));
                recalculateVideoSizeForProfile(profile, true, false);
            }
        }
    }
}

void MainWindow::onSourceTableItemChanged(QTableWidgetItem *item)
{
    if (sourceTableUpdating) return;

    if (item && item->column() == ColEnabled && item->checkState() != Qt::Checked) {
        const QString profile = sourceProfileAtRow(item->row());
        if (!profile.isEmpty()) {
            removeProfileFromQueue(profile);
            saveResumeFile();
        }
    }
    saveProfilesDatabase();
}

void MainWindow::onSourceSearchTextChanged(const QString &text)
{
    if (!sourceTable) return;
    const QString needle = text.trimmed();
    for (int row = 0; row < sourceTable->rowCount(); ++row) {
        const QString profile = sourceProfileAtRow(row);
        const bool match = needle.isEmpty() || profile.contains(needle, Qt::CaseInsensitive);
        sourceTable->setRowHidden(row, !match);
    }
}

void MainWindow::onCountVideoSizeToggled(bool enabled)
{
    settings->setValue("service/countVideoSize", enabled);
    settings->sync();
}

void MainWindow::recalculateVideoSizes()
{
    if (!sourceTable || sourceTable->rowCount() == 0) {
        log(tr("No profiles to recalculate video size."), QColor(Qt::yellow));
        return;
    }

    int updated = 0;
    for (int row = 0; row < sourceTable->rowCount(); ++row) {
        const QString profile = sourceProfileAtRow(row);
        if (profile.isEmpty()) continue;
        if (recalculateVideoSizeForProfile(profile, false, false)) {
            ++updated;
        }
    }

    saveProfilesDatabase();
    log(tr("Recalculated video size for %1 profiles.").arg(updated), QColor(Qt::green));
}

void MainWindow::showAboutDialog()
{
    const QString programName = QStringLiteral("Instagram Tasker Qt");
    const QString version = QStringLiteral("1.0");
    const QString author = QStringLiteral("Roman Gromov");
    const QString license = QStringLiteral("GPL 2.0");
    const QString licenseUrl = QStringLiteral("https://www.gnu.org/licenses/old-licenses/gpl-2.0.html");
    const QString authorRepoUrl = QStringLiteral("https://github.com/sapphirepro/");
    const QString projectRepoUrl = QStringLiteral("https://github.com/sapphirepro/instagramtaskerqt");

    const QString text = tr(
            "<p><b>%1</b> %2<br>"
            "<b>%3</b> %4<br>"
            "<b>%5</b> %6<br>"
            "<b>%7</b> %8<br>"
            "<b>%9</b> <a href=\"%10\">%10</a></p>"
            "<p><b>%11</b> <a href=\"%12\">%12</a><br>"
            "<b>%13</b> <a href=\"%14\">%14</a></p>")
            .arg(tr("Program:"),
                 programName,
                 tr("Version:"),
                 version,
                 tr("Author:"),
                 author,
                 tr("License:"),
                 license,
                 tr("License text:"),
                 licenseUrl,
                 tr("Author repository:"),
                 authorRepoUrl,
                 tr("Project repository and support:"),
                 projectRepoUrl);

    QMessageBox box(this);
    box.setWindowTitle(tr("About"));
    box.setTextFormat(Qt::RichText);
    box.setTextInteractionFlags(Qt::TextBrowserInteraction);
    box.setText(text);
    box.setStandardButtons(QMessageBox::Ok);
    const QList<QLabel*> labels = box.findChildren<QLabel*>();
    for (QLabel *label : labels) {
        if (!label) continue;
        label->setOpenExternalLinks(true);
        label->setTextInteractionFlags(Qt::TextBrowserInteraction);
    }
    box.exec();
}

void MainWindow::showLogContextMenu(const QPoint &pos)
{
    QMenu *menu = logTextEdit->createStandardContextMenu();
    menu->addSeparator();
    QAction *clearAct = menu->addAction(tr("Clear Log"));
    connect(clearAct, &QAction::triggered, this, [this]() {
        logTextEdit->clear();
    });
    menu->exec(logTextEdit->mapToGlobal(pos));
    delete menu;
}

void MainWindow::showNewProfilesContextMenu(const QPoint &pos)
{
    QMenu *menu = newProfilesLog->createStandardContextMenu();
    menu->addSeparator();
    QAction *clearAct = menu->addAction(tr("Clear New Profiles"));
    connect(clearAct, &QAction::triggered, this, [this]() {
        if (!newProfilesLog) return;
        newProfilesLog->clear();
        settings->setValue("newProfilesLogText", QString());
        settings->sync();
    });
    menu->exec(newProfilesLog->mapToGlobal(pos));
    delete menu;
}

void MainWindow::highlightInvalidAddProfiles(const QStringList &invalid)
{
    QList<QTextEdit::ExtraSelection> selections;

    const QString content = addProfilesEdit->toPlainText();
    if (!content.isEmpty() && !invalid.isEmpty()) {
        QTextCharFormat fmt;
        fmt.setBackground(Qt::red);
        fmt.setForeground(Qt::white);

        for (const QString &tok : invalid) {
            if (tok.isEmpty()) continue;
            int from = 0;
            while (true) {
                int idx = content.indexOf(tok, from);
                if (idx < 0) break;

                const int end = idx + tok.size();
                const bool leftOk = (idx == 0) || content[idx - 1].isSpace();
                const bool rightOk = (end == content.size()) || content[end].isSpace();
                if (leftOk && rightOk) {
                    QTextEdit::ExtraSelection sel;
                    sel.format = fmt;
                    QTextCursor c(addProfilesEdit->document());
                    c.setPosition(idx);
                    c.setPosition(end, QTextCursor::KeepAnchor);
                    sel.cursor = c;
                    selections.append(sel);
                }
                from = end;
            }
        }
    }

    addProfilesEdit->setExtraSelections(selections);
}

void MainWindow::startProcessing()
{
    if (queueProfiles.isEmpty()) {
        QMessageBox::warning(this, tr("No Profiles"), tr("Add profiles to queue first."));
        return;
    }

    selectedProfiles = queueProfiles;
    // Shuffle for randomization
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(selectedProfiles.begin(), selectedProfiles.end(), g);

    totalProfiles = selectedProfiles.size();
    processed = 0;
    success = 0;
    failed = 0;
    skipped = 0;
    paused = false;
    canceled = false;
    isProcessing = true;
    progressBar->setRange(0, totalProfiles);
    progressBar->setValue(0);
    profileProgressBar->reset();  // Reset profile progress
    startButton->setEnabled(false);
    startButton->setText(tr("Start"));
    pauseButton->setText(tr("Pause"));

    log(tr("Starting processing %1 profiles from queue.").arg(totalProfiles));
    processNext();
}

void MainWindow::togglePause()
{
    paused = !paused;
    pauseButton->setText(paused ? tr("Resume") : tr("Pause"));
    if (!paused && !canceled) {
        processNext();
    }
}

void MainWindow::cancelProcessing()
{
    canceled = true;
    if (process->state() == QProcess::Running) {
        process->kill();
    }
    log(tr("Processing canceled."), QColor(Qt::yellow));
    finalizeProcessing();
}

void MainWindow::processNext()
{
    if (paused || canceled || selectedProfiles.isEmpty()) {
        if (selectedProfiles.isEmpty() && !canceled) {
            log(tr("All profiles processed."), QColor(Qt::green));
            finalizeProcessing();
        }
        return;
    }

    checkpointError = false;  // Reset flag for new profile
    profileNotFoundError = false;
    profileProgressBar->reset();  // Reset for new profile

    QString profile = selectedProfiles[0];  // Always process first
    log(tr("Processing profile [%1/%2]: %3").arg(processed + 1).arg(totalProfiles).arg(profile));

    ProfileDefaults flags = profileDefaultsFromSettings();
    const int sourceRow = findSourceRowByProfile(profile);
    if (sourceRow >= 0) {
        flags.enabled = sourceCheckAtRow(sourceRow, ColEnabled);
        flags.videos = sourceCheckAtRow(sourceRow, ColVideos);
        flags.stories = sourceCheckAtRow(sourceRow, ColStories);
        flags.highlights = sourceCheckAtRow(sourceRow, ColHighlights);
        flags.tagged = sourceCheckAtRow(sourceRow, ColTagged);
        flags.reels = sourceCheckAtRow(sourceRow, ColReels);
    }

    const QString scriptPath = instaloaderScriptPathFromSettings();
    if (scriptPath.isEmpty() || !QFileInfo::exists(scriptPath)) {
        log(tr("Instaloader script not found: %1").arg(scriptPath), QColor(Qt::red));
        failed++;
        selectedProfiles.removeFirst();
        processed++;
        progressBar->setValue(processed);
        scheduleSleep();
        return;
    }

    QString command = "python3";
    QStringList arguments;
    arguments << scriptPath << profile;
    arguments << instaloaderCommonArgsFromSettings();
    if (!flags.videos) arguments << "--no-videos";
    if (flags.stories) arguments << "--stories";
    if (flags.highlights) arguments << "--highlights";
    if (flags.tagged) arguments << "--tagged";
    if (flags.reels) arguments << "--reels";
    arguments << "--fast-update";

    process->setWorkingDirectory(workingDir);

    process->start(command, arguments);
    if (!process->waitForStarted()) {
        log(tr("Failed to start process for %1: %2").arg(profile).arg(process->errorString()), QColor(Qt::red));
        failed++;
        selectedProfiles.removeFirst();  // Remove failed
        processed++;
        progressBar->setValue(processed);
        scheduleSleep();
        return;
    }
}

void MainWindow::readProcessOutput()
{
    QString output = process->readAllStandardOutput();
    QStringList lines = output.split('\n');
    for (const QString& line : lines) {
        if (line.trimmed().isEmpty()) continue;

        // Parse [ curr/total ]
        QRegularExpression rx("\\[\\s*(\\d+)/(\\d+)\\s*\\]");
        QRegularExpressionMatch match = rx.match(line);
        if (match.hasMatch()) {
            int current = match.captured(1).toInt();
            int total = match.captured(2).toInt();
            profileProgressBar->setRange(0, total);
            profileProgressBar->setValue(current);
        }
    }

    log(output, QColor(Qt::darkGreen));
}

void MainWindow::readProcessError()
{
    QString error = process->readAllStandardError();
    bool requiresPause = false;
    const bool has404NotFound = error.contains("404 Not Found", Qt::CaseInsensitive);

    if (has404NotFound && !profileNotFoundError && !selectedProfiles.isEmpty()) {
        profileNotFoundError = true;
        disableSourceProfile(selectedProfiles[0]);
    }

    // Check for specific error messages that require pause
    if (error.contains("400 Bad Request") && error.contains("checkpoint_required")) {
        requiresPause = true;
        checkpointError = true;  // Set flag to prevent adding to skipped.txt
    }
    if (error.contains("Download aborted: Query to https://www.instagram.com/graphql/query responded with \"400 Bad Request\": {\"message\":\"feedback_required\",\"spam\":true,\"feedback_title\":\"Try Again Later\",\"feedback_message\":\"We limit how often you can do certain things on Instagram to protect our community. Tell us if you think we made a mistake.\",\"feedback_url\":\"\",\"feedback_action\":\"report_problem\",\"feedback_appeal_label\":\"Let us know\",\"restriction_enrollment_data\":\"\",\"status\":\"fail\"}."))
    {
        requiresPause = true;
        checkpointError = true;
    }
    // TODO: Add other error messages that require pause here
    // Example: if (error.contains("another error string")) { requiresPause = true; checkpointError = true; }

    if (requiresPause) {
        if (!paused) {
            togglePause();  // Automatically pause if not already paused
        }
        QMessageBox::warning(this, tr("User Action Required"), tr("Instagram requires user action (checkpoint). Please check the logs and resolve manually."));
    }

    log(error, QColor(Qt::red));
}

void MainWindow::processFinished(int exitCode, QProcess::ExitStatus exitStatus)
{
    Q_UNUSED(exitStatus);
    if (selectedProfiles.isEmpty()) {
        return;
    }
    QString profile = selectedProfiles[0];  // Always first

    bool isSuccess = (exitCode == 0);
    if (isSuccess) {
        log(tr("Profile %1 downloaded successfully.").arg(profile), QColor(Qt::green));
        updateLastUpdatedForProfile(profile, QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss"));
        success++;
    } else {
        log(tr("Profile %1 download failed with code %2.").arg(profile).arg(exitCode), QColor(Qt::red));
        failed++;

        // Append to skipped.txt only if not a checkpoint error
        if (!checkpointError) {
            QString skippedPath = QDir(workingDir).filePath("skipped.txt");
            QSet<QString> skippedSet;
            QFile skippedFile(skippedPath);
            if (skippedFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
                QTextStream in(&skippedFile);
                while (!in.atEnd()) {
                    skippedSet.insert(in.readLine().trimmed().toLower());
                }
                skippedFile.close();
            }

            if (!skippedSet.contains(profile.toLower())) {
                if (skippedFile.open(QIODevice::Append | QIODevice::Text)) {
                    QTextStream out(&skippedFile);
                    out << profile << "\n";
                    skippedFile.close();
                }
            }
        }
    }

    // Remove processed from selected
    selectedProfiles.removeFirst();

    // Update queueProfiles to remaining selected (since shuffled, but we remove the processed one)
    queueProfiles.removeOne(profile);
    queueList->clear();
    for (const auto& p : queueProfiles) {
        queueList->addItem(p);
    }
    queueList->sortItems(Qt::AscendingOrder);

    if (isSuccess && shouldCountVideoSizeAutomatically()) {
        recalculateVideoSizeForProfile(profile, false, false);
    }
    if (isSuccess) {
        saveProfilesDatabase();
    }

    profileProgressBar->reset();  // Reset after profile done

    processed++;
    progressBar->setValue(processed);

    saveResumeFile();

    scheduleSleep();
}

void MainWindow::scheduleSleep()
{
    if (canceled || selectedProfiles.isEmpty()) {
        processNext();
        return;
    }

    int sleepTime = QRandomGenerator::global()->bounded(3, 16);
    log(tr("Sleeping for %1 seconds before next profile.").arg(sleepTime), QColor(Qt::yellow));
    sleepTimer->start(sleepTime * 1000);
}

void MainWindow::saveResumeFile()
{
    QString resumePath = QDir(workingDir).filePath("resume.txt");
    QFile resumeFile(resumePath);
    if (!resumeFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
        log(tr("Error saving resume.txt"), QColor(Qt::red));
        return;
    }

    QTextStream out(&resumeFile);
    for (const auto& prof : queueProfiles) {  // Save current queue
        out << prof << "\n";
    }
    resumeFile.close();
}

void MainWindow::finalizeProcessing()
{
    if (!canceled) {
        QString resumePath = QDir(workingDir).filePath("resume.txt");
        QFile::remove(resumePath);
        queueList->clear();
        queueProfiles.clear();
    } else {
        // Update queue to remaining
        queueProfiles = selectedProfiles;
        queueList->clear();
        for (const auto& p : queueProfiles) {
            queueList->addItem(p);
        }
        queueList->sortItems(Qt::AscendingOrder);
    }
    log(tr("Summary: Success: %1, Failed: %2, Processed: %3").arg(success).arg(failed).arg(processed));
    if (startButton) { startButton->setEnabled(true); startButton->setText(tr("Start")); }
    if (pauseButton) pauseButton->setText(tr("Pause"));
    paused = false;
    profileProgressBar->reset();
    isProcessing = false;
}

void MainWindow::log(const QString &message, const QColor &color)
{
    logTextEdit->setTextColor(color);
    logTextEdit->append(message);
    logTextEdit->setTextColor(Qt::cyan);
}


QString MainWindow::tsResourceForLanguage(const QString &effectiveCode) const
{
    return QString(":/i18n/InstagramTasker_%1.ts").arg(effectiveCode.toLower());
}

QString MainWindow::detectSystemLanguage() const
{
    const QStringList uiLangs = QLocale::system().uiLanguages(); // e.g. "de-DE"
    for (const QString &l : uiLangs) {
        if (l.isEmpty()) continue;
        const QString base = l.left(2).toLower();
        if (QFile::exists(tsResourceForLanguage(base))) {
            return base;
        }
    }
    return "en";
}

QString MainWindow::resolveEffectiveLanguage(const QString &settingCode) const
{
    if (settingCode.isEmpty()) return "en";

    if (settingCode == "system") {
        return detectSystemLanguage();
    }

    const QString base = settingCode.left(2).toLower();
    if (base == "en") return "en";

    if (QFile::exists(tsResourceForLanguage(base))) {
        return base;
    }

    return "en";
}

bool MainWindow::installLanguageInternal(const QString &effectiveCode)
{
    if (translatorInstalled) {
        qApp->removeTranslator(&translator);
        translatorInstalled = false;
    }
    translator.clear();

    const QString code = effectiveCode.toLower();
    if (code == "en" || code.isEmpty()) {
        return true; // English: no translator
    }

    const QString resPath = tsResourceForLanguage(code);
    if (!translator.loadFromTsResource(resPath)) {
        return false;
    }

    qApp->installTranslator(&translator);
    translatorInstalled = true;
    return true;
}
