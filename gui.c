#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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
    GtkWidget *vboxAvg;
    GtkWidget *vboxListSem2;
    GtkWidget *btnCloseSem;
    GtkWidget *hboxAddGrade;
    GtkWidget *vboxSem2;
} GradeForm;

extern Subject maths;
extern Subject english;
extern Subject ecg;
extern Subject infoI;
extern Subject infoC;
extern Subject maths2;
extern Subject english2;
extern Subject ecg2;
extern Subject infoI2;
extern Subject infoC2;

static int currentSem = 1;

void initSubjects(void);
Subject *getSubjectByName(const char *name, int sem);

static void on_destroy(GtkWidget *widget, gpointer data) {
    gtk_main_quit();
}

void on_weightToggled(GtkWidget *checkWidget, gpointer data) {
    gboolean isChecked = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(checkWidget));
    GtkWidget *entry = GTK_WIDGET(data);

    if (isChecked) {
        gtk_widget_set_visible(entry, TRUE);
    } else {
        gtk_widget_set_visible(entry, FALSE);
    }
}

static const gchar *STYLE_CSS =
    /* Fenêtre principale */
    "window {\n"
    "    background-color: #d4d0c8;\n"
    "}\n"

    /* Titres de colonnes */
    ".column-title {\n"
    "    font-size: 15pt;\n"
    "    font-weight: bold;\n"
    "    color: #000000;\n"
    "    margin-bottom: 6px;\n"
    "}\n"

    /* Titres de sections (text-transform supprimé — non supporté GTK3) */
    ".section-title {\n"
    "    font-size: 11pt;\n"
    "    font-weight: bold;\n"
    "    color: #404040;\n"
    "    margin-top: 8px;\n"
    "    margin-bottom: 2px;\n"
    "}\n"

    /* Lignes de notes */
    ".grade-row {\n"
    "    background-color: #ffffff;\n"
    "    border: 1px solid #808080;\n"
    "    padding: 4px 8px;\n"
    "    margin-bottom: 2px;\n"
    "}\n"

    /* Label nom de module */
    ".mod-label {\n"
    "    font-weight: bold;\n"
    "    color: #000000;\n"
    "    font-size: 11pt;\n"
    "}\n"

    /* Label coefficient */
    ".co-label {\n"
    "    color: #555555;\n"
    "    font-size: 10pt;\n"
    "}\n"

    /* Badge valeur de note */
    ".val-label {\n"
    "    font-weight: bold;\n"
    "    font-size: 11pt;\n"
    "    padding: 2px 6px;\n"
    "    border: 1px solid #808080;\n"
    "}\n"

    /* Note réussie */
    ".grade-pass {\n"
    "    background-color: #e6ffe6;\n"
    "    color: #006600;\n"
    "}\n"

    /* Note échouée */
    ".grade-fail {\n"
    "    background-color: #ffe6e6;\n"
    "    color: #cc0000;\n"
    "}\n"

    /* Moyenne de module */
    ".module-avg {\n"
    "    font-size: 11pt;\n"
    "    font-weight: bold;\n"
    "    color: #333333;\n"
    "    margin-left: 4px;\n"
    "}\n"

    /* Moyenne de matière */
    ".subject-avg {\n"
    "    font-weight: bold;\n"
    "    font-size: 11pt;\n"
    "    color: #000000;\n"
    "    background-color: #e4e4e4;\n"
    "    border: 1px solid #7f9db9;\n"
    "    padding: 4px 8px;\n"
    "}\n"

    /* Moyenne générale (linear-gradient non supporté GTK3 → couleur plate) */
    ".general-avg {\n"
    "    background-color: #0040b0;\n"
    "    color: #ffffff;\n"
    "    font-weight: bold;\n"
    "    font-size: 13pt;\n"
    "    border: 1px solid #002c80;\n"
    "    padding: 6px 10px;\n"
    "}\n"

    /* Champs de saisie */
    "entry {\n"
    "    background-color: #ffffff;\n"
    "    border: 1px solid #7f9db9;\n"
    "    padding: 2px 4px;\n"
    "    color: #000000;\n"
    "    font-size: 11pt;\n"
    "}\n"
    "entry:focus {\n"
    "    border-color: #0055ea;\n"
    "}\n"

    /* Bouton ajouter (linear-gradient supprimé → couleur plate + hover manuel) */
    ".btn-add {\n"
    "    background-color: #ececec;\n"
    "    color: #000000;\n"
    "    font-size: 11pt;\n"
    "    font-weight: bold;\n"
    "    border: 1px solid #707070;\n"
    "    padding: 3px 12px;\n"
    "}\n"
    ".btn-add:hover {\n"
    "    background-color: #bee6fd;\n"
    "    border-color: #3c7fb1;\n"
    "}\n"
    ".btn-add:active {\n"
    "    background-color: #98d1ef;\n"
    "    border-color: #2c628b;\n"
    "}\n"

    /* Bouton clôturer */
    ".btn-close {\n"
    "    background-color: #ececec;\n"
    "    color: #a00000;\n"
    "    font-size: 11pt;\n"
    "    font-weight: bold;\n"
    "    border: 1px solid #707070;\n"
    "    padding: 3px 10px;\n"
    "}\n"
    ".btn-close:hover {\n"
    "    background-color: #fdbebd;\n"
    "    border-color: #b13c3c;\n"
    "}\n";

