#include "custom_shell.h"

void on_run_clicked(GtkButton *button, gpointer user_data) {
    AppData *app = user_data;
    const char *command = gtk_entry_get_text(GTK_ENTRY(app->entry));
    
    if (strlen(command) == 0) return;
    
    add_to_history(app, command);
    
    GtkTextIter iter;
    gtk_text_buffer_get_end_iter(app->buffer, &iter);
    
    gtk_text_buffer_insert(app->buffer, &iter, "$ ", -1); // Add prompt before command
    
    char *cmd_copy = g_strdup(command);
    char *cmd_name = strtok(cmd_copy, " \t");
    const char *tag_name = "default";
    if (cmd_name) {
        if (strcmp(cmd_name, "cd") == 0) {
            tag_name = "cd";
        } else if (strcmp(cmd_name, "ls") == 0 || strcmp(cmd_name, "dir") == 0) {
            tag_name = "ls";
        } else if (strcmp(cmd_name, "pwd") == 0) {
            tag_name = "pwd";
        } else if (strcmp(cmd_name, "echo") == 0) {
            tag_name = "echo";
        } else if (strcmp(cmd_name, "cat") == 0) {
            tag_name = "cat";
        } else if (strcmp(cmd_name, "calc") == 0) {
            tag_name = "calc";
        } else if (strcmp(cmd_name, "help") == 0) {
            tag_name = "help";
        }
    }
    
    gtk_text_buffer_insert_with_tags_by_name(app->buffer, &iter, command, -1, tag_name, NULL);
    gtk_text_buffer_insert(app->buffer, &iter, "\n", -1);
    
    g_free(cmd_copy);
    
    execute_command(command, app->buffer, app->textview); // Removed app parameter
    
    gtk_text_buffer_get_end_iter(app->buffer, &iter);
    GtkTextMark *mark = gtk_text_buffer_create_mark(app->buffer, "end", &iter, FALSE);
    gtk_text_view_scroll_to_mark(GTK_TEXT_VIEW(app->textview), mark, 0.0, FALSE, 0.0, 0.0); // Changed app_data to app
    gtk_text_buffer_delete_mark(app->buffer, mark);
    
    gtk_entry_set_text(GTK_ENTRY(app->entry), "");
}

void on_clear_clicked(GtkButton *button, gpointer user_data) {
    AppData *app = user_data;
    gtk_text_buffer_set_text(app->buffer, "", -1);
    // Add prompt back after clearing
    GtkTextIter iter;
    gtk_text_buffer_get_end_iter(app->buffer, &iter);
    gtk_text_buffer_insert(app->buffer, &iter, "$ ", -1);
}

void on_time_clicked(GtkButton *button, gpointer user_data) {
    AppData *app = user_data;
    time_t now = time(NULL);
    char *time_str = ctime(&now);
    
    GtkTextIter iter;
    gtk_text_buffer_get_end_iter(app->buffer, &iter);
    gtk_text_buffer_insert(app->buffer, &iter, "$ ", -1); // Add prompt
    gtk_text_buffer_insert(app->buffer, &iter, "Current time: ", -1);
    gtk_text_buffer_insert(app->buffer, &iter, time_str, -1);
}

void on_theme_clicked(GtkButton *button, gpointer user_data) {
    AppData *app = user_data;
    cycle_theme(app);
}

void on_history_clicked(GtkButton *button, gpointer user_data) {
    AppData *app = user_data;
    show_history(app);
}

gboolean on_entry_key_press(GtkWidget *widget, GdkEventKey *event, gpointer user_data) {
    AppData *app = user_data;
    
    if (event->keyval == GDK_KEY_Up || event->keyval == GDK_KEY_Down) {
        if (app->history_count == 0) return TRUE;
        
        if (event->keyval == GDK_KEY_Up && app->history_index > 0) {
            app->history_index--;
        } else if (event->keyval == GDK_KEY_Down && app->history_index < app->history_count - 1) {
            app->history_index++;
        }
        
        if (app->history_index >= 0 && app->history_index < app->history_count) {
            gtk_entry_set_text(GTK_ENTRY(app->entry), app->command_history[app->history_index]);
            gtk_editable_set_position(GTK_EDITABLE(app->entry), -1);
        }
        return TRUE;
    } else if (event->keyval == GDK_KEY_Return) {
        gtk_widget_activate(app->run_button);
        return TRUE;
    }
    
    return FALSE;
}

void on_window_destroy(GtkWidget *widget, gpointer user_data) {
    AppData *app_data = user_data;
    destroy_app_data(app_data);
}