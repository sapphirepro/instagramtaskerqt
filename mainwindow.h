#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTableWidget>
#include <QListWidget>
#include <QPushButton>
#include <QTextEdit>
#include <QProgressBar>
#include <QSystemTrayIcon>
#include <QMenu>
#include <QAction>
#include <QProcess>
#include <QTimer>
#include <QVector>
#include <QString>
#include <QSettings>
#include <QLabel>
#include <QCheckBox>
#include <QLineEdit>
#include <QSplitter>
#include <QFutureWatcher>
#include <QPair>

#include <QEvent>
#include "ts_translator.h"

class PreferencesDialog;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

/**
 * @brief Main application window that manages source profiles, queue, and execution flow.
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    /**
     * @brief Constructs the main window and initializes persistent state.
     * @param parent Parent widget that owns the window.
     */
    explicit MainWindow(QWidget *parent = nullptr);
    /**
     * @brief Destroys the main window instance.
     */
    ~MainWindow() override;

protected:
    /**
     * @brief Handles window close requests and persists UI state.
     * @param event Close event received from Qt.
     */
    void closeEvent(QCloseEvent *event) override;
    /**
     * @brief Reacts to language/theme related change events.
     * @param event Event payload provided by Qt.
     */
    void changeEvent(QEvent *event) override;

private slots:
    /**
     * @brief Processes tray icon activation actions.
     * @param reason Activation reason sent by Qt.
     */
    void trayActivated(QSystemTrayIcon::ActivationReason reason);
    /**
     * @brief Opens file dialog and loads profiles from a text file.
     */
    void loadProfiles();
    /**
     * @brief Loads profiles from a specific text file path.
     * @param fileName Absolute or relative text file path.
     */
    void loadProfiles(const QString &fileName);
    /**
     * @brief Opens file dialog and loads profiles from a SQLite database.
     */
    void loadProfilesFromDatabase();
    /**
     * @brief Saves current profiles into a new database file selected by user.
     */
    void saveProfilesDatabaseAs();
    /**
     * @brief Creates and switches to a new plain-text profiles list.
     */
    void newProfilesList();
    /**
     * @brief Creates and switches to a new SQLite profiles database.
     */
    void newProfilesDatabase();
    /**
     * @brief Opens preferences dialog.
     */
    void openPreferences();
    /**
     * @brief Applies a new UI language selected by the user.
     * @param langCode Language identifier.
     */
    void applyLanguage(const QString &langCode);
    /**
     * @brief Restores queue state from resume file when present.
     */
    void checkResumeFile();
    /**
     * @brief Starts processing of queued profiles.
     */
    void startProcessing();
    /**
     * @brief Pauses or resumes active processing.
     */
    void togglePause();
    /**
     * @brief Cancels current processing session.
     */
    void cancelProcessing();
    /**
     * @brief Starts processing of the next queued profile.
     */
    void processNext();
    /**
     * @brief Reads standard output from running downloader process.
     */
    void readProcessOutput();
    /**
     * @brief Reads standard error from running downloader process.
     */
    void readProcessError();
    /**
     * @brief Handles completion of downloader subprocess.
     * @param exitCode Process exit code.
     * @param exitStatus Exit status enum value from Qt.
     */
    void processFinished(int exitCode, QProcess::ExitStatus exitStatus);
    /**
     * @brief Schedules delay between processing tasks.
     */
    void scheduleSleep();
    /**
     * @brief Persists current queue state into resume file.
     */
    void saveResumeFile();
    /**
     * @brief Finalizes processing session and updates UI.
     */
    void finalizeProcessing();
    /**
     * @brief Appends a message to the log view.
     * @param message Message text.
     * @param color Message color.
     */
    void log(const QString &message, const QColor &color = QColor(Qt::cyan));
    /**
     * @brief Adds profiles typed in the input box into source/queue lists.
     */
    void addNewProfiles();
    /**
     * @brief Adds selected source profiles to processing queue.
     */
    void addToQueue();
    /**
     * @brief Clears current queue and related runtime state.
     */
    void clearQueue();
    /**
     * @brief Removes profiles that are missing on disk.
     */
    void removeMissingProfiles();
    /**
     * @brief Shows source table row context menu.
     * @param pos Position in table coordinates.
     */
    void showSourceContextMenu(const QPoint &pos);
    /**
     * @brief Shows source table header context menu.
     * @param pos Position in header coordinates.
     */
    void showSourceHeaderContextMenu(const QPoint &pos);
    /**
     * @brief Shows context menu for main log widget.
     * @param pos Position in widget coordinates.
     */
    void showLogContextMenu(const QPoint &pos);
    /**
     * @brief Shows context menu for new-profiles log widget.
     * @param pos Position in widget coordinates.
     */
    void showNewProfilesContextMenu(const QPoint &pos);
    /**
     * @brief Handles changes in source table cells.
     * @param item Changed table item.
     */
    void onSourceTableItemChanged(QTableWidgetItem *item);
    /**
     * @brief Filters source table rows by profile name.
     * @param text Filter text.
     */
    void onSourceSearchTextChanged(const QString &text);
    /**
     * @brief Stores automatic video-size option state.
     * @param enabled True to enable auto calculation.
     */
    void onCountVideoSizeToggled(bool enabled);
    /**
     * @brief Recalculates video size values for all source profiles.
     */
    void recalculateVideoSizes();
    /**
     * @brief Shows "About" dialog with project metadata.
     */
    void showAboutDialog();