static void applyStyle(void) {
    GtkCssProvider *provider = gtk_css_provider_new();
    GError *error = NULL;

    if (!gtk_css_provider_load_from_data(provider, STYLE_CSS, -1, &error)) {
        g_warning("CSS load failed: %s", error->message);
        g_error_free(error);
        g_object_unref(provider);
        return;
    }

    gtk_style_context_add_provider_for_screen(
        gdk_screen_get_default(),
        GTK_STYLE_PROVIDER(provider),
        GTK_STYLE_PROVIDER_PRIORITY_APPLICATION
    );
    g_object_unref(provider);
}

void refreshUI(GtkWidget *container, int sem) {
    GList *ChildList = gtk_container_get_children(GTK_CONTAINER(container));
    GList *ptr = ChildList;
    while (ptr != NULL) {
        gtk_widget_destroy(ptr->data);
        ptr = ptr->next;
    }
    g_list_free(ChildList);

    Subject *m, *e, *ec, *iI, *iC;
    if (sem == 1) {
        m = &maths; e = &english; ec = &ecg; iI = &infoI; iC = &infoC;
    } else {
        m = &maths2; e = &english2; ec = &ecg2; iI = &infoI2; iC = &infoC2;
    }
    Subject *subjects[] = {m, e, iI, iC, ec};
    int nbSubjects = sizeof(subjects) / sizeof(subjects[0]);

    for (int s = 0; s < nbSubjects; s++) {
        Subject *subject = subjects[s];

        if (subject == m) {
            GtkWidget *title = gtk_label_new(NULL);
            gtk_label_set_markup(GTK_LABEL(title), "<span size='x-large'><b>CBE</b></span>");
            gtk_widget_set_halign(title, GTK_ALIGN_START);
            gtk_style_context_add_class(gtk_widget_get_style_context(title), "section-title");
            gtk_box_pack_start(GTK_BOX(container), title, FALSE, FALSE, 10);
        }
        if (subject == iI) {
            GtkWidget *title = gtk_label_new(NULL);
            gtk_label_set_markup(GTK_LABEL(title), "<span size='x-large'><b>I</b></span>");
            gtk_widget_set_halign(title, GTK_ALIGN_START);
            gtk_style_context_add_class(gtk_widget_get_style_context(title), "section-title");
            gtk_box_pack_start(GTK_BOX(container), title, FALSE, FALSE, 10);
        }
        if (subject == iC) {
            GtkWidget *title = gtk_label_new(NULL);
            gtk_label_set_markup(GTK_LABEL(title), "<span size='x-large'><b>C</b></span>");
            gtk_widget_set_halign(title, GTK_ALIGN_START);
            gtk_style_context_add_class(gtk_widget_get_style_context(title), "section-title");
            gtk_box_pack_start(GTK_BOX(container), title, FALSE, FALSE, 10);
        }
        if (subject == ec) {
            GtkWidget *title = gtk_label_new(NULL);
            gtk_label_set_markup(GTK_LABEL(title), "<span size='x-large'><b>ECG</b></span>");
            gtk_widget_set_halign(title, GTK_ALIGN_START);
            gtk_style_context_add_class(gtk_widget_get_style_context(title), "section-title");
            gtk_box_pack_start(GTK_BOX(container), title, FALSE, FALSE, 10);
        }

        if (!(subject->type == 'I' || subject->type == 'C')) {
            for (int i = 0; i < subject->size; i++) {
                GtkWidget *row = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 15);
                const gchar *name = subject->name;
                gchar *value = g_strdup_printf("%0.1f", subject->grades[i].value);
                gchar *coef = g_strdup_printf("%0.1f", subject->grades[i].coef);

                GtkWidget *labelName = gtk_label_new(name);
                GtkWidget *labelValue = gtk_label_new(value);
                GtkWidget *labelCoef = gtk_label_new(coef);
                gtk_label_set_width_chars(GTK_LABEL(labelName), 10);
                gtk_widget_set_halign(labelValue, GTK_ALIGN_END);
                gtk_label_set_width_chars(GTK_LABEL(labelValue), 6);
                gtk_widget_set_halign(labelCoef, GTK_ALIGN_END);
                gtk_label_set_width_chars(GTK_LABEL(labelCoef), 6);

                gtk_style_context_add_class(gtk_widget_get_style_context(row), "grade-row");
                gtk_style_context_add_class(gtk_widget_get_style_context(labelName), "mod-label");
                gtk_style_context_add_class(gtk_widget_get_style_context(labelCoef), "co-label");

                GtkStyleContext *valContext = gtk_widget_get_style_context(labelValue);
                gtk_style_context_add_class(valContext, "val-label");
                if (subject->grades[i].value >= 4.0f) {
                    gtk_style_context_add_class(valContext, "grade-pass");
                } else {
                    gtk_style_context_add_class(valContext, "grade-fail");
                }

                gtk_box_pack_start(GTK_BOX(row), labelName, FALSE, FALSE, 15);
                gtk_box_pack_start(GTK_BOX(row), labelValue, FALSE, FALSE, 15);
                gtk_box_pack_start(GTK_BOX(row), labelCoef, FALSE, FALSE, 15);
                g_free(value);
                g_free(coef);

                gtk_container_add(GTK_CONTAINER(container), row);
            }
        }

        if (subject == m) {
            float avg = Avg(subject);
            gchar *avgMath = g_strdup_printf("Moyenne Maths: %0.1f", avg);
            GtkWidget *labelavgmath = gtk_label_new(avgMath);
            gtk_style_context_add_class(gtk_widget_get_style_context(labelavgmath), "subject-avg");
            gtk_box_pack_start(GTK_BOX(container), labelavgmath, FALSE, FALSE, 15);
            g_free(avgMath);
        }
        if (subject == e) {
            float avg = Avg(subject);
            gchar *avgAng = g_strdup_printf("Moyenne Anglais: %0.1f", avg);
            GtkWidget *labelavgAng = gtk_label_new(avgAng);
            gtk_style_context_add_class(gtk_widget_get_style_context(labelavgAng), "subject-avg");
            gtk_box_pack_start(GTK_BOX(container), labelavgAng, FALSE, FALSE, 15);
            g_free(avgAng);
            float cbe = AvgCBE(m, e);
            gchar *cbeStr = g_strdup_printf("Moyenne CBE: %0.1f", cbe);
            GtkWidget *labelCBE = gtk_label_new(cbeStr);
            gtk_style_context_add_class(gtk_widget_get_style_context(labelCBE), "subject-avg");
            gtk_box_pack_start(GTK_BOX(container), labelCBE, FALSE, FALSE, 15);
            g_free(cbeStr);
        }

        if (subject->type == 'I' || subject->type == 'C') {
            if (subject->size > 0) {
                int nModules = 0;
                char **modules = NULL;

                for (int i = 0; i < subject->size; i++) {
                    if (subject->grades[i].module == NULL) continue;
                    int found = 0;
                    for (int m_idx = 0; m_idx < nModules; m_idx++) {
                        if (strcmp(modules[m_idx], subject->grades[i].module) == 0) {
                            found = 1;
                            break;
                        }
                    }
                    if (!found) {
                        modules = realloc(modules, (nModules + 1) * sizeof(char *));
                        modules[nModules] = subject->grades[i].module;
                        nModules++;
                    }
                }

                for (int m_idx = 0; m_idx < nModules; m_idx++) {
                    for (int i = 0; i < subject->size; i++) {
                        if (subject->grades[i].module != NULL &&
                            strcmp(subject->grades[i].module, modules[m_idx]) == 0) {
                            GtkWidget *row = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 15);
                            gchar *value = g_strdup_printf("%0.1f", subject->grades[i].value);
                            gchar *coef = g_strdup_printf("%0.1f", subject->grades[i].coef);

                            GtkWidget *labelMod = gtk_label_new(modules[m_idx]);
                            GtkWidget *labelValue = gtk_label_new(value);
                            GtkWidget *labelCoef = gtk_label_new(coef);
                            gtk_label_set_width_chars(GTK_LABEL(labelMod), 10);
                            gtk_widget_set_halign(labelValue, GTK_ALIGN_END);
                            gtk_label_set_width_chars(GTK_LABEL(labelValue), 6);
                            gtk_widget_set_halign(labelCoef, GTK_ALIGN_END);
                            gtk_label_set_width_chars(GTK_LABEL(labelCoef), 6);

                            gtk_style_context_add_class(gtk_widget_get_style_context(row), "grade-row");
                            gtk_style_context_add_class(gtk_widget_get_style_context(labelMod), "mod-label");
                            gtk_style_context_add_class(gtk_widget_get_style_context(labelCoef), "co-label");

                            GtkStyleContext *valContext = gtk_widget_get_style_context(labelValue);
                            gtk_style_context_add_class(valContext, "val-label");
                            if (subject->grades[i].value >= 4.0f) {
                                gtk_style_context_add_class(valContext, "grade-pass");
                            } else {
                                gtk_style_context_add_class(valContext, "grade-fail");
                            }

                            gtk_box_pack_start(GTK_BOX(row), labelMod, FALSE, FALSE, 15);
                            gtk_box_pack_start(GTK_BOX(row), labelValue, FALSE, FALSE, 15);
                            gtk_box_pack_start(GTK_BOX(row), labelCoef, FALSE, FALSE, 15);
                            g_free(value);
                            g_free(coef);

                            gtk_container_add(GTK_CONTAINER(container), row);
                        }
                    }
                    float modAvg = AvgModule(subject, modules[m_idx]);
                    gchar *avgStr = g_strdup_printf("Moyenne du module %s : %.1f", modules[m_idx], modAvg);
                    GtkWidget *labelModAvg = gtk_label_new(avgStr);
                    gtk_style_context_add_class(gtk_widget_get_style_context(labelModAvg), "module-avg");
                    gtk_box_pack_start(GTK_BOX(container), labelModAvg, FALSE, FALSE, 15);
                    g_free(avgStr);
                }

                free(modules);

                float overall = AvgOfModules(subject);
                gchar *overallStr;
                if (subject->type == 'I') {
                    overallStr = g_strdup_printf("Moyenne I : %.1f", overall);
                } else {
                    overallStr = g_strdup_printf("Moyenne C : %.1f", overall);
                }
                GtkWidget *labelOverall = gtk_label_new(overallStr);
                gtk_style_context_add_class(gtk_widget_get_style_context(labelOverall), "subject-avg");
                gtk_box_pack_start(GTK_BOX(container), labelOverall, FALSE, FALSE, 15);
                g_free(overallStr);
            }
        }

        if (subject == iC) {
            float infoAvg = AvgInformatique(iI, iC);
            gchar *infoStr = g_strdup_printf("Moyenne Informatique: %0.1f", infoAvg);
            GtkWidget *labelInfo = gtk_label_new(infoStr);
            gtk_style_context_add_class(gtk_widget_get_style_context(labelInfo), "subject-avg");
            gtk_box_pack_start(GTK_BOX(container), labelInfo, FALSE, FALSE, 15);
            g_free(infoStr);
        }

        if (subject == ec) {
            float avg = Avg(subject);
            gchar *avgECG = g_strdup_printf("Moyenne ECG: %0.1f", avg);
            GtkWidget *labelavgecg = gtk_label_new(avgECG);
            gtk_style_context_add_class(gtk_widget_get_style_context(labelavgecg), "subject-avg");
            gtk_box_pack_start(GTK_BOX(container), labelavgecg, FALSE, FALSE, 15);
            g_free(avgECG);
            float avgGeneral = AvgGeneral(AvgCBE(m, e), AvgInformatique(iI, iC), avg);
            gchar *generalStr = g_strdup_printf("Moyenne Générale: %0.1f", avgGeneral);
            GtkWidget *labelGeneral = gtk_label_new(generalStr);
            gtk_style_context_add_class(gtk_widget_get_style_context(labelGeneral), "general-avg");
            gtk_box_pack_start(GTK_BOX(container), labelGeneral, FALSE, FALSE, 15);
            g_free(generalStr);
        }
    }

    gtk_widget_show_all(container);
}

