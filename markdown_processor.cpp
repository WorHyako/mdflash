#include "markdown_processor.h"

QString MarkdownProcessor::processHeader(const QString& line) {
    if (line.startsWith("# ")) {
        QString content = line.mid(2).trimmed();
        return QString("<h1 style='font-size: 2em; font-weight: 600; padding-bottom: 0.3em; margin-bottom: 0;'>%1</h1>"
                       "<hr style='height: 0.25em; padding: 0; margin: 0 0 24px 0; background-color: #e1e4e8; border: 0;'>")
            .arg(content);
    } else if (line.startsWith("## ")) {
        QString content = line.mid(3).trimmed();
        return QString("<h2 style='font-size: 1.5em; font-weight: 600; padding-bottom: 0.3em; margin-bottom: 0;'>%1</h2>"
                       "<hr style='height: 0.2em; padding: 0; margin: 0 0 16px 0; background-color: #e1e4e8; border: 0;'>")
            .arg(content);
    } else if (line.startsWith("### ")) {
        QString content = line.mid(4).trimmed();
        return QString("<h3 style='font-size: 1.25em; font-weight: 600; margin-bottom: 16px;'>%1</h3>")
            .arg(content);
    }
    return line;
}

QString MarkdownProcessor::processCodeBlock(const QStringList& lines) {
    QString codeContent = lines.join("\n").toHtmlEscaped();
    return QString("<pre style='background-color: #f6f8fa; border-radius: 6px; padding: 16px; overflow: auto;'>"
                   "<code style='font-family: SFMono-Regular, Consolas, Liberation Mono, Menlo, monospace; "
                   "font-size: 85%; line-height: 1.45;'>%1</code></pre>")
            .arg(codeContent);
}

QString MarkdownProcessor::processContent(const QString& content) {
    QStringList lines = content.split("\n");
    QString processedContent;
    bool inCodeBlock = false;
    QStringList codeBlockLines;

    for (const QString& line : lines) {
        if (line.trimmed().startsWith("```")) {
            if (inCodeBlock) {
                // End of code block
                processedContent += processCodeBlock(codeBlockLines);
                codeBlockLines.clear();
                inCodeBlock = false;
            } else {
                // Start of code block
                inCodeBlock = true;
            }
        } else if (inCodeBlock) {
            codeBlockLines << line;
        } else {
            processedContent += processLine(line);
        }
    }

    // Handle any remaining code block
    if (inCodeBlock) {
        processedContent += processCodeBlock(codeBlockLines);
    }

    return processedContent;
}

QString MarkdownProcessor::processLine(const QString& line) {
    QString processedLine = processHeader(line);
    if (processedLine == line) {  // If it's not a header, preserve line breaks
        return processedLine + "<br>";
    }
    return processedLine;
}
