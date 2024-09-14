#ifndef MARKDOWN_PARSER_H
#define MARKDOWN_PARSER_H

#include <gtk/gtk.h>

typedef enum {
    MD_NORMAL,
    MD_HEADING_1,
    MD_HEADING_2,
    MD_HEADING_3,
    MD_HEADING_4,
    MD_HEADING_5,
    MD_HEADING_6,
    MD_BOLD,
    MD_ITALIC,
    MD_CODE,
    MD_CODE_BLOCK,
    MD_LINK,
    MD_LIST_ITEM,
    MD_BLOCKQUOTE
} MarkdownElementType;

typedef struct {
    MarkdownElementType type;
    char *content;
    char *url;  // Used for links
    char *language;  // Used for code blocks
} MarkdownElement;

MarkdownElement *parse_markdown_line(const char *line);
void insert_markdown_element(GtkTextBuffer *buffer, GtkTextIter *iter, MarkdownElement *element);
void free_markdown_element(MarkdownElement *element);

#endif // MARKDOWN_PARSER_H
