#ifndef MARKDOWN_PROCESSOR_H
#define MARKDOWN_PROCESSOR_H

#include <QString>
#include <QStringList>

class MarkdownProcessor {
public:
    static QString processHeader(const QString& line);
    static QString processLine(const QString& line);
    static QString processCodeBlock(const QStringList& lines);
    static QString processContent(const QString& content);
};

#endif // MARKDOWN_PROCESSOR_H
