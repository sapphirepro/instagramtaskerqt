#ifndef PREFERENCESDIALOG_H
#define PREFERENCESDIALOG_H

#include <QDialog>
#include <QTabWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QComboBox>
#include <QLabel>
#include <QCheckBox>
#include <QDialogButtonBox>
#include <QPlainTextEdit>
#include <QSpinBox>
#include <QString>
#include <QEvent>

/**
 * @brief Preferences dialog for general app options and default profile flags.
 */
class PreferencesDialog : public QDialog
{
    Q_OBJECT
public:
    /**
     * @brief Constructs preferences dialog and initializes controls.
     * @param parent Parent widget.
     */
    explicit PreferencesDialog(QWidget *parent = nullptr);

    /**
     * @brief Sets working directory text field.
     * @param dir Working directory path.
     */
    void setWorkingDirectory(const QString &dir);
    /**
     * @brief Returns currently selected working directory.
     * @return Working directory path.
     */
    QString workingDirectory() const;
    /**
     * @brief Sets Instaloader script path field.
     * @param path Path to `instaloader.py`.
     */
    void setInstaloaderPath(const QString &path);
    /**
     * @brief Returns current Instaloader script path.
     * @return Instaloader script path.
     */
    QString instaloaderPath() const;
    /**
     * @brief Sets environment command fields.
     * @param pythonExecutable Python executable command or path.
     * @param fileManager File manager command or path.
     * @param galleryViewer Gallery viewer command or path.
     */
    void setEnvironmentCommands(const QString &pythonExecutable,
                                const QString &fileManager,
                                const QString &galleryViewer);
    /**
     * @brief Returns selected Python executable command.
     * @return Python executable command.
     */
    QString pythonExecutable() const;
    /**
     * @brief Returns selected file manager command.
     * @return File manager command.
     */
    QString fileManagerCommand() const;
    /**
     * @brief Returns selected gallery viewer command.
     * @return Gallery viewer command.
     */
    QString galleryViewerCommand() const;

