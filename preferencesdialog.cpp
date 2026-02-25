#include "preferencesdialog.h"


#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QDialogButtonBox>
#include <QAbstractButton>
#include <QSizePolicy>

PreferencesDialog::PreferencesDialog(QWidget *parent)
    : QDialog(parent),
      tabs(nullptr),
      generalTab(nullptr),
      defaultBehaviourTab(nullptr),
      environmentTab(nullptr),
      workingDirLabel(nullptr),
      instaloaderPathLabel(nullptr),
      pythonExecutableLabel(nullptr),
      fileManagerLabel(nullptr),
      galleryViewerLabel(nullptr),
      languageLabel(nullptr),
      loginLabel(nullptr),
      userAgentLabel(nullptr),
      abortOnLabel(nullptr),
      batchQueueLimitLabel(nullptr),
      workingDirEdit(nullptr),
      instaloaderPathEdit(nullptr),
      instaloaderBrowseButton(nullptr),
      pythonExecutableEdit(nullptr),
      fileManagerEdit(nullptr),
      galleryViewerEdit(nullptr),
      pythonBrowseButton(nullptr),
      fileManagerBrowseButton(nullptr),
      galleryViewerBrowseButton(nullptr),
      loginEdit(nullptr),
      userAgentEdit(nullptr),
      browseButton(nullptr),
      languageCombo(nullptr),
      skipMetadataCheck(nullptr),
      skipCaptionsCheck(nullptr),
      noIphoneCheck(nullptr),
      abort400Check(nullptr),
      abort401Check(nullptr),
      abort403Check(nullptr),
      abort404Check(nullptr),
      abort429Check(nullptr),
      defaultEnabledCheck(nullptr),
      defaultVideosCheck(nullptr),
      defaultStoriesCheck(nullptr),
      defaultHighlightsCheck(nullptr),
      defaultTaggedCheck(nullptr),
      defaultReelsCheck(nullptr),
      batchQueueLimitSpin(nullptr),
      buttonBox(nullptr),
      currentLanguageCode("en")
{
    setModal(true);
    resize(740, 520);

    tabs = new QTabWidget(this);
    generalTab = new QWidget(this);
    environmentTab = new QWidget(this);
    defaultBehaviourTab = new QWidget(this);

    workingDirLabel = new QLabel(this);
    instaloaderPathLabel = new QLabel(this);
    pythonExecutableLabel = new QLabel(this);
    fileManagerLabel = new QLabel(this);
    galleryViewerLabel = new QLabel(this);
    languageLabel = new QLabel(this);
    loginLabel = new QLabel(this);
    userAgentLabel = new QLabel(this);
    abortOnLabel = new QLabel(this);
    batchQueueLimitLabel = new QLabel(this);

    workingDirEdit = new QLineEdit(this);
    workingDirEdit->setReadOnly(true);

    instaloaderPathEdit = new QLineEdit(this);
    instaloaderBrowseButton = new QPushButton(this);
    pythonExecutableEdit = new QLineEdit(this);
    fileManagerEdit = new QLineEdit(this);
    galleryViewerEdit = new QLineEdit(this);
    pythonBrowseButton = new QPushButton(this);
    fileManagerBrowseButton = new QPushButton(this);
    galleryViewerBrowseButton = new QPushButton(this);

    loginEdit = new QLineEdit(this);
    userAgentEdit = new QPlainTextEdit(this);
    userAgentEdit->setMaximumHeight(90);

    browseButton = new QPushButton(this);
    connect(browseButton, &QPushButton::clicked, this, &PreferencesDialog::onBrowseClicked);
    connect(instaloaderBrowseButton, &QPushButton::clicked, this, &PreferencesDialog::onInstaloaderBrowseClicked);
    connect(pythonBrowseButton, &QPushButton::clicked, this, &PreferencesDialog::onPythonBrowseClicked);
    connect(fileManagerBrowseButton, &QPushButton::clicked, this, &PreferencesDialog::onFileManagerBrowseClicked);
    connect(galleryViewerBrowseButton, &QPushButton::clicked, this, &PreferencesDialog::onGalleryViewerBrowseClicked);

    languageCombo = new QComboBox(this);

    // System default (follows OS language)
    languageCombo->addItem(tr("System default"), QStringLiteral("system"));

    // Language names should be shown in their native form (not translated).
    struct LangItem { const char* code; const char* name; };
    const LangItem langs[] = {
        {"en","English"},
        {"ru","Русский"},
        {"de","Deutsch"},
        {"es","Español"},
        {"fr","Français"},
        {"nl","Nederlands"},
        {"it","Italiano"},
        {"pt","Português"},
        {"pl","Polski"},
        {"cs","Čeština"},
        {"sk","Slovenčina"},
        {"hu","Magyar"},
        {"ro","Română"},
        {"sv","Svenska"},
        {"no","Norsk"},
        {"da","Dansk"},
        {"fi","Suomi"},
        {"el","Ελληνικά"}
    };
    for (const auto &l : langs) {
        languageCombo->addItem(QString::fromUtf8(l.name), QString::fromLatin1(l.code));
    }

    QWidget *dirWidget = new QWidget(this);
    QHBoxLayout *dirRow = new QHBoxLayout(dirWidget);
    dirRow->setContentsMargins(0, 0, 0, 0);
    dirRow->addWidget(workingDirEdit);
    dirRow->addWidget(browseButton);

    QWidget *instaloaderWidget = new QWidget(this);
    QHBoxLayout *instaloaderRow = new QHBoxLayout(instaloaderWidget);
    instaloaderRow->setContentsMargins(0, 0, 0, 0);
    instaloaderRow->addWidget(instaloaderPathEdit);
    instaloaderRow->addWidget(instaloaderBrowseButton);

    skipMetadataCheck = new QCheckBox(this);
    skipCaptionsCheck = new QCheckBox(this);
    noIphoneCheck = new QCheckBox(this);
    abort400Check = new QCheckBox(this);
    abort401Check = new QCheckBox(this);
    abort403Check = new QCheckBox(this);
    abort404Check = new QCheckBox(this);
    abort429Check = new QCheckBox(this);
    batchQueueLimitSpin = new QSpinBox(this);
    batchQueueLimitSpin->setRange(1, 100000);
    batchQueueLimitSpin->setValue(50);

    QWidget *abortWidget = new QWidget(this);
    QHBoxLayout *abortLayout = new QHBoxLayout(abortWidget);
    abortLayout->setContentsMargins(0, 0, 0, 0);
    abortLayout->addWidget(abort400Check);
    abortLayout->addWidget(abort401Check);
    abortLayout->addWidget(abort403Check);
    abortLayout->addWidget(abort404Check);
    abortLayout->addWidget(abort429Check);
    abortLayout->addStretch();

    QFormLayout *form = new QFormLayout();
    form->addRow(workingDirLabel, dirWidget);
    form->addRow(instaloaderPathLabel, instaloaderWidget);
    form->addRow(languageLabel, languageCombo);
    form->addRow(loginLabel, loginEdit);
    form->addRow(skipMetadataCheck);
    form->addRow(skipCaptionsCheck);
    form->addRow(userAgentLabel, userAgentEdit);
    form->addRow(noIphoneCheck);
    form->addRow(abortOnLabel, abortWidget);
    form->addRow(batchQueueLimitLabel, batchQueueLimitSpin);

    generalTab->setLayout(form);
    tabs->addTab(generalTab, "");

    QWidget *pythonWidget = new QWidget(this);
    QHBoxLayout *pythonRow = new QHBoxLayout(pythonWidget);
    pythonRow->setContentsMargins(0, 0, 0, 0);
    pythonRow->addWidget(pythonExecutableEdit);
    pythonRow->addWidget(pythonBrowseButton);

    QWidget *fileManagerWidget = new QWidget(this);
    QHBoxLayout *fileManagerRow = new QHBoxLayout(fileManagerWidget);
    fileManagerRow->setContentsMargins(0, 0, 0, 0);
    fileManagerRow->addWidget(fileManagerEdit);
    fileManagerRow->addWidget(fileManagerBrowseButton);

    QWidget *galleryWidget = new QWidget(this);
    QHBoxLayout *galleryRow = new QHBoxLayout(galleryWidget);
    galleryRow->setContentsMargins(0, 0, 0, 0);
    galleryRow->addWidget(galleryViewerEdit);
    galleryRow->addWidget(galleryViewerBrowseButton);

    QFormLayout *environmentForm = new QFormLayout();
    environmentForm->addRow(pythonExecutableLabel, pythonWidget);
    environmentForm->addRow(fileManagerLabel, fileManagerWidget);
    environmentForm->addRow(galleryViewerLabel, galleryWidget);
    environmentForm->addItem(new QSpacerItem(0, 0, QSizePolicy::Minimum, QSizePolicy::Expanding));

    environmentTab->setLayout(environmentForm);
    tabs->addTab(environmentTab, "");

    defaultEnabledCheck = new QCheckBox(this);
    defaultVideosCheck = new QCheckBox(this);
    defaultStoriesCheck = new QCheckBox(this);
    defaultHighlightsCheck = new QCheckBox(this);
    defaultTaggedCheck = new QCheckBox(this);
    defaultReelsCheck = new QCheckBox(this);

    QVBoxLayout *defaultsLayout = new QVBoxLayout();
    defaultsLayout->addWidget(defaultEnabledCheck);
    defaultsLayout->addWidget(defaultVideosCheck);
    defaultsLayout->addWidget(defaultStoriesCheck);
    defaultsLayout->addWidget(defaultHighlightsCheck);
    defaultsLayout->addWidget(defaultTaggedCheck);
    defaultsLayout->addWidget(defaultReelsCheck);
    defaultsLayout->addStretch();

    defaultBehaviourTab->setLayout(defaultsLayout);
    tabs->addTab(defaultBehaviourTab, "");

    buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
    connect(buttonBox, &QDialogButtonBox::accepted, this, &PreferencesDialog::onAccepted);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(tabs);
    mainLayout->addWidget(buttonBox);
    setLayout(mainLayout);

    updateTexts();
}

