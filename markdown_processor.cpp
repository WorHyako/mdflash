#include "markdown_processor.h"
#include <QRegularExpression>

QString MarkdownProcessor::processHeader(const QString& line) {
    if (line.startsWith("# ")) {
        QString content = line.mid(2).trimmed();
        return QString("<h1 style='font-size: 2.5em; font-weight: 600; padding-bottom: 0.3em; margin-bottom: 0;'>%1</h1>"
                       "<hr style='height: 0.25em; padding: 0; margin: 0 0 24px 0; background-color: #d0d7de; border: 0;'>")
            .arg(content);
    } else if (line.startsWith("## ")) {
        QString content = line.mid(3).trimmed();
        return QString("<h2 style='font-size: 1.875em; font-weight: 600; padding-bottom: 0.3em; margin-bottom: 0;'>%1</h2>"
                       "<hr style='height: 0.2em; padding: 0; margin: 0 0 16px 0; background-color: #d0d7de; border: 0;'>")
            .arg(content);
    } else if (line.startsWith("### ")) {
        QString content = line.mid(4).trimmed();
        return QString("<h3 style='font-size: 1.5625em; font-weight: 600; margin-bottom: 16px;'>%1</h3>")
            .arg(content);
    }
    return line;
}

QString MarkdownProcessor::processContent(const QString& content) {
    QStringList lines = content.split("\n");
    QString processedContent;
    bool inCodeBlock = false;
    bool inList = false;
    bool inNumberedList = false;
    QStringList codeBlockLines;
    QStringList listItems;
    int listNumber = 1;

    for (const QString& line : lines) {
        if (line.trimmed().startsWith("```")) {
            if (inCodeBlock) {
                // End of code block
                processedContent += processCodeBlock(codeBlockLines);
                codeBlockLines.clear();
                inCodeBlock = false;
            } else {
                // Start of code block
                if (inList) {
                    processedContent += processList(listItems);
                    listItems.clear();
                    inList = false;
                }
                if (inNumberedList) {
                    processedContent += processNumberedList(listItems);
                    listItems.clear();
                    inNumberedList = false;
                    listNumber = 1;
                }
                inCodeBlock = true;
            }
        } else if (inCodeBlock) {
            codeBlockLines << line;
        } else {
            if (inList) {
                processedContent += processList(listItems);
                listItems.clear();
                inList = false;
            }
            if (inNumberedList) {
                processedContent += processNumberedList(listItems);
                listItems.clear();
                inNumberedList = false;
                listNumber = 1;
            }
            QString processedLine = processLine(line);
            processedLine = processBoldText(processedLine);
            processedLine = processItalicText(processedLine);
            processedLine = processInlineCode(processedLine); 
            processedContent += processedLine;
        }
    }

    if (inCodeBlock) {
        processedContent += processCodeBlock(codeBlockLines);
    } else if (inList) {
        processedContent += processList(listItems);
    } else if (inNumberedList) {
        processedContent += processNumberedList(listItems);
    }

    return processedContent;
}

QString MarkdownProcessor::processInlineCode(const QString& text) {
    QRegularExpression inlineCodeRegex("(`[^`\n]+`)");
    QString processed = text;
    auto matches = inlineCodeRegex.globalMatch(processed);
    while (matches.hasNext()) {
        auto match = matches.next();
        QString codeText = match.captured(1);
        codeText = codeText.mid(1, codeText.length() - 2);
        processed.replace(match.captured(0), 
            QString("<code style='background-color: #F0F1F2; padding: 0.2em 0.4em; "
                    "border-radius: 3px; font-family: monospace; font-size: 85%;'>%1</code>")
                .arg(codeText.toHtmlEscaped()));
    }
    return processed;
}

QString MarkdownProcessor::processCodeBlock(const QStringList& lines) {
    QString codeContent = lines.join("\n").toHtmlEscaped();
    return QString("<pre style='background-color: #F6F8FA; border-radius: 6px; padding: 16px; overflow: auto;'>"
                   "<code style='font-family: monospace; font-size: 20px;'>%1</code></pre>")
            .arg(codeContent);
}


QString MarkdownProcessor::processNumberedList(const QStringList& items) {
    QString listHtml = "<ol style='padding-left: 2em; margin-bottom: 16px; font-size: 20px;'>";
    for (const QString& item : items) {
        listHtml += QString("<li style='margin-bottom: 4px;'>%1</li>").arg(item);
    }
    listHtml += "</ol>";
    return listHtml;
}

QString MarkdownProcessor::processBoldText(const QString& text) {
    QRegularExpression boldRegex("\\*\\*(.*?)\\*\\*");
    QString processed = text;
    auto matches = boldRegex.globalMatch(processed);
    while (matches.hasNext()) {
        auto match = matches.next();
        QString boldText = match.captured(1);
        processed.replace(match.captured(0), QString("<strong style='font-weight: 600;'>%1</strong>").arg(boldText));
    }
    return processed;
}

QString MarkdownProcessor::processItalicText(const QString& text) {
    QRegularExpression italicRegex("\\*(.*?)\\*");
    QString processed = text;
    auto matches = italicRegex.globalMatch(processed);
    while (matches.hasNext()) {
        auto match = matches.next();
        QString italicText = match.captured(1);
        processed.replace(match.captured(0), QString("<em style='font-style: italic;'>%1</em>").arg(italicText));
    }
    return processed;
}

QString MarkdownProcessor::processLine(const QString& line) {
    QString processedLine = processHeader(line);
    if (processedLine == line) {
        if (!line.trimmed().isEmpty()) {
            return QString("<p style='font-size: 20px; line-height: 1.5;'>%1</p>").arg(line);
        } else {
            return processedLine;
        }
    }
    return processedLine;
}

QString MarkdownProcessor::processList(const QStringList& items) {
    QString listHtml = "<ul style='padding-left: 2em; margin-bottom: 16px; font-size: 20px;'>";
    for (const QString& item : items) {
        listHtml += QString("<li style='margin-bottom: 4px;'>%1</li>").arg(item);
    }
    listHtml += "</ul>";
    return listHtml;
}





