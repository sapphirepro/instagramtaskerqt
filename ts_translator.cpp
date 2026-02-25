#include "ts_translator.h"

#include <QFile>
#include <QXmlStreamReader>

TsTranslator::TsTranslator(QObject *parent)
    : QTranslator(parent)
{
}

void TsTranslator::clear()
{
    map_.clear();
}

QString TsTranslator::makeKey(const QString &context,
                              const QString &source,
                              const QString &disambiguation)
{
    // Use ASCII Unit Separator as delimiter.
    static const QChar sep(0x1F);
    return context + sep + source + sep + disambiguation;
}

bool TsTranslator::loadFromTsResource(const QString &resourcePath)
{
    clear();

    QFile f(resourcePath);
    if (!f.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return false;
    }

    QXmlStreamReader xml(&f);

    QString currentContext;
    bool inMessage = false;

    QString source;
    QString translation;
    QString disambiguation;

    while (!xml.atEnd()) {
        xml.readNext();

        if (xml.isStartElement()) {
            // Qt6: QXmlStreamReader::name() returns QStringView (QStringRef was removed).
            const QString name = xml.name().toString();

            if (name == QLatin1String("context")) {
                currentContext.clear();
            } else if (name == QLatin1String("name")) {
                currentContext = xml.readElementText();
            } else if (name == QLatin1String("message")) {
                inMessage = true;
                source.clear();
                translation.clear();
                disambiguation.clear();
            } else if (inMessage && name == QLatin1String("source")) {
                source = xml.readElementText();
            } else if (inMessage && name == QLatin1String("translation")) {
                const QString type = xml.attributes().value(QLatin1String("type")).toString();
                const QString text = xml.readElementText();
                if (type != QLatin1String("unfinished")) {
                    translation = text;
                }
            } else if (inMessage && name == QLatin1String("comment")) {
                // Qt uses <comment> as disambiguation in some cases.
                disambiguation = xml.readElementText();
            } else if (inMessage && (name == QLatin1String("numerusform") ||
                                     name == QLatin1String("numerusform"))) {
                // ignore plural forms for now
                xml.readElementText();
            }
        } else if (xml.isEndElement()) {
            if (xml.name() == QLatin1String("message") && inMessage) {
                inMessage = false;

                if (!currentContext.isEmpty() && !source.isEmpty() && !translation.isEmpty()) {
                    map_.insert(makeKey(currentContext, source, disambiguation), translation);
                    if (!disambiguation.isEmpty()) {
                        // Also store without disambiguation as fallback.
                        map_.insert(makeKey(currentContext, source, QString()), translation);
                    }
                }
            }
        }
    }

    return !xml.hasError();
}

QString TsTranslator::translate(const char *context,
                               const char *sourceText,
                               const char *disambiguation,
                               int /*n*/) const
{
    const QString ctx = QString::fromUtf8(context ? context : "");
    const QString src = QString::fromUtf8(sourceText ? sourceText : "");
    const QString dis = QString::fromUtf8(disambiguation ? disambiguation : "");

    auto it = map_.constFind(makeKey(ctx, src, dis));
    if (it != map_.constEnd()) {
        return it.value();
    }

    // Fallback: try without disambiguation.
    it = map_.constFind(makeKey(ctx, src, QString()));
    if (it != map_.constEnd()) {
        return it.value();
    }

    return QString();
}