void PreferencesDialog::changeEvent(QEvent *event)
{
    if (event && event->type() == QEvent::LanguageChange) {
        updateTexts();
    }
    QDialog::changeEvent(event);
}


void PreferencesDialog::updateTexts()
{
    setWindowTitle(tr("Preferences"));
    tabs->setTabText(0, tr("General"));
    tabs->setTabText(1, tr("Environment parameters"));
    tabs->setTabText(2, tr("Default behaviour"));
    workingDirLabel->setText(tr("Working directory:"));
    instaloaderPathLabel->setText(tr("Instaloader script:"));
    pythonExecutableLabel->setText(tr("Python executable:"));
    fileManagerLabel->setText(tr("File manager:"));
    galleryViewerLabel->setText(tr("Gallery viewer:"));
    browseButton->setText(tr("Browse..."));
    instaloaderBrowseButton->setText(tr("Browse..."));
    pythonBrowseButton->setText(tr("Browse..."));
    fileManagerBrowseButton->setText(tr("Browse..."));
    galleryViewerBrowseButton->setText(tr("Browse..."));
    languageLabel->setText(tr("Language:"));
    loginLabel->setText(tr("Login:"));
    userAgentLabel->setText(tr("User-agent:"));
    abortOnLabel->setText(tr("Abort on:"));
    batchQueueLimitLabel->setText(tr("Batch queue limit:"));
    skipMetadataCheck->setText(tr("Skip metadata json (--no-metadata-json)"));
    skipCaptionsCheck->setText(tr("Skip captions (--no-captions)"));
    noIphoneCheck->setText(tr("No iPhone mode (--no-iphone)"));
    abort400Check->setText(QStringLiteral("400"));
    abort401Check->setText(QStringLiteral("401"));
    abort403Check->setText(QStringLiteral("403"));
    abort404Check->setText(QStringLiteral("404"));
    abort429Check->setText(QStringLiteral("429"));
    defaultEnabledCheck->setText(tr("Enabled"));
    defaultVideosCheck->setText(tr("Video"));
    defaultStoriesCheck->setText(tr("Stories"));
    defaultHighlightsCheck->setText(tr("Highlights"));
    defaultTaggedCheck->setText(tr("Tagged"));
    defaultReelsCheck->setText(tr("Reels"));

    if (buttonBox) {
        if (auto *ok = buttonBox->button(QDialogButtonBox::Ok)) {
            ok->setText(tr("OK"));
        }
        if (auto *cancel = buttonBox->button(QDialogButtonBox::Cancel)) {
            cancel->setText(tr("Cancel"));
        }
    }
}

