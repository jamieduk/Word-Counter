#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <gtk/gtk.h>

// Function to count words, characters, and spaces in a file
void count_file_stats(const char *filename, int *word_count, int *char_count, int *space_count) {
    FILE *file=fopen(filename, "r");
    if (!file) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    int ch;
    *word_count=0;
    *char_count=0;
    *space_count=0;
    int in_word=0;

    while ((ch=fgetc(file)) != EOF) {
        (*char_count)++;
        if (ch == ' ') {
            (*space_count)++;
        }
        if (ch == '\n' || ch == ' ' || ch == '\t') {
            in_word=0;
        } else {
            if (in_word == 0) {
                (*word_count)++;
                in_word=1;
            }
        }
    }

    fclose(file);
}

void show_stats(const char *filename) {
    int word_count, char_count, space_count;
    count_file_stats(filename, &word_count, &char_count, &space_count);

    // Create a new dialog
    GtkWidget *dialog=gtk_dialog_new_with_buttons("File Statistics",
                                                    NULL,
                                                    GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT,
                                                    "_Close",
                                                    GTK_RESPONSE_CLOSE,
                                                    NULL);
    
    // Create a message string
    char message[256];
    snprintf(message, sizeof(message), "File: %s\nWords: %d\nCharacters: %d\nSpaces: %d",
             filename, word_count, char_count, space_count);

    // Create a label to display the statistics
    GtkWidget *label=gtk_label_new(message);
    gtk_container_add(GTK_CONTAINER(gtk_dialog_get_content_area(GTK_DIALOG(dialog))), label);

    gtk_widget_show_all(dialog);

    // Wait for the user to close the dialog
    g_signal_connect(dialog, "response", G_CALLBACK(gtk_widget_destroy), NULL);
}

void show_about() {
    // Create an "About" dialog
    GtkWidget *about_dialog=gtk_dialog_new_with_buttons("About",
                                                          NULL,
                                                          GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT,
                                                          "_Close",
                                                          GTK_RESPONSE_CLOSE,
                                                          NULL);
    
    // Create a message for the About dialog
    GtkWidget *about_label=gtk_label_new("Author: Jay Mee\n@ J~Net 2024");
    gtk_container_add(GTK_CONTAINER(gtk_dialog_get_content_area(GTK_DIALOG(about_dialog))), about_label);

    gtk_widget_show_all(about_dialog);

    // Wait for the user to close the About dialog
    g_signal_connect(about_dialog, "response", G_CALLBACK(gtk_widget_destroy), NULL);
}

void on_file_selected(GtkFileChooserButton *button) {
    gchar *filename=gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(button));
    show_stats(filename);
    g_free(filename);
}

void on_open_button_clicked(GtkButton *button, GtkFileChooserButton *file_chooser) {
    gchar *filename=gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(file_chooser));
    show_stats(filename);
    g_free(filename);
}

void print_help() {
    printf("Word Counter Usage:\n\n");
    printf("Usage: ./word_counter <filename>\n");
    printf("Counts the number of words, characters, and spaces in a specified text file.\n");
    printf("Example: ./word_counter myfile.txt\n");
}

int main(int argc, char *argv[]) {
    gtk_init(&argc, &argv);

    if (argc == 2) {
        const char *filename=argv[1];
        show_stats(filename);
        return EXIT_SUCCESS;
    } else if (argc > 2) {
        print_help();
        return EXIT_FAILURE;
    }

    // Create GUI
    GtkWidget *window=gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Word Counter");
    gtk_window_set_default_size(GTK_WINDOW(window), 300, 200);
    
    GtkWidget *vbox=gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    gtk_container_add(GTK_CONTAINER(window), vbox);

    GtkWidget *file_chooser=gtk_file_chooser_button_new("Select a file", GTK_FILE_CHOOSER_ACTION_OPEN);
    gtk_box_pack_start(GTK_BOX(vbox), file_chooser, TRUE, TRUE, 0);

    GtkWidget *open_button=gtk_button_new_with_label("Count Words");
    gtk_box_pack_start(GTK_BOX(vbox), open_button, TRUE, TRUE, 0);

    // Create the "About" button
    GtkWidget *about_button=gtk_button_new_with_label("About");
    gtk_box_pack_start(GTK_BOX(vbox), about_button, TRUE, TRUE, 0);

    g_signal_connect(open_button, "clicked", G_CALLBACK(on_open_button_clicked), file_chooser);
    g_signal_connect(about_button, "clicked", G_CALLBACK(show_about), NULL);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    gtk_widget_show_all(window);
    gtk_main();

    return 0;
}

