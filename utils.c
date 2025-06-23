#include "custom_shell.h"

void add_to_history(AppData *app, const char *command) {
    if (app->history_count < MAX_HISTORY) {
        app->command_history[app->history_count] = g_strdup(command);
        app->history_count++;
    } else {
        g_free(app->command_history[0]);
        for (int i = 0; i < MAX_HISTORY - 1; i++) {
            app->command_history[i] = app->command_history[i + 1];
        }
        app->command_history[MAX_HISTORY - 1] = g_strdup(command);
    }
    app->history_index = app->history_count - 1;
}

void show_history(AppData *app) {
    GtkTextIter iter;
    gtk_text_buffer_get_end_iter(app->buffer, &iter);
    gtk_text_buffer_insert(app->buffer, &iter, "\nCommand History:\n", -1);
    for (int i = 0; i < app->history_count; i++) {
        char num[5];
        snprintf(num, sizeof(num), "%d. ", i + 1);
        gtk_text_buffer_insert(app->buffer, &iter, num, -1);
        gtk_text_buffer_insert(app->buffer, &iter, app->command_history[i], -1);
        gtk_text_buffer_insert(app->buffer, &iter, "\n", -1);
    }
}

void destroy_app_data(AppData *app_data) {
    if (app_data->css_provider) {
        gtk_style_context_remove_provider_for_screen(gdk_screen_get_default(),
                                                    GTK_STYLE_PROVIDER(app_data->css_provider));
        g_object_unref(app_data->css_provider);
    }

    for (int i = 0; i < app_data->history_count; i++) {
        g_free(app_data->command_history[i]);
    }

    g_free(app_data);
}