private:
    /**
     * @brief Source table column identifiers.
     */
    enum SourceColumn {
        ColEnabled = 0,
        ColProfile,
        ColRating,
        ColVideos,
        ColStories,
        ColHighlights,
        ColTagged,
        ColReels,
        ColLastUpdated,
        ColVideosSize,
        ColCount
    };

    /**
     * @brief Default checkbox state set applied to new profiles.
     */
    struct ProfileDefaults {
        bool enabled = true;
        bool videos = true;
        bool stories = true;
        bool highlights = true;
        bool tagged = true;
        bool reels = true;
    };

    /**
     * @brief Updates source profiles counter label.
     */
    void updateSourceCounter();
    /**
     * @brief Re-translates and refreshes all UI labels.
     */
    void updateUiTexts();
    /**
     * @brief Updates queue counter label.
     */
    void updateQueueCounter();
    /**
     * @brief Saves source profiles into the active text file.
     */
    void saveProfilesFile();
    /**
     * @brief Saves source profiles into the active database.
     * @return True on successful write or when database mode is disabled.
     */
    bool saveProfilesDatabase();
    /**
     * @brief Loads source profiles from database file.
     * @param fileName Database file path.
     * @return True when loading succeeds.
     */
    bool loadProfilesFromDatabase(const QString &fileName);
    /**
     * @brief Ensures required database schema exists and runs lightweight migrations.
     * @param fileName Database file path.
     * @return True when schema is ready for use.
     */
    bool ensureProfilesDatabaseSchema(const QString &fileName);
    /**
     * @brief Removes all rows from source table.
     */
    void clearSourceTable();
    /**
     * @brief Appends one profile row to source table.
     * @param profile Profile name.
     * @param enabled Enabled state.
     * @param videos Download videos flag.
     * @param stories Download stories flag.
     * @param highlights Download highlights flag.
     * @param tagged Download tagged posts flag.
     * @param reels Download reels flag.
     * @param rating Star rating in range 0..5.
     * @param lastUpdated Last successful update timestamp.
     * @param videosSize Cached video size text.
     */
    void addSourceRow(const QString &profile,
                      bool enabled,
                      bool videos,
                      bool stories,
                      bool highlights,
                      bool tagged,
                      bool reels,
                      int rating = 0,
                      const QString &lastUpdated = QString(""),
                      const QString &videosSize = QString(""));
    /**
     * @brief Creates a checkable table item.
     * @param checked Initial check state.
     * @return Newly allocated table widget item.
     */
    QTableWidgetItem *makeCheckItem(bool checked) const;
    /**
     * @brief Creates a read-only text table item.
     * @param text Initial text.
     * @return Newly allocated table widget item.
     */
    QTableWidgetItem *makeTextItem(const QString &text) const;
    /**
     * @brief Creates a sortable data-size table item.
     * @param text Human-readable size text.
     * @return Newly allocated table widget item.
     */
    QTableWidgetItem *makeSizeItem(const QString &text) const;
    /**
     * @brief Creates a sortable rating table item.
     * @param rating Rating value in range 0..5.
     * @return Newly allocated table widget item.
     */
    QTableWidgetItem *makeRatingItem(int rating) const;
    /**
     * @brief Returns profile name by source table row index.
     * @param row Source table row index.
     * @return Profile name or empty string on invalid row.
     */
    QString sourceProfileAtRow(int row) const;
    /**
     * @brief Returns checkbox state for a table cell.
     * @param row Table row index.
     * @param column Table column index.
     * @return True when checked.
     */
    bool sourceCheckAtRow(int row, int column) const;
    /**
     * @brief Returns rating value for a source row.
     * @param row Source table row index.
     * @return Rating in range 0..5.
     */
    int sourceRatingAtRow(int row) const;
    /**
     * @brief Finds source row by case-insensitive profile name.
     * @param profile Profile name to search for.
     * @return Row index or -1 if not found.
     */
    int findSourceRowByProfile(const QString &profile) const;
    /**
     * @brief Removes all matching profile entries from queue.
     * @param profile Profile name.
     */
    void removeProfileFromQueue(const QString &profile);
    /**
     * @brief Checks whether profile is enabled in source table.
     * @param profile Profile name.
     * @return True when profile exists and has enabled state.
     */
    bool isSourceProfileEnabled(const QString &profile) const;
    /**
     * @brief Marks source profile as disabled and removes it from queue.
     * @param profile Profile name.
     */
    void disableSourceProfile(const QString &profile);
    /**
     * @brief Removes a row from source table and persists updates.
     * @param row Row index to remove.
     */
    void removeSourceRow(int row);
    /**
     * @brief Deletes profile directory recursively.
     * @param profile Profile name.
     * @return True when deletion succeeds.
     */
    bool removeProfileFolder(const QString &profile);
    /**
     * @brief Deletes all known video files from profile directory.
     * @param profile Profile name.
     * @return Number of deleted files or -1 on failure.
     */
    int clearVideoFilesForProfile(const QString &profile);
    /**
     * @brief Calculates total size of known video files in bytes.
     * @param profile Profile name.
     * @return Video size in bytes.
     */
    qint64 calculateVideoSizeBytesForProfile(const QString &profile) const;
    /**
     * @brief Formats byte value as human-readable binary size.
     * @param bytes Size in bytes.
     * @return Formatted size string.
     */
    QString formatDataSizeBinary(qint64 bytes) const;
    /**
     * @brief Recalculates and stores video size for a profile.
     * @param profile Profile name.
     * @param persistNow True to immediately persist data.
     * @param writeLog True to write log entry.
     * @return True when profile was found and updated.
     */
    bool recalculateVideoSizeForProfile(const QString &profile, bool persistNow, bool writeLog);
    /**
     * @brief Applies computed video size bytes to source table row.
     * @param profile Profile name.
     * @param bytes Computed size in bytes.
     * @return True when row was found and updated.
     */
    bool applyVideoSizeBytesToProfile(const QString &profile, qint64 bytes);
    /**
     * @brief Updates "last updated" value for profile row.
     * @param profile Profile name.
     * @param value Timestamp text value.
     */
    void updateLastUpdatedForProfile(const QString &profile, const QString &value);
    /**
     * @brief Opens options dialog for a newly discovered profile.
     * @param profile Profile name.
     * @param outDefaults Output structure with selected defaults.
     * @return True when user accepts dialog.
     */
    bool promptNewProfileOptions(const QString &profile, ProfileDefaults *outDefaults);
    /**
     * @brief Reads profile default flags from settings.
     * @return Profile default flags.
     */
    ProfileDefaults profileDefaultsFromSettings() const;
    /**
     * @brief Persists profile default flags to settings.
     * @param defaults Default flags to persist.
     */
    void applyProfileDefaultsToSettings(const ProfileDefaults &defaults);
    /**
     * @brief Highlights invalid profile names entered in add box.
     * @param invalid List of invalid profile tokens.
     */
    void highlightInvalidAddProfiles(const QStringList &invalid);
    /**
     * @brief Adds profiles to queue filtered by rating range.
     * @param minRating Inclusive minimum rating.
     * @param maxRating Inclusive maximum rating.
     */
    void addProfilesByRatingRange(int minRating, int maxRating);
    /**
     * @brief Adds oldest profiles to queue using configured batch limit.
     */
    void addOldestProfilesToQueue();
    /**
     * @brief Removes disabled source profiles from current queue.
     */
    void removeDisabledProfilesFromQueue();
    /**
     * @brief Returns path to Instaloader script from settings.
     * @return Script path.
     */
    QString instaloaderScriptPathFromSettings() const;
    /**
     * @brief Returns Python executable command from settings.
     * @return Python executable command.
     */
    QString pythonExecutableFromSettings() const;
    /**
     * @brief Returns file manager command from settings.
     * @return File manager command.
     */
    QString fileManagerCommandFromSettings() const;
    /**
     * @brief Returns gallery viewer command from settings.
     * @return Gallery viewer command.
     */
    QString galleryViewerCommandFromSettings() const;
    /**
     * @brief Builds common Instaloader CLI arguments from settings.
     * @return Ordered argument list.
     */
    QStringList instaloaderCommonArgsFromSettings() const;
    /**
     * @brief Returns configured queue batch limit.
     * @return Batch limit value.
     */
    int batchQueueLimitFromSettings() const;
    /**
     * @brief Checks whether automatic video-size calculation is enabled.
     * @return True when enabled.
     */
    bool shouldCountVideoSizeAutomatically() const;

    /**
     * @brief Detects system language code supported by the app.
     * @return Language code.
     */
    QString detectSystemLanguage() const;
    /**
     * @brief Resolves effective language code from settings value.
     * @param settingCode Stored language setting.
     * @return Effective language code.
     */
    QString resolveEffectiveLanguage(const QString &settingCode) const;
    /**
     * @brief Installs translator for selected language.
     * @param effectiveCode Effective language code.
     * @return True when translator was installed.
     */
    bool installLanguageInternal(const QString &effectiveCode);
    /**
     * @brief Maps language code to TS resource path.
     * @param effectiveCode Effective language code.
     * @return Qt resource path for translation file.
     */
    QString tsResourceForLanguage(const QString &effectiveCode) const;

    QTableWidget *sourceTable = nullptr;
    QListWidget *queueList = nullptr;
    QTextEdit *logTextEdit = nullptr;
    QTextEdit *newProfilesLog = nullptr;
    QLabel *logLabel = nullptr;
    QLabel *newProfilesLabel = nullptr;
    QLabel *addNewProfilesLabel = nullptr;
    QProgressBar *progressBar = nullptr;
    QProgressBar *profileProgressBar = nullptr;
    QSplitter *mainSplitter = nullptr;
    QSplitter *profilesAreaSplitter = nullptr;
    QSplitter *queueAddSplitter = nullptr;
    QPushButton *startButton = nullptr;
    QPushButton *pauseButton = nullptr;
    QPushButton *cancelButton = nullptr;
    QPushButton *addButton = nullptr;
    QPushButton *addToQueueButton = nullptr;
    QPushButton *clearQueueButton = nullptr;
    QPushButton *removeMissingButton = nullptr;
    QTextEdit *addProfilesEdit = nullptr;
    QLabel *sourceCountLabel = nullptr;
    QLabel *sourceSearchLabel = nullptr;
    QLineEdit *sourceSearchEdit = nullptr;
    QLabel *queueCountLabel = nullptr;
    QCheckBox *allowNewProfilesCheck = nullptr;
    QSystemTrayIcon *trayIcon = nullptr;
    QMenu *fileMenu = nullptr;
    QMenu *settingsMenu = nullptr;
    QMenu *serviceMenu = nullptr;
    QMenu *serviceRatingMenu = nullptr;
    QMenu *helpMenu = nullptr;
    QAction *actLoadProfiles = nullptr;
    QAction *actLoadProfilesDb = nullptr;
    QAction *actNewProfiles = nullptr;
    QAction *actNewProfilesDb = nullptr;
    QAction *actSaveProfilesDb = nullptr;
    QAction *actExit = nullptr;
    QAction *actPreferences = nullptr;
    QAction *actCountVideoSize = nullptr;
    QAction *actRecalculateVideoSize = nullptr;
    QAction *actAddRated5 = nullptr;
    QAction *actAddRated4 = nullptr;
    QAction *actAddRated3 = nullptr;
    QAction *actAddRated2 = nullptr;
    QAction *actAddRated1 = nullptr;
    QAction *actAddAllRated = nullptr;
    QAction *actAddOldestProfiles = nullptr;
    QAction *actAbout = nullptr;
    QAction *trayShowAction = nullptr;
    QAction *trayQuitAction = nullptr;
    QProcess *process;
    QTimer *sleepTimer;
    TsTranslator translator;
    bool translatorInstalled = false;
    QSettings *settings;
    QString languageCode;
    PreferencesDialog *preferencesDialog = nullptr;

    QVector<QString> queueProfiles;
    QVector<QString> selectedProfiles;
    int totalProfiles;
    int processed;
    bool paused, canceled, usingResume;
    bool isProcessing;
    int success, failed, skipped;
    bool checkpointError;
    bool profileNotFoundError = false;
    bool sourceTableUpdating = false;
    bool sourceBackedByDatabase = false;
    bool videoSizeRecalcInProgress = false;
    QString lastSourceKind;
    QString workingDir;
    QString profilesFilePath;
    QString profilesDbPath;
    QFutureWatcher<QVector<QPair<QString, qint64>>> *videoSizeRecalcWatcher = nullptr;
};

#endif // MAINWINDOW_H
