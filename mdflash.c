#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "markdown_parser.h"
#include "css_styles.h"

#define MAX_LINE_LENGTH 4096

typedef struct {
    GtkWidget *window;
    GtkWidget *scrolled_window;
    GtkWidget *text_view;
    GtkTextBuffer *buffer;
    GtkCssProvider *provider;
} AppWidgets;

static void apply_css(GtkWidget *widget, GtkStyleProvider *provider) {
    gtk_style_context_add_provider(
        gtk_widget_get_style_context(widget),
        provider,
        GTK_STYLE_PROVIDER_PRIORITY_APPLICATION
    );

    if (GTK_IS_CONTAINER(widget)) {
        gtk_container_forall(GTK_CONTAINER(widget), (GtkCallback)apply_css, provider);
    }
}

static void load_css(AppWidgets *app) {
    app->provider = gtk_css_provider_new();
    gtk_css_provider_load_from_data(app->provider, github_markdown_css, -1, NULL);
    apply_css(app->window, GTK_STYLE_PROVIDER(app->provider));
}

static void create_widgets(AppWidgets *app) {
    app->window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(app->window), "MDFlash - GitHub Markdown Viewer");
    gtk_window_set_default_size(GTK_WINDOW(app->window), 800, 600);

    app->scrolled_window = gtk_scrolled_window_new(NULL, NULL);
    gtk_container_set_border_width(GTK_CONTAINER(app->scrolled_window), 10);

    app->text_view = gtk_text_view_new();
    gtk_text_view_set_editable(GTK_TEXT_VIEW(app->text_view), FALSE);
    gtk_text_view_set_cursor_visible(GTK_TEXT_VIEW(app->text_view), FALSE);
    gtk_text_view_set_wrap_mode(GTK_TEXT_VIEW(app->text_view), GTK_WRAP_WORD);
    gtk_text_view_set_left_margin(GTK_TEXT_VIEW(app->text_view), 20);
    gtk_text_view_set_right_margin(GTK_TEXT_VIEW(app->text_view), 20);

    app->buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(app->text_view));

    gtk_container_add(GTK_CONTAINER(app->scrolled_window), app->text_view);
    gtk_container_add(GTK_CONTAINER(app->window), app->scrolled_window);

    g_signal_connect(app->window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
}

static void apply_tag_to_range(GtkTextBuffer *buffer, const gchar *tag_name, GtkTextIter *start, GtkTextIter *end) {
    GtkTextTag *tag = gtk_text_tag_table_lookup(gtk_text_buffer_get_tag_table(buffer), tag_name);
    if (!tag) {
        if (strcmp(tag_name, "bold") == 0) {
            tag = gtk_text_buffer_create_tag(buffer, "bold", "weight", PANGO_WEIGHT_BOLD, NULL);
        } else if (strcmp(tag_name, "italic") == 0) {
            tag = gtk_text_buffer_create_tag(buffer, "italic", "style", PANGO_STYLE_ITALIC, NULL);
        } else if (strcmp(tag_name, "code") == 0) {
            tag = gtk_text_buffer_create_tag(buffer, "code",
                                             "family", "monospace",
                                             "background", "#f6f8fa",
                                             "paragraph-background", "#f6f8fa",
                                             NULL);
        } else if (strcmp(tag_name, "heading") == 0) {
            tag = gtk_text_buffer_create_tag(buffer, "heading",
                                             "weight", PANGO_WEIGHT_BOLD,
                                             "size", 18 * PANGO_SCALE,
                                             NULL);
        }
    }
    if (tag) {
        gtk_text_buffer_apply_tag(buffer, tag, start, end);
    }
}

static void process_inline_formatting(GtkTextBuffer *buffer) {
    GtkTextIter start, end;
    gtk_text_buffer_get_bounds(buffer, &start, &end);

    GtkTextIter match_start, match_end;
    GtkTextSearchFlags flags = GTK_TEXT_SEARCH_TEXT_ONLY;

    // Process bold text
    while (gtk_text_iter_forward_search(&start, "**", flags, &match_start, &match_end, &end)) {
        GtkTextIter bold_end;
        if (gtk_text_iter_forward_search(&match_end, "**", flags, &bold_end, &end, &end)) {
            gtk_text_buffer_delete(buffer, &bold_end, &end);
            gtk_text_buffer_delete(buffer, &match_start, &match_end);
            apply_tag_to_range(buffer, "bold", &match_start, &bold_end);
            start = bold_end;
        } else {
            start = match_end;
        }
    }

    // Reset iterators
    gtk_text_buffer_get_bounds(buffer, &start, &end);

    // Process italic text
    while (gtk_text_iter_forward_search(&start, "*", flags, &match_start, &match_end, &end)) {
        GtkTextIter italic_end;
        if (gtk_text_iter_forward_search(&match_end, "*", flags, &italic_end, &end, &end)) {
            gtk_text_buffer_delete(buffer, &italic_end, &end);
            gtk_text_buffer_delete(buffer, &match_start, &match_end);
            apply_tag_to_range(buffer, "italic", &match_start, &italic_end);
            start = italic_end;
        } else {
            start = match_end;
        }
    }
}

static void insert_markdown(AppWidgets *app, const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        fprintf(stderr, "Failed to open file: %s\n", filename);
        exit(1);
    }

    char line[MAX_LINE_LENGTH];
    GtkTextIter iter;
    gtk_text_buffer_get_start_iter(app->buffer, &iter);

    int in_code_block = 0;
    GtkTextMark *code_block_start = NULL;

    while (fgets(line, sizeof(line), file) != NULL) {
        MarkdownElement *element = parse_markdown_line(line);

        if (element->type == MD_CODE_BLOCK) {
            if (!in_code_block) {
                in_code_block = 1;
                code_block_start = gtk_text_buffer_create_mark(app->buffer, NULL, &iter, TRUE);
            } else {
                in_code_block = 0;
                GtkTextIter code_start, code_end;
                gtk_text_buffer_get_iter_at_mark(app->buffer, &code_start, code_block_start);
                code_end = iter;
                apply_tag_to_range(app->buffer, "code", &code_start, &code_end);
                gtk_text_buffer_delete_mark(app->buffer, code_block_start);
            }
        } else if (in_code_block) {
            gtk_text_buffer_insert(app->buffer, &iter, element->content, -1);
        } else {
            GtkTextMark *start_mark = gtk_text_buffer_create_mark(app->buffer, NULL, &iter, TRUE);
            insert_markdown_element(app->buffer, &iter, element);
            GtkTextIter start_iter, end_iter;
            gtk_text_buffer_get_iter_at_mark(app->buffer, &start_iter, start_mark);
            end_iter = iter;

            if (element->type >= MD_HEADING_1 && element->type <= MD_HEADING_6) {
                apply_tag_to_range(app->buffer, "heading", &start_iter, &end_iter);
            }

            gtk_text_buffer_delete_mark(app->buffer, start_mark);
        }

        free_markdown_element(element);
    }

    fclose(file);

    // Process inline formatting after all text has been inserted
    process_inline_formatting(app->buffer);
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <README.md>\n", argv[0]);
        return 1;
    }

    gtk_init(&argc, &argv);

    AppWidgets app;
    create_widgets(&app);
    load_css(&app);
    insert_markdown(&app, argv[1]);

    gtk_widget_show_all(app.window);
    gtk_main();

    return 0;
}
