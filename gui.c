#include <gtk/gtk.h>
#include "gdk-pixbuf/gdk-pixbuf.h"
#include "glib-object.h"
#include "glib.h"
#include "glibconfig.h"
#include "report.h"

typedef struct {
    GtkWidget *combo_nature;
    GtkWidget *entryName;
    GtkWidget *entryGrade;
    GtkWidget *checkWeight;
    GtkWidget *entryWeight;
    GtkWidget *vboxList;
} GradeForm;

static void on_destroy(GtkWidget *widget, gpointer data) {
    gtk_main_quit();
}

void on_weightToggled(GtkWidget *checkWidget, gpointer data) {
    //récupération de l'état du checkbutton (si il est coché ou pas)
    gboolean isChecked = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(checkWidget));
    //conversion de data en widget
    GtkWidget *entry = GTK_WIDGET(data);

    if (isChecked) {
        gtk_widget_set_visible(entry, TRUE);
    }
    else {
        gtk_widget_set_visible(entry, FALSE);
    }
}

void on_addGrade(GtkWidget *button, gpointer data){
    GradeForm *form = data;
    const gchar *Name = gtk_entry_get_text(GTK_ENTRY(form->entryName));
    const gchar *Grade = gtk_entry_get_text(GTK_ENTRY(form->entryGrade));
    gchar *Nature = gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(form->combo_nature));
    gboolean isChecked = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(form->checkWeight));
    GtkWidget *hbox_Grade;

    const gchar *Weight = gtk_entry_get_text(GTK_ENTRY(form->entryWeight));
    hbox_Grade = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 15);
    gtk_box_pack_start(GTK_BOX(form->vboxList), hbox_Grade, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(hbox_Grade), gtk_label_new(Name), FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(hbox_Grade), gtk_label_new(Nature), FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(hbox_Grade), gtk_label_new(Grade), FALSE, FALSE, 0);
    if (isChecked) {
        gtk_box_pack_start(GTK_BOX(hbox_Grade), gtk_label_new(Weight), FALSE, FALSE, 0);
    }
    gtk_widget_show_all(hbox_Grade);
    gtk_entry_set_text(GTK_ENTRY(form->entryName), "");
    gtk_entry_set_text(GTK_ENTRY(form->entryGrade), "");
    gtk_entry_set_text(GTK_ENTRY(form->entryWeight), "");
    gtk_combo_box_set_active(GTK_COMBO_BOX(form->combo_nature), 0);
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(form->checkWeight), FALSE);
    g_free(Nature);
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
    GtkWidget *btn_add;
    GtkWidget *scroll_sem1;
    static GradeForm Sem1Form;

    gtk_init(&argc, &argv);

    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "ETML Grades");
    gtk_window_set_default_size(GTK_WINDOW(window), 1440, 1024);
    g_signal_connect(window, "destroy", G_CALLBACK(on_destroy), NULL);

    icon = gdk_pixbuf_new_from_file("./img/favicon.ico", NULL);
    if (icon != NULL) {
        gtk_window_set_icon(GTK_WINDOW(window), icon);

        g_object_unref(icon);
    }

    vbox_main = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_container_set_border_width(GTK_CONTAINER(vbox_main), 15);
    gtk_container_add(GTK_CONTAINER(window), vbox_main);

    hbox_header = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 20);

    logo_image = gtk_image_new_from_file("./img/ETML-Grades.png");
    // déclaration menu déroulant
    combo_year = gtk_combo_box_text_new();
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(combo_year), "Année 1");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(combo_year), "Année 2");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(combo_year), "Année 3");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(combo_year), "Année 4");
    gtk_combo_box_set_active(GTK_COMBO_BOX(combo_year), 0);
    // Gérer la taille du menu déroulant
    gtk_widget_set_valign(combo_year, GTK_ALIGN_CENTER);
    // Section header haut de page
    gtk_box_pack_start(GTK_BOX(hbox_header), logo_image, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(hbox_header), combo_year, FALSE, FALSE, 0);
    // Section page principale
    gtk_box_pack_start(GTK_BOX(vbox_main), hbox_header, FALSE, FALSE, 0);

    hbox_columns = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 15);

    vbox_sem1 = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    vbox_sem2 = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    vbox_avg = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);

    Sem1Form.entryName = gtk_entry_new();
    Sem1Form.entryGrade = gtk_entry_new();
    Sem1Form.checkWeight = gtk_check_button_new_with_label("Pondéré ?:");
    Sem1Form.entryWeight = gtk_entry_new();
    btn_add = gtk_button_new_with_label("+");

    gtk_entry_set_placeholder_text(GTK_ENTRY(Sem1Form.entryName), "Name");
    gtk_entry_set_placeholder_text(GTK_ENTRY(Sem1Form.entryGrade), "Note");
    gtk_entry_set_placeholder_text(GTK_ENTRY(Sem1Form.entryWeight), "%");

    gtk_entry_set_width_chars(GTK_ENTRY(Sem1Form.entryName), 10);
    gtk_entry_set_width_chars(GTK_ENTRY(Sem1Form.entryWeight), 4);

    hbox_add_grade = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 15);

    Sem1Form.combo_nature = gtk_combo_box_text_new();
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(Sem1Form.combo_nature), "C");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(Sem1Form.combo_nature), "I");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(Sem1Form.combo_nature), "Maths");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(Sem1Form.combo_nature), "Anglais");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(Sem1Form.combo_nature), "ECG");
    gtk_combo_box_set_active(GTK_COMBO_BOX(Sem1Form.combo_nature), 0);

    gtk_box_pack_start(GTK_BOX(hbox_add_grade), Sem1Form.entryName, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(hbox_add_grade), Sem1Form.combo_nature, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(hbox_add_grade), Sem1Form.entryGrade, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(hbox_add_grade), Sem1Form.checkWeight, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(hbox_add_grade), Sem1Form.entryWeight, FALSE, FALSE, 0);
    gtk_widget_set_visible(Sem1Form.entryWeight, FALSE);
    gtk_widget_set_no_show_all(Sem1Form.entryWeight, TRUE);
    g_signal_connect(Sem1Form.checkWeight, "toggled", G_CALLBACK(on_weightToggled), Sem1Form.entryWeight);
    gtk_box_pack_start(GTK_BOX(hbox_add_grade), btn_add, FALSE, FALSE, 0);
    g_signal_connect(btn_add, "clicked", G_CALLBACK(on_addGrade), &Sem1Form);
    scroll_sem1 = gtk_scrolled_window_new(NULL, NULL);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scroll_sem1), GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);

    Sem1Form.vboxList = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);

    gtk_container_add(GTK_CONTAINER(scroll_sem1), Sem1Form.vboxList);

    // Section Semestre 1
    gtk_box_pack_start(GTK_BOX(vbox_sem1), gtk_label_new("Semester 1"), FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(vbox_sem1), hbox_add_grade, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(vbox_sem1), scroll_sem1, TRUE, TRUE, 0);
    // Section Semestre 2
    gtk_box_pack_start(GTK_BOX(vbox_sem2), gtk_label_new("Semester 2"), FALSE, FALSE, 0);
    // Section Moyenne Annuel
    gtk_box_pack_start(GTK_BOX(vbox_avg), gtk_label_new("Annual Avg"), FALSE, FALSE, 0);

    gtk_box_pack_start(GTK_BOX(hbox_columns), vbox_sem1, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(hbox_columns), vbox_sem2, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(hbox_columns), vbox_avg, TRUE, TRUE, 0);

    gtk_box_pack_start(GTK_BOX(vbox_main), hbox_columns, TRUE, TRUE, 0);

    gtk_widget_show_all(window);
    gtk_main();
}