void PreferencesDialog::setWorkingDirectory(const QString &dir)
{
    workingDirEdit->setText(dir);
}

QString PreferencesDialog::workingDirectory() const
{
    return workingDirEdit->text();
}

void PreferencesDialog::setInstaloaderPath(const QString &path)
{
    instaloaderPathEdit->setText(path);
}

QString PreferencesDialog::instaloaderPath() const
{
    return instaloaderPathEdit->text().trimmed();
}

void PreferencesDialog::setEnvironmentCommands(const QString &pythonExecutable,
                                               const QString &fileManager,
                                               const QString &galleryViewer)
{
    pythonExecutableEdit->setText(pythonExecutable);
    fileManagerEdit->setText(fileManager);
    galleryViewerEdit->setText(galleryViewer);
}

QString PreferencesDialog::pythonExecutable() const
{
    return pythonExecutableEdit->text().trimmed();
}

QString PreferencesDialog::fileManagerCommand() const
{
    return fileManagerEdit->text().trimmed();
}

QString PreferencesDialog::galleryViewerCommand() const
{
    return galleryViewerEdit->text().trimmed();
}

void PreferencesDialog::setLanguageCode(const QString &code)
{
    int idx = languageCombo->findData(code);
    if (idx >= 0) {
        languageCombo->setCurrentIndex(idx);
        currentLanguageCode = code;
    } else if (!code.isEmpty()) {
        currentLanguageCode = code;
    }
    updateTexts();
}