void on_closeSem1(GtkWidget *button, gpointer data) {
    GradeForm *form = data;
    gint result;

    if (currentSem == 1) {
        GtkWidget *dialog = gtk_message_dialog_new(
            GTK_WINDOW(gtk_widget_get_toplevel(button)),
            GTK_DIALOG_MODAL,
            GTK_MESSAGE_QUESTION,
            GTK_BUTTONS_YES_NO,
            "Fermer le semestre 1 ?\nVous pourrez ensuite saisir les notes du semestre 2."
        );
        result = gtk_dialog_run(GTK_DIALOG(dialog));
        gtk_widget_destroy(dialog);

        if (result == GTK_RESPONSE_YES) {
            currentSem = 2;

            g_object_ref(form->hboxAddGrade);
            gtk_container_remove(GTK_CONTAINER(gtk_widget_get_parent(form->hboxAddGrade)), form->hboxAddGrade);
            gtk_box_pack_start(GTK_BOX(form->vboxSem2), form->hboxAddGrade, FALSE, FALSE, 0);
            gtk_box_reorder_child(GTK_BOX(form->vboxSem2), form->hboxAddGrade, 1);
            g_object_unref(form->hboxAddGrade);

            g_object_ref(form->btnCloseSem);
            gtk_container_remove(GTK_CONTAINER(gtk_widget_get_parent(form->btnCloseSem)), form->btnCloseSem);
            gtk_box_pack_start(GTK_BOX(form->vboxSem2), form->btnCloseSem, FALSE, FALSE, 5);
            gtk_box_reorder_child(GTK_BOX(form->vboxSem2), form->btnCloseSem, 2);
            g_object_unref(form->btnCloseSem);

            gtk_button_set_label(GTK_BUTTON(form->btnCloseSem), "Clôturer le semestre 2");

            refreshUI(form->vboxListSem2, 2);
            gtk_widget_show_all(form->vboxSem2);
        }
    } else {
        GtkWidget *dialog = gtk_message_dialog_new(
            GTK_WINDOW(gtk_widget_get_toplevel(button)),
            GTK_DIALOG_MODAL,
            GTK_MESSAGE_QUESTION,
            GTK_BUTTONS_YES_NO,
            "Fermer le semestre 2 ?\nVous ne pourrez plus ajouter de notes."
        );
        result = gtk_dialog_run(GTK_DIALOG(dialog));
        gtk_widget_destroy(dialog);

        if (result == GTK_RESPONSE_YES) {
            gtk_widget_set_visible(form->hboxAddGrade, FALSE);
            gtk_widget_set_visible(form->btnCloseSem, FALSE);
        }
    }
}

