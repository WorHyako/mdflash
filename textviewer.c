#include <gtk/gtk.h>
#include <stdio.h>

static void activate(GtkApplication *app, gpointer user_data) {
    GtkWidget *window;
    GtkWidget *scrolled_window;
    GtkWidget *text_view;
    GtkTextBuffer *buffer;
    char *content = NULL;
    long length;
    FILE *file;
    const char *filename = user_data;

    // Создаем главное окно
    window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), "Просмотр текстового файла");
    gtk_window_set_default_size(GTK_WINDOW(window), 400, 300);

    // Создаем виджет для прокрутки
    scrolled_window = gtk_scrolled_window_new(NULL, NULL);
    gtk_container_add(GTK_CONTAINER(window), scrolled_window);

    // Создаем текстовое поле
    text_view = gtk_text_view_new();
    gtk_container_add(GTK_CONTAINER(scrolled_window), text_view);

    // Устанавливаем белый фон
    GtkCssProvider *provider = gtk_css_provider_new();
    gtk_css_provider_load_from_data(provider,
        "textview { background-color: white; }", -1, NULL);
    gtk_style_context_add_provider(gtk_widget_get_style_context(text_view),
        GTK_STYLE_PROVIDER(provider),
        GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
    g_object_unref(provider);

    // Читаем содержимое файла
    file = fopen(filename, "rb");
    if (file) {
        fseek(file, 0, SEEK_END);
        length = ftell(file);
        fseek(file, 0, SEEK_SET);
        content = malloc(length);
        if (content) {
            fread(content, 1, length, file);
        }
        fclose(file);
    }

    // Устанавливаем текст в буфер
    buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(text_view));
    if (content) {
        gtk_text_buffer_set_text(buffer, content, length);
        free(content);
    } else {
        gtk_text_buffer_set_text(buffer, "Ошибка при чтении файла", -1);
    }

    gtk_widget_show_all(window);
}

static int command_line(GtkApplication *app, GApplicationCommandLine *cmdline, gpointer user_data) {
    gchar **argv;
    gint argc;

    argv = g_application_command_line_get_arguments(cmdline, &argc);

    if (argc != 2) {
        g_print("Использование: %s <имя_файла>\n", argv[0]);
        g_strfreev(argv);
        return 1;
    }

    g_application_activate(G_APPLICATION(app));
    activate(app, argv[1]);

    g_strfreev(argv);
    return 0;
}

int main(int argc, char **argv) {
    GtkApplication *app;
    int status;

    app = gtk_application_new("org.example.textviewer", G_APPLICATION_HANDLES_COMMAND_LINE);
    g_signal_connect(app, "command-line", G_CALLBACK(command_line), NULL);
    status = g_application_run(G_APPLICATION(app), argc, argv);
    g_object_unref(app);

    return status;
}
