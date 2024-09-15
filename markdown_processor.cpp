#include "markdown_processor.h"

QString MarkdownProcessor::processHeader(const QString& line) {
    if (line.startsWith("# ")) {
        QString content = line.mid(2).trimmed();
        return QString("<h1 style='font-size: 2em; font-weight: 600; padding-bottom: 0.3em; margin-bottom: 0;'>%1</h1>"
                       "<hr style='height: 0.25em; padding: 0; margin: 0 0 24px 0; background-color: #e1e4e8; border: 0;'>")
            .arg(content);
    }
    return line;
}

QString MarkdownProcessor::processLine(const QString& line) {
    QString processedLine = processHeader(line);
    if (processedLine == line) {  // If it's not a header, preserve line breaks
        return processedLine + "<br>";
    }
    return processedLine;
}

