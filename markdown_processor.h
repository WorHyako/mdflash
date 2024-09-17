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
    static QString processList(const QStringList& items); 
    static QString processBoldText(const QString& text);
    static QString processItalicText(const QString& text);
};

#endif // MARKDOWN_PROCESSOR_H
