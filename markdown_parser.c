#include "markdown_parser.h"
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

static int count_leading_chars(const char *str, char c) {
    int count = 0;
    while (*str == c) {
        count++;
        str++;
    }
    return count;
}

static int is_code_block_start(const char *line) {
    return (strncmp(line, "```", 3) == 0);
}

static char *extract_language(const char *line) {
    const char *start = line + 3;  // Skip the ```
    const char *end = strchr(start, '\n');
    if (end == NULL) {
        end = start + strlen(start);
    }
    int len = end - start;
    char *language = malloc(len + 1);
    strncpy(language, start, len);
    language[len] = '\0';
    return language;
}

static int is_list_item(const char *line) {
    return (*line == '-' || *line == '*' || *line == '+') && isspace(*(line + 1));
}

static int is_blockquote(const char *line) {
    return (*line == '>');
}

MarkdownElement *parse_markdown_line(const char *line) {
    MarkdownElement *element = malloc(sizeof(MarkdownElement));
    element->content = strdup(line);
    element->url = NULL;
    element->language = NULL;

    // Check for code blocks
    if (is_code_block_start(line)) {
        element->type = MD_CODE_BLOCK;
        element->language = extract_language(line);
        return element;
    }

    // Check for headings
    if (line[0] == '#') {
        int level = count_leading_chars(line, '#');
        if (level >= 1 && level <= 6 && isspace(line[level])) {
            element->type = MD_HEADING_1 + (level - 1);
            memmove(element->content, element->content + level + 1, strlen(element->content) - level);
            return element;
        }
    }

    // Check for list items
    if (is_list_item(line)) {
        element->type = MD_LIST_ITEM;
        memmove(element->content, element->content + 2, strlen(element->content) - 1);
        return element;
    }

    // Check for blockquotes
    if (is_blockquote(line)) {
        element->type = MD_BLOCKQUOTE;
        memmove(element->content, element->content + 2, strlen(element->content) - 1);
        return element;
    }

    // Check for links
    char *link_start = strstr(element->content, "[");
    char *link_mid = strstr(element->content, "](");
    char *link_end = strstr(element->content, ")");
    if (link_start && link_mid && link_end && link_mid < link_end) {
        element->type = MD_LINK;
        *link_mid = '\0';
        element->url = strdup(link_mid + 2);
        element->url[link_end - (link_mid + 2)] = '\0';
        memmove(element->content, link_start + 1, strlen(link_start));
        return element;
    }

    // Default to normal text
    element->type = MD_NORMAL;
    return element;
}

void insert_markdown_element(GtkTextBuffer *buffer, GtkTextIter *iter, MarkdownElement *element) {
    GtkTextTag *tag = NULL;

    switch (element->type) {
        case MD_HEADING_1:
        case MD_HEADING_2:
        case MD_HEADING_3:
        case MD_HEADING_4:
        case MD_HEADING_5:
        case MD_HEADING_6:
            tag = gtk_text_buffer_create_tag(buffer, NULL,
                                             "weight", PANGO_WEIGHT_BOLD,
                                             "size", (24 - (element->type - MD_HEADING_1) * 2) * PANGO_SCALE,
                                             "pixels-above-lines", 20,
                                             "pixels-below-lines", 10,
                                             NULL);
            break;
        case MD_BOLD:
            tag = gtk_text_buffer_create_tag(buffer, NULL,
                                             "weight", PANGO_WEIGHT_BOLD,
                                             NULL);
            break;
        case MD_ITALIC:
            tag = gtk_text_buffer_create_tag(buffer, NULL,
                                             "style", PANGO_STYLE_ITALIC,
                                             NULL);
            break;
        case MD_CODE:
        case MD_CODE_BLOCK:
            tag = gtk_text_buffer_create_tag(buffer, NULL,
                                             "family", "monospace",
                                             "background", "#f6f8fa",
                                             "paragraph-background", "#f6f8fa",
                                             "pixels-above-lines", 10,
                                             "pixels-below-lines", 10,
                                             NULL);
            break;
        case MD_LINK:
            tag = gtk_text_buffer_create_tag(buffer, NULL,
                                             "foreground", "blue",
                                             "underline", PANGO_UNDERLINE_SINGLE,
                                             NULL);
            break;
        case MD_LIST_ITEM:
            gtk_text_buffer_insert(buffer, iter, "• ", -1);
            tag = gtk_text_buffer_create_tag(buffer, NULL,
                                             "left-margin", 20,
                                             "pixels-above-lines", 5,
                                             "pixels-below-lines", 5,
                                             NULL);
            break;
        case MD_BLOCKQUOTE:
            tag = gtk_text_buffer_create_tag(buffer, NULL,
                                             "left-margin", 20,
                                             "pixels-above-lines", 5,
                                             "pixels-below-lines", 5,
                                             "background", "#f0f0f0",
                                             "paragraph-background", "#f0f0f0",
                                             NULL);
            break;
        default:
            break;
    }

    if (tag) {
        gtk_text_buffer_insert_with_tags(buffer, iter, element->content, -1, tag, NULL);
    } else {
        gtk_text_buffer_insert(buffer, iter, element->content, -1);
    }

    if (element->type != MD_CODE_BLOCK) {
        gtk_text_buffer_insert(buffer, iter, "\n", -1);
    }
}

void free_markdown_element(MarkdownElement *element) {
    if (element) {
        free(element->content);
        free(element->url);
        free(element->language);
        free(element);
    }
}