void on_addGrade(GtkWidget *button, gpointer data) {
    GradeForm *form = data;
    const gchar *module = gtk_entry_get_text(GTK_ENTRY(form->entryName));
    gchar *type = gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(form->combo_nature));
    Subject *subject = getSubjectByName(type, currentSem);
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
    refreshUI(form->vboxList, 1);
    refreshUI(form->vboxListSem2, 2);

    GList *avgChildren = gtk_container_get_children(GTK_CONTAINER(form->vboxAvg));
    GList *ptrAvg = avgChildren;
    while (ptrAvg != NULL) {
        gtk_widget_destroy(ptrAvg->data);
        ptrAvg = ptrAvg->next;
    }
    g_list_free(avgChildren);

    GtkWidget *avgHeader = gtk_label_new("Moyenne annuelle");
    gtk_style_context_add_class(gtk_widget_get_style_context(avgHeader), "column-title");
    gtk_box_pack_start(GTK_BOX(form->vboxAvg), avgHeader, FALSE, FALSE, 0);

    float cbe1 = AvgCBE(&maths, &english);
    float cbe2 = AvgCBE(&maths2, &english2);
    float cbeYear = round05((cbe1 + cbe2) / 2.0f);
    float info1 = AvgInformatique(&infoI, &infoC);
    float info2 = AvgInformatique(&infoI2, &infoC2);
    float infoYear = round05((info1 + info2) / 2.0f);
    float ecg1 = Avg(&ecg);
    float ecg2_val = Avg(&ecg2);
    float ecgYear = round05((ecg1 + ecg2_val) / 2.0f);
    float generalYear = AvgGeneral(cbeYear, infoYear, ecgYear);

    gchar *cbeStr = g_strdup_printf("Moyenne CBE annuelle: %.1f", cbeYear);
    GtkWidget *cbeLabel = gtk_label_new(cbeStr);
    gtk_style_context_add_class(gtk_widget_get_style_context(cbeLabel), "subject-avg");
    gtk_box_pack_start(GTK_BOX(form->vboxAvg), cbeLabel, FALSE, FALSE, 15);
    g_free(cbeStr);

    gchar *infoStr = g_strdup_printf("Moyenne Informatique annuelle: %.1f", infoYear);
    GtkWidget *infoLabel = gtk_label_new(infoStr);
    gtk_style_context_add_class(gtk_widget_get_style_context(infoLabel), "subject-avg");
    gtk_box_pack_start(GTK_BOX(form->vboxAvg), infoLabel, FALSE, FALSE, 15);
    g_free(infoStr);

    gchar *ecgStr = g_strdup_printf("Moyenne ECG annuelle: %.1f", ecgYear);
    GtkWidget *ecgLabel = gtk_label_new(ecgStr);
    gtk_style_context_add_class(gtk_widget_get_style_context(ecgLabel), "subject-avg");
    gtk_box_pack_start(GTK_BOX(form->vboxAvg), ecgLabel, FALSE, FALSE, 15);
    g_free(ecgStr);

    gchar *generalStr = g_strdup_printf("Moyenne Générale annuelle: %.1f", generalYear);
    GtkWidget *generalLabel = gtk_label_new(generalStr);
    gtk_style_context_add_class(gtk_widget_get_style_context(generalLabel), "general-avg");
    gtk_box_pack_start(GTK_BOX(form->vboxAvg), generalLabel, FALSE, FALSE, 15);
    g_free(generalStr);

    gtk_widget_show_all(form->vboxAvg);
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
    GtkWidget *scroll_sem2;
    static GradeForm Sem1Form;

    gtk_init(&argc, &argv);
    applyStyle();

    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "ETML Grades");
    gtk_window_set_default_size(GTK_WINDOW(window), 1440, 1024);
    g_signal_connect(window, "destroy", G_CALLBACK(on_destroy), NULL);

    icon = gdk_pixbuf_new_from_file("../img/favicon.ico", NULL);
    if (icon != NULL) {
        gtk_window_set_icon(GTK_WINDOW(window), icon);
        g_object_unref(icon);
    }

    vbox_main = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_container_set_border_width(GTK_CONTAINER(vbox_main), 15);
    gtk_container_add(GTK_CONTAINER(window), vbox_main);

    hbox_header = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 20);
    GtkWidget *link = gtk_link_button_new("https://github.com/SHR-Zidane");
    gtk_button_set_label(GTK_BUTTON(link), "Made by SHR_Zidane");
    logo_image = gtk_image_new_from_file("./img/ETML-Grades.png");
    combo_year = gtk_combo_box_text_new();
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(combo_year), "Année 1");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(combo_year), "Année 2");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(combo_year), "Année 3");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(combo_year), "Année 4");
    gtk_combo_box_set_active(GTK_COMBO_BOX(combo_year), 0);
    gtk_widget_set_valign(combo_year, GTK_ALIGN_CENTER);

    gtk_box_pack_start(GTK_BOX(hbox_header), logo_image, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(hbox_header), combo_year, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(hbox_header), link, FALSE, FALSE, 0);
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
    gtk_style_context_add_class(gtk_widget_get_style_context(btn_add), "btn-add");

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
    Sem1Form.vboxAvg = vbox_avg;
    Sem1Form.vboxListSem2 = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    Sem1Form.hboxAddGrade = hbox_add_grade;
    Sem1Form.vboxSem2 = vbox_sem2;

    gtk_container_add(GTK_CONTAINER(scroll_sem1), Sem1Form.vboxList);

    scroll_sem2 = gtk_scrolled_window_new(NULL, NULL);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scroll_sem2), GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
    gtk_container_add(GTK_CONTAINER(scroll_sem2), Sem1Form.vboxListSem2);

    GtkWidget *btnCloseSem = gtk_button_new_with_label("Clôturer le semestre 1");
    Sem1Form.btnCloseSem = btnCloseSem;
    gtk_style_context_add_class(gtk_widget_get_style_context(btnCloseSem), "btn-close");
    g_signal_connect(btnCloseSem, "clicked", G_CALLBACK(on_closeSem1), &Sem1Form);

    GtkWidget *sem1Title = gtk_label_new("Semestre 1");
    gtk_style_context_add_class(gtk_widget_get_style_context(sem1Title), "column-title");
    gtk_box_pack_start(GTK_BOX(vbox_sem1), sem1Title, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(vbox_sem1), hbox_add_grade, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(vbox_sem1), btnCloseSem, FALSE, FALSE, 5);
    gtk_box_pack_start(GTK_BOX(vbox_sem1), scroll_sem1, TRUE, TRUE, 0);

    GtkWidget *sem2Title = gtk_label_new("Semestre 2");
    gtk_style_context_add_class(gtk_widget_get_style_context(sem2Title), "column-title");
    gtk_box_pack_start(GTK_BOX(vbox_sem2), sem2Title, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(vbox_sem2), scroll_sem2, TRUE, TRUE, 0);

    gtk_box_pack_start(GTK_BOX(hbox_columns), vbox_sem1, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(hbox_columns), vbox_sem2, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(hbox_columns), vbox_avg, TRUE, TRUE, 0);

    gtk_box_pack_start(GTK_BOX(vbox_main), hbox_columns, TRUE, TRUE, 0);

    gtk_widget_show_all(window);
    gtk_main();
}
