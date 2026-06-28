#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>
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

extern Subject maths;
extern Subject english;
extern Subject ecg;
extern Subject infoI;
extern Subject infoC;

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

void refreshUI(GtkWidget *container){
    GList *ChildList = gtk_container_get_children(GTK_CONTAINER(container));
    GList *ptr = ChildList;
    while (ptr != NULL){
        gtk_widget_destroy(ptr->data);
        ptr = ptr->next;
    }
    g_list_free(ChildList);

    // Reconstruction
    Subject *subjects[] = {
        &maths,
        &english,
        &infoI,
        &infoC,
        &ecg
    };
    int nbSubjects = sizeof(subjects) / sizeof(subjects[0]);
    // INITIALISATION DE CHAQUE SUJET
    for (int s = 0; s < nbSubjects; s++){
        Subject *subject = subjects[s];

        if (subject == &maths){
            GtkWidget *title = gtk_label_new(NULL);
            gtk_label_set_markup(GTK_LABEL(title), "<span size='x-large'><b>CBE</b></span>");
            gtk_widget_set_halign(title, GTK_ALIGN_START);
            gtk_box_pack_start(
                GTK_BOX(container),
                title,
                FALSE,
                FALSE,
                10
            );
        }
        if (subject == &infoI){
            GtkWidget *title = gtk_label_new(NULL);
            gtk_label_set_markup(GTK_LABEL(title), "<span size='x-large'><b>I</b></span>");
            gtk_widget_set_halign(title, GTK_ALIGN_START);
            gtk_box_pack_start(
                GTK_BOX(container),
                title,
                FALSE,
                FALSE,
                10
            );
        }
        if (subject == &infoC){
            GtkWidget *title = gtk_label_new(NULL);
            gtk_label_set_markup(GTK_LABEL(title), "<span size='x-large'><b>C</b></span>");
            gtk_widget_set_halign(title, GTK_ALIGN_START);
            gtk_box_pack_start(
                GTK_BOX(container),
                title,
                FALSE,
                FALSE,
                10
            );
        }
        if (subject == &ecg){
            GtkWidget *title = gtk_label_new(NULL);
            gtk_label_set_markup(GTK_LABEL(title), "<span size='x-large'><b>ECG</b></span>");
            gtk_widget_set_halign(title, GTK_ALIGN_START);
            gtk_box_pack_start(
                GTK_BOX(container),
                title,
                FALSE,
                FALSE,
                10
            );
        }
        for (int i = 0; i < subject->size; i++){
            //AFFICHAGE DES NOTES
            GtkWidget *row = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 15);
            const gchar *name = subject->name;
            gchar *value = g_strdup_printf("%0.1f", subject->grades[i].value);
            gchar *coef = g_strdup_printf("%0.1f", subject->grades[i].coef);

            GtkWidget *labelName;
            GtkWidget *labelValue;
            GtkWidget *labelCoef;

            labelName = gtk_label_new(name);
            gtk_box_pack_start(GTK_BOX(row), labelName, FALSE, FALSE, 15);
            labelValue = gtk_label_new(value);
            gtk_box_pack_start(GTK_BOX(row), labelValue, FALSE, FALSE, 15);
            g_free(value);
            labelCoef = gtk_label_new(coef);
            gtk_box_pack_start(GTK_BOX(row), labelCoef, FALSE, FALSE, 15);
            g_free(coef);

            gtk_container_add(GTK_CONTAINER(container), row);
        }
        //AFFICHAGE DES MOYENNES
        if (subject == &maths){
        float avg = Avg(subject);
        gchar *avgMath = g_strdup_printf("Moyenne Maths: %0.1f", avg);
        GtkWidget *labelavgmath = gtk_label_new(avgMath);
        gtk_box_pack_start(GTK_BOX(container), labelavgmath, FALSE, FALSE, 15);
        g_free(avgMath);
        }
        if (subject == &english){
        float avg = Avg(subject);
        gchar *avgAng = g_strdup_printf("Moyenne Anglais: %0.1f", avg);
        GtkWidget *labelavgAng = gtk_label_new(avgAng);
        gtk_box_pack_start(GTK_BOX(container), labelavgAng, FALSE, FALSE, 15);
        g_free(avgAng);
        }
        if (subject == &infoI){
            for (int i = 0; i < subject->size; i++) {
                gchar *module = g_strdup(subject->grades[i].module);
                gchar *avgModule = g_strdup_printf("Moyenne Module %s: %0.1f", module, Avg(subject));
                GtkWidget *labelavgModule = gtk_label_new(avgModule);
                gtk_box_pack_start(GTK_BOX(container), labelavgModule, FALSE, FALSE, 15);
                g_free(module);
                g_free(avgModule);
            }
        float avg = Avg(subject);
        gchar *avgI = g_strdup_printf("Moyenne Module I: %0.1f", avg);
        GtkWidget *labelavgI = gtk_label_new(avgI);
        gtk_box_pack_start(GTK_BOX(container), labelavgI, FALSE, FALSE, 15);
        g_free(avgI);
        }
        if (subject == &infoC){
        float avg = Avg(subject);
        gchar *avgC = g_strdup_printf("Moyenne Module C: %0.1f", avg);
        GtkWidget *labelavgC = gtk_label_new(avgC);
        gtk_box_pack_start(GTK_BOX(container), labelavgC, FALSE, FALSE, 15);
        g_free(avgC);
        }
        if (subject == &ecg){
        float avg = Avg(subject);
        gchar *avgECG = g_strdup_printf("Moyenne ECG: %0.1f", avg);
        GtkWidget *labelavgecg = gtk_label_new(avgECG);
        gtk_box_pack_start(GTK_BOX(container), labelavgecg, FALSE, FALSE, 15);
        g_free(avgECG);
        }
    }

    gtk_widget_show_all(container);
}

void on_addGrade(GtkWidget *button, gpointer data){
    GradeForm *form = data;
    const gchar *module = gtk_entry_get_text(GTK_ENTRY(form->entryName));
    gchar *type = gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(form->combo_nature));
    Subject *subject = getSubjectByName(type);
    g_free(type);
    Grade g;
    g.module = g_strdup(module);
    const gchar *gradeText = gtk_entry_get_text(GTK_ENTRY(form->entryGrade));
    g.value = atof(gradeText);
    gboolean isChecked = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(form->checkWeight));

        g.has_coef = isChecked;

        if (isChecked) {
            const gchar *weightText = gtk_entry_get_text(GTK_ENTRY(form->entryWeight));
            g.coef = atof(weightText);
        } else {
            g.coef = 1.0f;
        }

    addGrade(subject, g);
    refreshUI(form->vboxList);
}

void initSubjects(void);
Subject *getSubjectByName(const char *name);

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
