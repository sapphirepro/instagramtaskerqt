#ifndef UI_STRINGS_H
#define UI_STRINGS_H

#include <QString>
#include <QHash>

/**
 * @brief Static dictionary-based UI string provider.
 */
class UiStrings
{
public:
    /**
     * @brief Resolves localized text by key and language code.
     * @param key Translation key.
     * @param langCode Language code (e.g. "en", "de", "ru").
     * @return Localized string for the requested key.
     */
    static QString t(const QString &key, const QString &langCode);

private:
    /**
     * @brief Returns immutable localization dictionary.
     * @return Dictionary indexed by key and language code.
     */
    static const QHash<QString, QHash<QString, QString>>& dict();
};

#endif // UI_STRINGS_H
