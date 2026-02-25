#ifndef TS_TRANSLATOR_H
#define TS_TRANSLATOR_H

#include <QTranslator>
#include <QHash>
#include <QString>

/**
 * @brief Lightweight runtime translator that loads Qt Linguist .ts XML files.
 *
 * This avoids requiring lrelease/lupdate on the target system.
 * You can still edit translations with Qt Linguist (.ts files).
 */
class TsTranslator final : public QTranslator
{
    Q_OBJECT
public:
    /**
     * @brief Creates a translator instance.
     * @param parent Parent QObject.
     */
    explicit TsTranslator(QObject *parent = nullptr);

    /**
     * @brief Loads translations from a Qt resource .ts file.
     * @param resourcePath Resource path (for example, ":/i18n/app_ru.ts").
     * @return True on successful parse and load.
     */
    bool loadFromTsResource(const QString &resourcePath);
    /**
     * @brief Clears all loaded translations.
     */
    void clear();

    /**
     * @brief Resolves translated text for a source string.
     * @param context Translation context.
     * @param sourceText Source text in original language.
     * @param disambiguation Optional disambiguation suffix.
     * @param n Optional plural form selector.
     * @return Translated text when available, otherwise empty string.
     */
    QString translate(const char *context,
                      const char *sourceText,
                      const char *disambiguation = nullptr,
                      int n = -1) const override;

private:
    /**
     * @brief Builds hash key for translation lookup.
     * @param context Translation context.
     * @param source Source text.
     * @param disambiguation Disambiguation value.
     * @return Composite key used by internal map.
     */
    static QString makeKey(const QString &context,
                           const QString &source,
                           const QString &disambiguation);

    QHash<QString, QString> map_;
};

#endif // TS_TRANSLATOR_H
