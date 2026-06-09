#include <gtk/gtk.h>
#include "gdk-pixbuf/gdk-pixbuf.h"
#include "glib-object.h"
#include "report.h"

static void on_destroy(GtkWidget *widget, gpointer data) {
    gtk_main_quit();
}

void create_main_window(int argc, char *argv[]) {
    GtkWidget *window;
    GtkWidget *vbox_main;
    GtkWidget *logo_image;
    GtkWidget *hbox_header;
    GdkPixbuf *icon;
    GtkWidget *combo_year;
    GtkWidget *hbox_columns;
    GtkWidget *vbox_sem1;
    GtkWidget *vbox_sem2;
    GtkWidget *vbox_avg;
    GtkWidget *hbox_add_grade;
    GtkWidget *entryName = gtk_entry_new();
    GtkWidget *btn_add = gtk_button_new_with_label("+");
    gtk_init(&argc, &argv);

    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "ETML Grades");
    gtk_window_set_default_size(GTK_WINDOW(window), 1440, 1024);
    g_signal_connect(window, "destroy", G_CALLBACK(on_destroy), NULL);

    icon = gdk_pixbuf_new_from_file("favicon.ico", NULL);
    if (icon != NULL) {
        gtk_window_set_icon(GTK_WINDOW(window), icon);

        g_object_unref(icon);
    }

    vbox_main = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_container_set_border_width(GTK_CONTAINER(vbox_main), 15);
    gtk_container_add(GTK_CONTAINER(window), vbox_main);

    hbox_header = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 20);

    logo_image = gtk_image_new_from_file("ETML-Grades.png");

    combo_year = gtk_combo_box_text_new();
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(combo_year), "Année 1");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(combo_year), "Année 2");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(combo_year), "Année 3");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(combo_year), "Année 4");
    gtk_combo_box_set_active(GTK_COMBO_BOX(combo_year), 0);

    gtk_widget_set_valign(combo_year, GTK_ALIGN_CENTER);

    gtk_box_pack_start(GTK_BOX(hbox_header), logo_image, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(hbox_header), combo_year, FALSE, FALSE, 0);

    gtk_box_pack_start(GTK_BOX(vbox_main), hbox_header, FALSE, FALSE, 0);

    hbox_columns = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 15);

    vbox_sem1 = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    vbox_sem2 = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    vbox_avg = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);

    gtk_box_pack_start(GTK_BOX(vbox_sem1), gtk_label_new("Semester 1"), FALSE, FALSE, 0);
    hbox_add_grade = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 15);
    gtk_entry_set_placeholder_text(GTK_ENTRY(entryName), "Name");
    gtk_box_pack_start(GTK_BOX(vbox_sem2), gtk_label_new("Semester 2"), FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(vbox_avg), gtk_label_new("Annual Avg"), FALSE, FALSE, 0);

    gtk_box_pack_start(GTK_BOX(hbox_columns), vbox_sem1, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(hbox_columns), vbox_sem2, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(hbox_columns), vbox_avg, TRUE, TRUE, 0);

    gtk_box_pack_start(GTK_BOX(vbox_main), hbox_columns, TRUE, TRUE, 0);

    gtk_widget_show_all(window);
    gtk_main();
}