QString PreferencesDialog::languageCode() const
{
    return languageCombo->currentData().toString();
}

void PreferencesDialog::setDownloaderOptions(const QString &login,
                                             bool noMetadata,
                                             bool noCaptions,
                                             const QString &userAgent,
                                             bool noIphone,
                                             bool abort400,
                                             bool abort401,
                                             bool abort403,
                                             bool abort404,
                                             bool abort429)
{
    loginEdit->setText(login);
    skipMetadataCheck->setChecked(noMetadata);
    skipCaptionsCheck->setChecked(noCaptions);
    userAgentEdit->setPlainText(userAgent);
    noIphoneCheck->setChecked(noIphone);
    abort400Check->setChecked(abort400);
    abort401Check->setChecked(abort401);
    abort403Check->setChecked(abort403);
    abort404Check->setChecked(abort404);
    abort429Check->setChecked(abort429);
}

void PreferencesDialog::setDefaultBehaviour(bool enabled,
                                            bool videos,
                                            bool stories,
                                            bool highlights,
                                            bool tagged,
                                            bool reels)
{
    defaultEnabledCheck->setChecked(enabled);
    defaultVideosCheck->setChecked(videos);
    defaultStoriesCheck->setChecked(stories);
    defaultHighlightsCheck->setChecked(highlights);
    defaultTaggedCheck->setChecked(tagged);
    defaultReelsCheck->setChecked(reels);
}

void PreferencesDialog::setBatchQueueLimit(int limit)
{
    if (!batchQueueLimitSpin) return;
    const int clamped = (limit < batchQueueLimitSpin->minimum()) ? batchQueueLimitSpin->minimum() : limit;
    batchQueueLimitSpin->setValue(clamped);
}

int PreferencesDialog::batchQueueLimit() const
{
    return batchQueueLimitSpin ? batchQueueLimitSpin->value() : 50;
}

void PreferencesDialog::onBrowseClicked()
{
    emit workingDirectoryBrowseRequested();
}

void PreferencesDialog::onInstaloaderBrowseClicked()
{
    emit instaloaderBrowseRequested();
}

void PreferencesDialog::onPythonBrowseClicked()
{
    emit pythonBrowseRequested();
}

void PreferencesDialog::onFileManagerBrowseClicked()
{
    emit fileManagerBrowseRequested();
}

void PreferencesDialog::onGalleryViewerBrowseClicked()
{
    emit galleryViewerBrowseRequested();
}

void PreferencesDialog::onAccepted()
{
    emit preferencesAccepted(workingDirectory(),
                             instaloaderPath(),
                             pythonExecutable(),
                             fileManagerCommand(),
                             galleryViewerCommand(),
                             languageCode(),
                             loginEdit->text().trimmed(),
                             skipMetadataCheck->isChecked(),
                             skipCaptionsCheck->isChecked(),
                             userAgentEdit->toPlainText().trimmed(),
                             noIphoneCheck->isChecked(),
                             abort400Check->isChecked(),
                             abort401Check->isChecked(),
                             abort403Check->isChecked(),
                             abort404Check->isChecked(),
                             abort429Check->isChecked(),
                             defaultEnabledCheck->isChecked(),
                             defaultVideosCheck->isChecked(),
                             defaultStoriesCheck->isChecked(),
                             defaultHighlightsCheck->isChecked(),
                             defaultTaggedCheck->isChecked(),
                             defaultReelsCheck->isChecked(),
                             batchQueueLimit());
    accept();
}