    /**
     * @brief Sets active UI language code.
     * @param code Language code.
     */
    void setLanguageCode(const QString &code);
    /**
     * @brief Returns active UI language code.
     * @return Language code.
     */
    QString languageCode() const;
    /**
     * @brief Sets downloader options shown in the dialog.
     * @param login Login name for Instaloader.
     * @param noMetadata True to skip metadata JSON files.
     * @param noCaptions True to skip caption text files.
     * @param userAgent User-Agent string.
     * @param noIphone True to disable iPhone endpoint usage.
     * @param abort400 Abort on HTTP 400 errors.
     * @param abort401 Abort on HTTP 401 errors.
     * @param abort403 Abort on HTTP 403 errors.
     * @param abort404 Abort on HTTP 404 errors.
     * @param abort429 Abort on HTTP 429 errors.
     */
    void setDownloaderOptions(const QString &login,
                              bool noMetadata,
                              bool noCaptions,
                              const QString &userAgent,
                              bool noIphone,
                              bool abort400,
                              bool abort401,
                              bool abort403,
                              bool abort404,
                              bool abort429);
    /**
     * @brief Sets default checkbox values for newly created profiles.
     * @param enabled Default "enabled" flag.
     * @param videos Default "videos" flag.
     * @param stories Default "stories" flag.
     * @param highlights Default "highlights" flag.
     * @param tagged Default "tagged" flag.
     * @param reels Default "reels" flag.
     */
    void setDefaultBehaviour(bool enabled,
                             bool videos,
                             bool stories,
                             bool highlights,
                             bool tagged,
                             bool reels);
    /**
     * @brief Sets batch queue limit value.
     * @param limit Maximum number of profiles to enqueue in one batch operation.
     */
    void setBatchQueueLimit(int limit);
    /**
     * @brief Returns batch queue limit value.
     * @return Configured batch queue limit.
     */
    int batchQueueLimit() const;

protected:
    /**
     * @brief Handles language change events to refresh labels.
     * @param event Qt event payload.
     */
    void changeEvent(QEvent *event) override;

signals:
    /**
     * @brief Requests parent to open working directory chooser.
     */
    void workingDirectoryBrowseRequested();
    /**
     * @brief Requests parent to open Instaloader script chooser.
     */
    void instaloaderBrowseRequested();
    /**
     * @brief Requests parent to open Python executable chooser.
     */
    void pythonBrowseRequested();
    /**
     * @brief Requests parent to open file manager chooser.
     */
    void fileManagerBrowseRequested();
    /**
     * @brief Requests parent to open gallery viewer chooser.
     */
    void galleryViewerBrowseRequested();
    /**
     * @brief Emits accepted preferences payload.
     * @param workingDir Working directory.
     * @param instaloaderPath Instaloader script path.
     * @param pythonExecutable Python executable command.
     * @param fileManager File manager command.
     * @param galleryViewer Gallery viewer command.
     * @param languageCode UI language code.
     * @param login Instaloader login name.
     * @param noMetadata Skip metadata files flag.
     * @param noCaptions Skip caption files flag.
     * @param userAgent User-Agent value.
     * @param noIphone Disable iPhone endpoint flag.
     * @param abort400 Abort on HTTP 400.
     * @param abort401 Abort on HTTP 401.
     * @param abort403 Abort on HTTP 403.
     * @param abort404 Abort on HTTP 404.
     * @param abort429 Abort on HTTP 429.
     * @param defEnabled Default "enabled" value.
     * @param defVideos Default "videos" value.
     * @param defStories Default "stories" value.
     * @param defHighlights Default "highlights" value.
     * @param defTagged Default "tagged" value.
     * @param defReels Default "reels" value.
     * @param batchQueueLimit Batch queue limit for service operations.
     */
    void preferencesAccepted(const QString &workingDir,
                            const QString &instaloaderPath,
                            const QString &pythonExecutable,
                            const QString &fileManager,
                            const QString &galleryViewer,
                            const QString &languageCode,
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
                            int batchQueueLimit);

private:
    /**
     * @brief Refreshes all visible labels according to current translator.
     */
    void updateTexts();

private slots:
    /**
     * @brief Handles click on working directory browse button.
     */
    void onBrowseClicked();
    /**
     * @brief Handles click on Instaloader browse button.
     */
    void onInstaloaderBrowseClicked();
    /**
     * @brief Handles click on Python browse button.
     */
    void onPythonBrowseClicked();
    /**
     * @brief Handles click on file manager browse button.
     */
    void onFileManagerBrowseClicked();
    /**
     * @brief Handles click on gallery viewer browse button.
     */
    void onGalleryViewerBrowseClicked();
    /**
     * @brief Validates and emits preferences when dialog is accepted.
     */
    void onAccepted();

private:
    QTabWidget *tabs;
    QWidget *generalTab;
    QWidget *defaultBehaviourTab;
    QWidget *environmentTab;

    QLabel *workingDirLabel;
    QLabel *instaloaderPathLabel;
    QLabel *pythonExecutableLabel;
    QLabel *fileManagerLabel;
    QLabel *galleryViewerLabel;
    QLabel *languageLabel;
    QLabel *loginLabel;
    QLabel *userAgentLabel;
    QLabel *abortOnLabel;
    QLabel *batchQueueLimitLabel;

    QLineEdit *workingDirEdit;
    QLineEdit *instaloaderPathEdit;
    QPushButton *instaloaderBrowseButton;
    QLineEdit *pythonExecutableEdit;
    QLineEdit *fileManagerEdit;
    QLineEdit *galleryViewerEdit;
    QPushButton *pythonBrowseButton;
    QPushButton *fileManagerBrowseButton;
    QPushButton *galleryViewerBrowseButton;
    QLineEdit *loginEdit;
    QPlainTextEdit *userAgentEdit;
    QPushButton *browseButton;

    QComboBox *languageCombo;
    QCheckBox *skipMetadataCheck;
    QCheckBox *skipCaptionsCheck;
    QCheckBox *noIphoneCheck;
    QCheckBox *abort400Check;
    QCheckBox *abort401Check;
    QCheckBox *abort403Check;
    QCheckBox *abort404Check;
    QCheckBox *abort429Check;
    QCheckBox *defaultEnabledCheck;
    QCheckBox *defaultVideosCheck;
    QCheckBox *defaultStoriesCheck;
    QCheckBox *defaultHighlightsCheck;
    QCheckBox *defaultTaggedCheck;
    QCheckBox *defaultReelsCheck;
    QSpinBox *batchQueueLimitSpin;

    QDialogButtonBox *buttonBox;

    QString currentLanguageCode;
};

#endif // PREFERENCESDIALOG_H
