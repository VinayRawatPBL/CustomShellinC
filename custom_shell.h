#ifndef CUSTOM_SHELL_H
#define CUSTOM_SHELL_H

#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <time.h>
#include <errno.h>
#include <ctype.h>
#include <glib.h>

#define MAX_HISTORY 50
#define MAX_COMMAND_LENGTH 1000

typedef struct {
    GtkWidget *window;
    GtkWidget *entry;
    GtkTextView *textview;
    GtkTextBuffer *buffer;
    GtkWidget *run_button;
    GtkWidget *clear_button;
    GtkWidget *time_button;
    GtkWidget *theme_button;
    GtkWidget *history_button;
    int theme_index; // 0: light, 1: dark, 2: hacker, 3: solarized
    char *command_history[MAX_HISTORY];
    int history_count;
    int history_index;
    GtkCssProvider *css_provider;
} AppData;

// Function declarations
void execute_command(const char *command, GtkTextBuffer *buffer, GtkTextView *textview);
void apply_css(AppData *app, const char *css);
void cycle_theme(AppData *app);
void add_to_history(AppData *app, const char *command);
void show_history(AppData *app);
void on_run_clicked(GtkButton *button, gpointer user_data);
void on_clear_clicked(GtkButton *button, gpointer user_data);
void on_time_clicked(GtkButton *button, gpointer user_data);
void on_theme_clicked(GtkButton *button, gpointer user_data);
void on_history_clicked(GtkButton *button, gpointer user_data);
gboolean on_entry_key_press(GtkWidget *widget, GdkEventKey *event, gpointer user_data);
void on_window_destroy(GtkWidget *widget, gpointer user_data);
void activate(GtkApplication *app, gpointer user_data);
void destroy_app_data(AppData *app_data);
double calculate_expression(const char *expr); // Calculator function
int precedence(char op);
double apply_operator(double a, double b, char op);

#endif // CUSTOM_SHELL_H