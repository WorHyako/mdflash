#ifndef MARKDOWN_PROCESSOR_H
#define MARKDOWN_PROCESSOR_H

#include <QString>

class MarkdownProcessor {
public:
    static QString processHeader(const QString& line);
    static QString processLine(const QString& line);
};

#endif // MARKDOWN_PROCESSOR_H

