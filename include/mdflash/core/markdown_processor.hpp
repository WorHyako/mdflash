#ifndef MARKDOWN_PROCESSOR_HPP
#define MARKDOWN_PROCESSOR_HPP

#include <QString>

namespace mdflash::Core {
	/**
	 * @brief
	 *
	 * @author	yellow-footed-honeyguide
	 *
	 * @since	0.6.0
	 */
	class MarkdownProcessor {
	public:
		static QString processHeader(const QString& line);

		static QString processLine(const QString& line);

		static QString processCodeBlock(const QStringList& lines);

		static QString processContent(const QString& content);

		static QString processList(const QStringList& items);

		static QString processBoldText(const QString& text);

		static QString processItalicText(const QString& text);

		static QString processNumberedList(const QStringList& items);

		static QString processInlineCode(const QString& text);

		static QString processGitHubLinks(const QString& text);
	};
}

#endif
