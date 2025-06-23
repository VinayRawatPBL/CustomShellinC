#include "custom_shell.h"

void activate(GtkApplication *app, gpointer user_data) {
    AppData *app_data = g_new0(AppData, 1);
    app_data->window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(app_data->window), "Custom Shell");
    gtk_window_set_default_size(GTK_WINDOW(app_data->window), 800, 600);
    gtk_container_set_border_width(GTK_CONTAINER(app_data->window), 10);

    // Create vertical box
    GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    gtk_container_add(GTK_CONTAINER(app_data->window), vbox);

    // Text view for output
    app_data->textview = GTK_TEXT_VIEW(gtk_text_view_new()); // Explicit cast
    gtk_text_view_set_editable(app_data->textview, FALSE);
    gtk_text_view_set_cursor_visible(app_data->textview, FALSE);
    app_data->buffer = gtk_text_view_get_buffer(app_data->textview);
    gtk_text_buffer_create_tag(app_data->buffer, "cd", "foreground", "blue", NULL);
    gtk_text_buffer_create_tag(app_data->buffer, "ls", "foreground", "green", NULL);
    gtk_text_buffer_create_tag(app_data->buffer, "pwd", "foreground", "purple", NULL);
    gtk_text_buffer_create_tag(app_data->buffer, "echo", "foreground", "orange", NULL);
    gtk_text_buffer_create_tag(app_data->buffer, "cat", "foreground", "red", NULL);
    gtk_text_buffer_create_tag(app_data->buffer, "calc", "foreground", "brown", NULL);
    gtk_text_buffer_create_tag(app_data->buffer, "help", "foreground", "gray", NULL);
    // Create tag for welcome message with large font
    gtk_text_buffer_create_tag(app_data->buffer, "welcome", "font", "24", NULL);
    GtkWidget *scrolled_window = gtk_scrolled_window_new(NULL, NULL);
    gtk_container_add(GTK_CONTAINER(scrolled_window), GTK_WIDGET(app_data->textview)); // Explicit cast
    gtk_box_pack_start(GTK_BOX(vbox), scrolled_window, TRUE, TRUE, 0);

    // Add welcome message with large font
    GtkTextIter iter;
    gtk_text_buffer_get_end_iter(app_data->buffer, &iter);
    gtk_text_buffer_insert_with_tags_by_name(app_data->buffer, &iter, "WELCOME,\n Made by Nikhil, Vinay and Pratyush\n", -1, "welcome", NULL);
    gtk_text_buffer_insert(app_data->buffer, &iter, "\nImportant Details:\n", -1);
    gtk_text_buffer_insert(app_data->buffer, &iter, "- Last Update on: 10 June 2025\n", -1);
    gtk_text_buffer_insert(app_data->buffer, &iter, "- Current Time: ", -1);
    time_t now = time(NULL);
    char *time_str = ctime(&now);
    gtk_text_buffer_insert(app_data->buffer, &iter, time_str, -1);
    gtk_text_buffer_insert(app_data->buffer, &iter, "\nUnique Features:\n", -1);
    gtk_text_buffer_insert(app_data->buffer, &iter, "- Built-in Calculator (e.g., calc 2+3*5)\n", -1);
    gtk_text_buffer_insert(app_data->buffer, &iter, "- Theme Switching (Light, Dark, Hacker, Solarized)\n", -1);
    gtk_text_buffer_insert(app_data->buffer, &iter, "- Command History with Up/Down arrows\n", -1);
    gtk_text_buffer_insert(app_data->buffer, &iter, "- Real-time Time Display\n", -1);
    gtk_text_buffer_insert(app_data->buffer, &iter, "- Color-coded Command Output\n", -1);

    // Add initial prompt
    gtk_text_buffer_get_end_iter(app_data->buffer, &iter);
    gtk_text_buffer_insert(app_data->buffer, &iter, "\n$ ", -1);

    // Entry for command input
    app_data->entry = gtk_entry_new();
    gtk_box_pack_start(GTK_BOX(vbox), app_data->entry, FALSE, FALSE, 0);

    // Button box for actions
    GtkWidget *button_box = gtk_button_box_new(GTK_ORIENTATION_HORIZONTAL);
    gtk_button_box_set_layout(GTK_BUTTON_BOX(button_box), GTK_BUTTONBOX_CENTER);
    gtk_box_set_spacing(GTK_BOX(button_box), 5);
    gtk_box_pack_start(GTK_BOX(vbox), button_box, FALSE, FALSE, 0);

    app_data->run_button = gtk_button_new_with_label("Run Command");
    gtk_container_add(GTK_CONTAINER(button_box), app_data->run_button);

    app_data->clear_button = gtk_button_new_with_label("Clear Output");
    gtk_container_add(GTK_CONTAINER(button_box), app_data->clear_button);

    app_data->time_button = gtk_button_new_with_label("Show Time");
    gtk_container_add(GTK_CONTAINER(button_box), app_data->time_button);

    app_data->theme_button = gtk_button_new_with_label("Light Theme");
    gtk_container_add(GTK_CONTAINER(button_box), app_data->theme_button);

    app_data->history_button = gtk_button_new_with_label("Show History");
    gtk_container_add(GTK_CONTAINER(button_box), app_data->history_button);

    // Initialize theme
    app_data->theme_index = 0;
    app_data->history_count = 0;
    app_data->history_index = -1;
    cycle_theme(app_data);

    // Connect signals
    g_signal_connect(app_data->run_button, "clicked", G_CALLBACK(on_run_clicked), app_data);
    g_signal_connect(app_data->clear_button, "clicked", G_CALLBACK(on_clear_clicked), app_data);
    g_signal_connect(app_data->time_button, "clicked", G_CALLBACK(on_time_clicked), app_data);
    g_signal_connect(app_data->theme_button, "clicked", G_CALLBACK(on_theme_clicked), app_data);
    g_signal_connect(app_data->history_button, "clicked", G_CALLBACK(on_history_clicked), app_data);
    g_signal_connect(app_data->entry, "key-press-event", G_CALLBACK(on_entry_key_press), app_data);
    g_signal_connect(app_data->window, "destroy", G_CALLBACK(on_window_destroy), app_data);

    gtk_widget_show_all(app_data->window);
    gtk_widget_grab_focus(app_data->entry);
}

int main(int argc, char **argv) {
    GtkApplication *app;
    int status;

    app = gtk_application_new("com.example.custom_shell", G_APPLICATION_DEFAULT_FLAGS);
    g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);
    status = g_application_run(G_APPLICATION(app), argc, argv);
    g_object_unref(app);

    return status;
}