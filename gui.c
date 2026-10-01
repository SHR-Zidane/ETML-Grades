#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "report.h"

typedef struct {
    GtkWidget *cardForm;
    GtkWidget *labelTarget;
    GtkWidget *comboSubject;
    GtkWidget *labelModule;
    GtkWidget *entryModule;
    GtkWidget *entryGrade;
    GtkWidget *checkWeight;
    GtkWidget *revealWeight;
    GtkWidget *entryWeight;
    GtkWidget *btnAdd;
    GtkWidget *labelFeedback;
    GtkWidget *btnCloseSem;

    GtkWidget *stack;
    GtkWidget *pageSem1;
    GtkWidget *pageSem2;
    GtkWidget *vboxList;
    GtkWidget *vboxListSem2;
    GtkWidget *vboxAvg;

    gboolean yearClosed;
} GradeForm;

void initSubjects(void);
Subject *getSubjectByName(const char *name, int sem);

static const gchar *STYLE_CSS =
    "window, .app-body { background-color: #f3f4f6; }\n"

    ".app-header { background-color: #ffffff; border-bottom: 1px solid #e2e4e8; }\n"
    ".app-title { font-size: 18pt; font-weight: 800; color: #111827; }\n"
    ".app-subtitle { font-size: 10pt; color: #6b7280; }\n"

    ".card { background-color: #ffffff; border: 1px solid #e2e4e8; border-radius: 12px; }\n"
    ".card-title { font-size: 14pt; font-weight: 700; color: #111827; }\n"
    ".card-subtitle { font-size: 9pt; color: #6b7280; }\n"
    ".subject-title { font-size: 11.5pt; font-weight: 700; color: #1f2937; }\n"
    ".subgroup { border-left: 2px solid #e5e7eb; }\n"
    ".muted { color: #9ca3af; font-style: italic; }\n"
    ".column-head { font-size: 8.5pt; font-weight: 700; color: #9ca3af; }\n"

    ".field-label { font-weight: 600; color: #374151; font-size: 10.5pt; }\n"
    ".hint { font-size: 9pt; color: #6b7280; }\n"
    ".feedback-ok { font-size: 10pt; color: #15803d; font-weight: 600; }\n"
    ".feedback-error { font-size: 10pt; color: #b91c1c; font-weight: 600; }\n"
    ".target-badge { background-color: #e8eefb; color: #1d4ed8; border-radius: 12px;"
    "  padding: 3px 10px; font-weight: 700; font-size: 9.5pt; }\n"
    ".target-badge.closed { background-color: #f3f4f6; color: #6b7280; }\n"

    ".form entry { min-height: 34px; padding: 0 10px; border-radius: 8px;"
    "  border: 1px solid #d1d5db; background-color: #ffffff; background-image: none;"
    "  box-shadow: none; color: #111827; font-size: 11pt; }\n"
    ".form entry:focus { border-color: #2563eb; box-shadow: inset 0 0 0 1px #2563eb; }\n"
    ".form entry.invalid { border-color: #dc2626; box-shadow: inset 0 0 0 1px #dc2626; }\n"
    ".form entry:disabled { background-color: #f3f4f6; color: #9ca3af; }\n"
    ".form combobox button { min-height: 34px; border-radius: 8px; border: 1px solid #d1d5db;"
    "  background-color: #ffffff; background-image: none; box-shadow: none; color: #111827; }\n"
    ".form checkbutton label { color: #374151; }\n"

    "button.btn-primary { min-height: 42px; border-radius: 8px; border: none;"
    "  background-color: #2563eb; background-image: none; box-shadow: none;"
    "  color: #ffffff; font-weight: 700; font-size: 11.5pt; text-shadow: none; }\n"
    "button.btn-primary label { color: #ffffff; }\n"
    "button.btn-primary:hover { background-color: #1d4ed8; }\n"
    "button.btn-primary:active { background-color: #1e40af; }\n"
    "button.btn-primary:disabled { background-color: #bfcdf5; }\n"
    "button.btn-primary:disabled label { color: #ffffff; }\n"

    "button.btn-danger { min-height: 36px; border-radius: 8px; border: 1px solid #f1c4c4;"
    "  background-color: #ffffff; background-image: none; box-shadow: none;"
    "  color: #b91c1c; font-weight: 600; text-shadow: none; }\n"
    "button.btn-danger label { color: #b91c1c; }\n"
    "button.btn-danger:hover { background-color: #fdecec; border-color: #e8a1a1; }\n"

    "stackswitcher button { min-height: 34px; padding: 0 18px; background-image: none;"
    "  background-color: #ffffff; border: 1px solid #d1d5db; box-shadow: none;"
    "  color: #374151; font-weight: 600; text-shadow: none; }\n"
    "stackswitcher button:checked { background-color: #111827; border-color: #111827; color: #ffffff; }\n"
    "stackswitcher button:checked label { color: #ffffff; }\n"

    ".tile { background-color: #ffffff; border: 1px solid #e2e4e8; border-radius: 12px; }\n"
    ".tile-title { font-size: 9pt; font-weight: 700; color: #6b7280; }\n"
    ".tile-value { font-size: 24pt; font-weight: 800; color: #111827; }\n"
    ".tile-value.pass { color: #15803d; background-color: transparent; }\n"
    ".tile-value.fail { color: #b91c1c; background-color: transparent; }\n"
    ".tile-value.none { color: #c4c8cf; background-color: transparent; }\n"
    ".tile.accent { background-color: #1e3a8a; border-color: #1e3a8a; }\n"
    ".tile.accent .tile-title { color: #c7d2fe; }\n"
    ".tile.accent .tile-value { color: #ffffff; }\n"

    ".grade-row { border-bottom: 1px solid #f0f1f3; }\n"
    ".grade-name { font-size: 10.5pt; color: #1f2937; }\n"
    ".coef { font-size: 9pt; color: #6b7280; }\n"
    ".module-name { font-size: 10.5pt; font-weight: 600; color: #1f2937; }\n"

    ".pill { border-radius: 7px; padding: 2px 9px; font-weight: 700; font-size: 10.5pt; }\n"
    ".pill-big { border-radius: 8px; padding: 4px 12px; font-weight: 800; font-size: 13pt; }\n"
    ".chip { border-radius: 6px; padding: 1px 7px; font-weight: 600; font-size: 9.5pt; }\n"
    ".pass { background-color: #e7f6ec; color: #15803d; }\n"
    ".fail { background-color: #fdecec; color: #b91c1c; }\n"
    ".none { background-color: #f3f4f6; color: #9ca3af; }\n"

    ".year-head { font-size: 9pt; font-weight: 700; color: #6b7280; }\n"
    ".year-label { font-size: 11pt; font-weight: 600; color: #1f2937; }\n"
    ".year-total { font-size: 12pt; font-weight: 800; color: #111827; }\n"
    ".separator-line { background-color: #e5e7eb; min-height: 1px; }\n";

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

static void addClass(GtkWidget *widget, const char *cssClass) {
    gtk_style_context_add_class(gtk_widget_get_style_context(widget), cssClass);
}

static void removeClass(GtkWidget *widget, const char *cssClass) {
    gtk_style_context_remove_class(gtk_widget_get_style_context(widget), cssClass);
}

static void clearContainer(GtkWidget *container) {
    GList *children = gtk_container_get_children(GTK_CONTAINER(container));
    for (GList *ptr = children; ptr != NULL; ptr = ptr->next) {
        gtk_widget_destroy(ptr->data);
    }
    g_list_free(children);
}

static GtkWidget *makeLabel(const char *text, const char *cssClass) {
    GtkWidget *label = gtk_label_new(text);
    gtk_label_set_xalign(GTK_LABEL(label), 0.0f);
    if (cssClass != NULL) {
        addClass(label, cssClass);
    }
    return label;
}

static gboolean hasValue(float value) {
    return value > 0.0f;
}

static const char *colorClass(float value) {
    if (!hasValue(value)) return "none";
    return value >= 4.0f ? "pass" : "fail";
}

static gchar *formatAvg(float value) {
    if (!hasValue(value)) return g_strdup("–");
    return g_strdup_printf("%.1f", value);
}

static GtkWidget *makePill(float value, const char *sizeClass) {
    gchar *text = formatAvg(value);
    GtkWidget *label = gtk_label_new(text);
    g_free(text);
    addClass(label, sizeClass);
    addClass(label, colorClass(value));
    gtk_widget_set_valign(label, GTK_ALIGN_CENTER);
    gtk_widget_set_halign(label, GTK_ALIGN_END);
    return label;
}

static GtkWidget *makeTitleRow(const char *title, const char *titleClass,
                               const char *subtitle, float avg, const char *pillClass) {
    GtkWidget *row = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 12);
    GtkWidget *texts = gtk_box_new(GTK_ORIENTATION_VERTICAL, 2);

    gtk_box_pack_start(GTK_BOX(texts), makeLabel(title, titleClass), FALSE, FALSE, 0);
    if (subtitle != NULL) {
        gtk_box_pack_start(GTK_BOX(texts), makeLabel(subtitle, "card-subtitle"), FALSE, FALSE, 0);
    }
    gtk_box_pack_start(GTK_BOX(row), texts, TRUE, TRUE, 0);
    gtk_box_pack_end(GTK_BOX(row), makePill(avg, pillClass), FALSE, FALSE, 0);
    return row;
}

static GtkWidget *makeSectionCard(GtkWidget *parent, const char *title,
                                  const char *subtitle, float avg) {
    GtkWidget *card = gtk_box_new(GTK_ORIENTATION_VERTICAL, 14);
    addClass(card, "card");
    gtk_container_set_border_width(GTK_CONTAINER(card), 0);
    gtk_widget_set_margin_bottom(card, 16);

    GtkWidget *inner = gtk_box_new(GTK_ORIENTATION_VERTICAL, 14);
    gtk_container_set_border_width(GTK_CONTAINER(inner), 20);
    gtk_box_pack_start(GTK_BOX(card), inner, FALSE, FALSE, 0);

    gtk_box_pack_start(GTK_BOX(inner),
                       makeTitleRow(title, "card-title", subtitle, avg, "pill-big"),
                       FALSE, FALSE, 0);

    GtkWidget *body = gtk_box_new(GTK_ORIENTATION_VERTICAL, 14);
    gtk_box_pack_start(GTK_BOX(inner), body, FALSE, FALSE, 0);

    gtk_box_pack_start(GTK_BOX(parent), card, FALSE, FALSE, 0);
    return body;
}

static GtkWidget *makeSubjectBlock(GtkWidget *parent, const char *name,
                                   const char *subtitle, float avg) {
    GtkWidget *block = gtk_box_new(GTK_ORIENTATION_VERTICAL, 6);
    gtk_box_pack_start(GTK_BOX(block),
                       makeTitleRow(name, "subject-title", subtitle, avg, "pill"),
                       FALSE, FALSE, 0);

    GtkWidget *content = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    addClass(content, "subgroup");
    gtk_widget_set_margin_start(content, 4);
    GtkWidget *padded = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_widget_set_margin_start(padded, 14);
    gtk_box_pack_start(GTK_BOX(content), padded, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(block), content, FALSE, FALSE, 0);

    gtk_box_pack_start(GTK_BOX(parent), block, FALSE, FALSE, 0);
    return padded;
}

static void addEmptyMessage(GtkWidget *parent) {
    GtkWidget *label = makeLabel("Aucune note pour l'instant", "muted");
    gtk_widget_set_margin_top(label, 4);
    gtk_widget_set_margin_bottom(label, 4);
    gtk_box_pack_start(GTK_BOX(parent), label, FALSE, FALSE, 0);
}

static gchar *formatCoef(const Grade *grade) {
    if (grade->has_coef) return g_strdup_printf("coef. %.1f", grade->coef);
    return g_strdup("");
}

static void addSimpleGrades(GtkWidget *parent, Subject *subject) {
    if (subject->size == 0) {
        addEmptyMessage(parent);
        return;
    }

    GtkWidget *head = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 12);
    gtk_box_pack_start(GTK_BOX(head), makeLabel("THÈME", "column-head"), TRUE, TRUE, 0);
    GtkWidget *headNote = makeLabel("NOTE", "column-head");
    gtk_label_set_width_chars(GTK_LABEL(headNote), 6);
    gtk_label_set_xalign(GTK_LABEL(headNote), 1.0f);
    gtk_box_pack_end(GTK_BOX(head), headNote, FALSE, FALSE, 0);
    gtk_widget_set_margin_top(head, 2);
    gtk_box_pack_start(GTK_BOX(parent), head, FALSE, FALSE, 0);

    for (int i = 0; i < subject->size; i++) {
        Grade *grade = &subject->grades[i];
        const char *name = (grade->module != NULL && grade->module[0] != '\0')
                               ? grade->module
                               : subject->name;

        GtkWidget *row = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 12);
        addClass(row, "grade-row");

        GtkWidget *labelName = makeLabel(name, "grade-name");
        gtk_label_set_ellipsize(GTK_LABEL(labelName), PANGO_ELLIPSIZE_END);
        gchar *coefText = formatCoef(grade);
        GtkWidget *labelCoef = makeLabel(coefText, "coef");
        g_free(coefText);

        g_object_set(row, "margin-top", 3, "margin-bottom", 3, NULL);
        gtk_box_pack_start(GTK_BOX(row), labelName, TRUE, TRUE, 0);
        gtk_box_pack_end(GTK_BOX(row), makePill(grade->value, "pill"), FALSE, FALSE, 0);
        gtk_box_pack_end(GTK_BOX(row), labelCoef, FALSE, FALSE, 0);
        gtk_box_pack_start(GTK_BOX(parent), row, FALSE, FALSE, 0);
    }
}

static void addModuleGrades(GtkWidget *parent, Subject *subject) {
    if (subject->size == 0) {
        addEmptyMessage(parent);
        return;
    }

    int nModules = 0;
    char **modules = NULL;
    for (int i = 0; i < subject->size; i++) {
        if (subject->grades[i].module == NULL) continue;
        int found = 0;
        for (int m = 0; m < nModules; m++) {
            if (strcmp(modules[m], subject->grades[i].module) == 0) {
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

    GtkWidget *head = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 12);
    gtk_box_pack_start(GTK_BOX(head), makeLabel("MODULE", "column-head"), FALSE, FALSE, 0);
    GtkWidget *headAvg = makeLabel("MOY.", "column-head");
    gtk_label_set_width_chars(GTK_LABEL(headAvg), 6);
    gtk_label_set_xalign(GTK_LABEL(headAvg), 1.0f);
    gtk_box_pack_end(GTK_BOX(head), headAvg, FALSE, FALSE, 0);
    gtk_widget_set_margin_top(head, 2);
    gtk_box_pack_start(GTK_BOX(parent), head, FALSE, FALSE, 0);

    for (int m = 0; m < nModules; m++) {
        GtkWidget *row = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 12);
        addClass(row, "grade-row");
        g_object_set(row, "margin-top", 3, "margin-bottom", 3, NULL);

        GtkWidget *labelModule = makeLabel(modules[m], "module-name");
        gtk_label_set_ellipsize(GTK_LABEL(labelModule), PANGO_ELLIPSIZE_END);
        gtk_box_pack_start(GTK_BOX(row), labelModule, TRUE, TRUE, 0);

        gtk_box_pack_end(GTK_BOX(row), makePill(AvgModule(subject, modules[m]), "pill"),
                         FALSE, FALSE, 0);

        GtkWidget *chips = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 6);
        gtk_widget_set_valign(chips, GTK_ALIGN_CENTER);
        for (int i = 0; i < subject->size; i++) {
            Grade *grade = &subject->grades[i];
            if (grade->module == NULL || strcmp(grade->module, modules[m]) != 0) continue;

            gchar *text;
            if (grade->has_coef) {
                text = g_strdup_printf("%.1f  ×%.1f", grade->value, grade->coef);
            } else {
                text = g_strdup_printf("%.1f", grade->value);
            }
            GtkWidget *chip = gtk_label_new(text);
            g_free(text);
            addClass(chip, "chip");
            addClass(chip, colorClass(grade->value));
            gtk_box_pack_start(GTK_BOX(chips), chip, FALSE, FALSE, 0);
        }
        gtk_box_pack_end(GTK_BOX(row), chips, FALSE, FALSE, 8);

        gtk_box_pack_start(GTK_BOX(parent), row, FALSE, FALSE, 0);
    }

    free(modules);
}

static GtkWidget *makeTile(const char *title, float value, gboolean accent) {
    GtkWidget *tile = gtk_box_new(GTK_ORIENTATION_VERTICAL, 4);
    addClass(tile, "tile");
    if (accent) addClass(tile, "accent");

    GtkWidget *inner = gtk_box_new(GTK_ORIENTATION_VERTICAL, 4);
    gtk_container_set_border_width(GTK_CONTAINER(inner), 14);
    gtk_box_pack_start(GTK_BOX(tile), inner, TRUE, TRUE, 0);

    gtk_box_pack_start(GTK_BOX(inner), makeLabel(title, "tile-title"), FALSE, FALSE, 0);

    gchar *text = formatAvg(value);
    GtkWidget *labelValue = makeLabel(text, "tile-value");
    g_free(text);
    if (!accent) addClass(labelValue, colorClass(value));
    gtk_box_pack_start(GTK_BOX(inner), labelValue, FALSE, FALSE, 0);
    return tile;
}

void refreshUI(GtkWidget *container, int sem) {
    clearContainer(container);

    Subject *m, *e, *ec, *iI, *iC;
    if (sem == 1) {
        m = &maths; e = &english; ec = &ecg; iI = &infoI; iC = &infoC;
    } else {
        m = &maths2; e = &english2; ec = &ecg2; iI = &infoI2; iC = &infoC2;
    }

    float avgCBE = AvgCBE(m, e);
    float avgInfo = AvgInformatique(iI, iC);
    float avgECG = Avg(ec);
    float avgGeneral = AvgGlobal(avgInfo, avgECG, avgCBE);

    GtkWidget *tiles = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 12);
    gtk_box_set_homogeneous(GTK_BOX(tiles), TRUE);
    gtk_box_pack_start(GTK_BOX(tiles), makeTile("CBE", avgCBE, FALSE), TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(tiles), makeTile("INFORMATIQUE", avgInfo, FALSE), TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(tiles), makeTile("ECG", avgECG, FALSE), TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(tiles), makeTile("MOYENNE GÉNÉRALE", avgGeneral, TRUE), TRUE, TRUE, 0);
    gtk_widget_set_margin_bottom(tiles, 18);
    gtk_box_pack_start(GTK_BOX(container), tiles, FALSE, FALSE, 0);

    GtkWidget *cbe = makeSectionCard(container, "CBE", "Moyenne de Maths et Anglais", avgCBE);
    addSimpleGrades(makeSubjectBlock(cbe, "Maths", NULL, Avg(m)), m);
    addSimpleGrades(makeSubjectBlock(cbe, "Anglais", NULL, Avg(e)), e);

    GtkWidget *info = makeSectionCard(container, "Informatique", "80 % I  +  20 % C", avgInfo);
    addModuleGrades(makeSubjectBlock(info, "I", "Moyenne des modules I", AvgOfModules(iI)), iI);
    addModuleGrades(makeSubjectBlock(info, "C", "Moyenne des modules C", AvgOfModules(iC)), iC);

    GtkWidget *ecgBody = makeSectionCard(container, "ECG", NULL, avgECG);
    GtkWidget *ecgList = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_box_pack_start(GTK_BOX(ecgBody), ecgList, FALSE, FALSE, 0);
    addSimpleGrades(ecgList, ec);

    gtk_widget_show_all(container);
}

static void addYearCell(GtkWidget *grid, int col, int row, float value, gboolean total) {
    GtkWidget *pill = makePill(value, total ? "pill-big" : "pill");
    gtk_widget_set_halign(pill, GTK_ALIGN_CENTER);
    gtk_grid_attach(GTK_GRID(grid), pill, col, row, 1, 1);
}

void refreshAvg(GtkWidget *vboxAvg) {
    clearContainer(vboxAvg);

    float cbe1 = AvgCBE(&maths, &english);
    float cbe2 = AvgCBE(&maths2, &english2);
    float cbeYear = round05((cbe1 + cbe2) / 2.0f);
    float info1 = AvgInformatique(&infoI, &infoC);
    float info2 = AvgInformatique(&infoI2, &infoC2);
    float infoYear = round05((info1 + info2) / 2.0f);
    float ecg1 = Avg(&ecg);
    float ecg2_val = Avg(&ecg2);
    float ecgYear = round05((ecg1 + ecg2_val) / 2.0f);
    float generalYear = AvgGlobal(infoYear, ecgYear, cbeYear);

    float general1 = AvgGlobal(info1, ecg1, cbe1);
    float general2 = AvgGlobal(info2, ecg2_val, cbe2);

    GtkWidget *tiles = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 12);
    gtk_box_set_homogeneous(GTK_BOX(tiles), TRUE);
    gtk_box_pack_start(GTK_BOX(tiles), makeTile("CBE", cbeYear, FALSE), TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(tiles), makeTile("INFORMATIQUE", infoYear, FALSE), TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(tiles), makeTile("ECG", ecgYear, FALSE), TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(tiles), makeTile("MOYENNE ANNUELLE", generalYear, TRUE), TRUE, TRUE, 0);
    gtk_widget_set_margin_bottom(tiles, 18);
    gtk_box_pack_start(GTK_BOX(vboxAvg), tiles, FALSE, FALSE, 0);

    GtkWidget *body = makeSectionCard(vboxAvg, "Détail par semestre",
                                      "La moyenne annuelle est la moyenne des deux semestres, "
                                      "arrondie au demi-point.",
                                      generalYear);

    GtkWidget *grid = gtk_grid_new();
    gtk_grid_set_row_spacing(GTK_GRID(grid), 12);
    gtk_grid_set_column_spacing(GTK_GRID(grid), 24);
    gtk_widget_set_margin_top(grid, 6);

    const char *heads[] = {"", "SEMESTRE 1", "SEMESTRE 2", "ANNÉE"};
    for (int c = 0; c < 4; c++) {
        GtkWidget *head = makeLabel(heads[c], "year-head");
        if (c > 0) gtk_label_set_xalign(GTK_LABEL(head), 0.5f);
        gtk_grid_attach(GTK_GRID(grid), head, c, 0, 1, 1);
    }

    const char *names[] = {"CBE", "Informatique", "ECG"};
    float values[3][3] = {
        {cbe1, cbe2, cbeYear},
        {info1, info2, infoYear},
        {ecg1, ecg2_val, ecgYear},
    };
    for (int r = 0; r < 3; r++) {
        GtkWidget *name = makeLabel(names[r], "year-label");
        gtk_widget_set_hexpand(name, TRUE);
        gtk_grid_attach(GTK_GRID(grid), name, 0, r + 1, 1, 1);
        for (int c = 0; c < 3; c++) {
            addYearCell(grid, c + 1, r + 1, values[r][c], FALSE);
        }
    }

    GtkWidget *line = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    addClass(line, "separator-line");
    gtk_grid_attach(GTK_GRID(grid), line, 0, 4, 4, 1);

    gtk_grid_attach(GTK_GRID(grid), makeLabel("Moyenne générale", "year-total"), 0, 5, 1, 1);
    addYearCell(grid, 1, 5, general1, FALSE);
    addYearCell(grid, 2, 5, general2, FALSE);
    addYearCell(grid, 3, 5, generalYear, TRUE);

    gtk_box_pack_start(GTK_BOX(body), grid, FALSE, FALSE, 0);
    gtk_widget_show_all(vboxAvg);
}

static gboolean subjectUsesModules(GradeForm *form) {
    const gchar *id = gtk_combo_box_get_active_id(GTK_COMBO_BOX(form->comboSubject));
    return id != NULL && (strcmp(id, "I") == 0 || strcmp(id, "C") == 0);
}

static gboolean parseNumber(const gchar *text, float *out) {
    if (text == NULL || text[0] == '\0') return FALSE;

    gchar *copy = g_strdup(text);
    g_strdelimit(copy, ",", '.');
    gchar *end = NULL;
    double value = g_ascii_strtod(copy, &end);
    gboolean ok = (end != copy && *end == '\0');
    g_free(copy);

    if (ok) *out = (float)value;
    return ok;
}

static void on_numericInsert(GtkEditable *editable, const gchar *text, gint length,
                             gint *position, gpointer data) {
    if (length < 0) length = strlen(text);
    for (int i = 0; i < length; i++) {
        if (!g_ascii_isdigit(text[i]) && text[i] != '.' && text[i] != ',') {
            g_signal_stop_emission_by_name(editable, "insert-text");
            gtk_widget_error_bell(GTK_WIDGET(editable));
            return;
        }
    }
}

static void setFeedback(GradeForm *form, const char *text, gboolean isError) {
    gtk_label_set_text(GTK_LABEL(form->labelFeedback), text);
    gtk_widget_set_visible(form->labelFeedback, text[0] != '\0');
    removeClass(form->labelFeedback, "feedback-ok");
    removeClass(form->labelFeedback, "feedback-error");
    addClass(form->labelFeedback, isError ? "feedback-error" : "feedback-ok");
}

static void markInvalid(GtkWidget *entry, gboolean invalid) {
    if (invalid) addClass(entry, "invalid");
    else removeClass(entry, "invalid");
}

static gboolean validateForm(GradeForm *form) {
    const char *error = NULL;
    float value;

    const gchar *gradeText = gtk_entry_get_text(GTK_ENTRY(form->entryGrade));
    gboolean gradeEmpty = (gradeText[0] == '\0');
    gboolean gradeOk = parseNumber(gradeText, &value) && value >= 1.0f && value <= 6.0f;
    markInvalid(form->entryGrade, !gradeEmpty && !gradeOk);
    if (!gradeEmpty && !gradeOk) error = "La note doit être comprise entre 1 et 6.";

    gboolean weighted = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(form->checkWeight));
    gboolean coefOk = TRUE;
    if (weighted) {
        const gchar *coefText = gtk_entry_get_text(GTK_ENTRY(form->entryWeight));
        gboolean coefEmpty = (coefText[0] == '\0');
        coefOk = parseNumber(coefText, &value) && value > 0.0f && value <= 100.0f;
        markInvalid(form->entryWeight, !coefEmpty && !coefOk);
        if (!coefEmpty && !coefOk && error == NULL) error = "Le coefficient doit être plus grand que 0.";
    } else {
        markInvalid(form->entryWeight, FALSE);
    }

    gchar *module = g_strstrip(g_strdup(gtk_entry_get_text(GTK_ENTRY(form->entryModule))));
    gboolean moduleOk = !subjectUsesModules(form) || module[0] != '\0';
    g_free(module);

    gboolean valid = gradeOk && coefOk && moduleOk && !form->yearClosed;
    gtk_widget_set_sensitive(form->btnAdd, valid);

    if (error != NULL) {
        setFeedback(form, error, TRUE);
    } else if (gtk_style_context_has_class(gtk_widget_get_style_context(form->labelFeedback),
                                           "feedback-error")) {
        setFeedback(form, "", FALSE);
    }
    return valid;
}

static void on_formChanged(GtkWidget *widget, gpointer data) {
    validateForm((GradeForm *)data);
}

static void on_subjectChanged(GtkWidget *combo, gpointer data) {
    GradeForm *form = data;
    if (subjectUsesModules(form)) {
        gtk_label_set_text(GTK_LABEL(form->labelModule), "Module");
        gtk_entry_set_placeholder_text(GTK_ENTRY(form->entryModule), "ex. 122 (obligatoire)");
    } else {
        gtk_label_set_text(GTK_LABEL(form->labelModule), "Thème");
        gtk_entry_set_placeholder_text(GTK_ENTRY(form->entryModule), "ex. Fonctions (optionnel)");
    }
    validateForm(form);
}

void on_weightToggled(GtkWidget *checkWidget, gpointer data) {
    GradeForm *form = data;
    gboolean isChecked = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(checkWidget));

    gtk_revealer_set_reveal_child(GTK_REVEALER(form->revealWeight), isChecked);
    gtk_widget_set_sensitive(form->entryWeight, isChecked);
    if (isChecked) {
        gtk_widget_grab_focus(form->entryWeight);
    } else {
        gtk_entry_set_text(GTK_ENTRY(form->entryWeight), "");
    }
    validateForm(form);
}

static void updateSemesterState(GradeForm *form) {
    if (form->yearClosed) {
        gtk_label_set_text(GTK_LABEL(form->labelTarget), "Année clôturée");
        addClass(form->labelTarget, "closed");
        gtk_widget_set_sensitive(form->cardForm, FALSE);
        gtk_widget_hide(form->btnCloseSem);
        setFeedback(form, "Le semestre 2 est clôturé : plus d'ajout possible.", FALSE);
    } else {
        gchar *text = g_strdup_printf("Ajout dans : Semestre %d", currentSem);
        gtk_label_set_text(GTK_LABEL(form->labelTarget), text);
        g_free(text);
        gtk_button_set_label(GTK_BUTTON(form->btnCloseSem),
                             currentSem == 1 ? "Clôturer le semestre 1"
                                             : "Clôturer le semestre 2");
    }

    gtk_container_child_set(GTK_CONTAINER(form->stack), form->pageSem1, "title",
                            (currentSem == 1 && !form->yearClosed) ? "Semestre 1  •  en cours"
                                                                   : "Semestre 1", NULL);
    gtk_container_child_set(GTK_CONTAINER(form->stack), form->pageSem2, "title",
                            (currentSem == 2 && !form->yearClosed) ? "Semestre 2  •  en cours"
                                                                   : "Semestre 2", NULL);
}

static void showCurrentSemester(GradeForm *form) {
    gtk_stack_set_visible_child(GTK_STACK(form->stack),
                                currentSem == 1 ? form->pageSem1 : form->pageSem2);
}

void on_addGrade(GtkWidget *button, gpointer data) {
    GradeForm *form = data;
    if (!validateForm(form)) return;

    const gchar *id = gtk_combo_box_get_active_id(GTK_COMBO_BOX(form->comboSubject));
    Subject *subject = getSubjectByName(id, currentSem);
    if (subject == NULL) {
        setFeedback(form, "Matière inconnue.", TRUE);
        return;
    }

    Grade g;
    memset(&g, 0, sizeof(g));
    g.module = g_strstrip(g_strdup(gtk_entry_get_text(GTK_ENTRY(form->entryModule))));
    parseNumber(gtk_entry_get_text(GTK_ENTRY(form->entryGrade)), &g.value);

    gboolean isChecked = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(form->checkWeight));
    g.has_coef = isChecked;
    if (isChecked) {
        parseNumber(gtk_entry_get_text(GTK_ENTRY(form->entryWeight)), &g.coef);
    } else {
        g.coef = 1.0f;
    }

    gchar *message;
    if (g.module[0] != '\0') {
        message = g_strdup_printf("✓ %.1f ajouté en %s — %s (semestre %d)",
                                  g.value, id, g.module, currentSem);
    } else {
        message = g_strdup_printf("✓ %.1f ajouté en %s (semestre %d)", g.value, id, currentSem);
    }

    addGrade(subject, g);
    saveData();

    refreshUI(form->vboxList, 1);
    refreshUI(form->vboxListSem2, 2);
    refreshAvg(form->vboxAvg);
    showCurrentSemester(form);

    gtk_entry_set_text(GTK_ENTRY(form->entryModule), "");
    gtk_entry_set_text(GTK_ENTRY(form->entryGrade), "");
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(form->checkWeight), FALSE);
    validateForm(form);
    setFeedback(form, message, FALSE);
    g_free(message);

    gtk_widget_grab_focus(form->entryModule);
}

static void on_entryActivate(GtkWidget *entry, gpointer data) {
    GradeForm *form = data;
    if (gtk_widget_get_sensitive(form->btnAdd)) {
        gtk_button_clicked(GTK_BUTTON(form->btnAdd));
    }
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
            "Clôturer le semestre 1 ?"
        );
        gtk_message_dialog_format_secondary_text(GTK_MESSAGE_DIALOG(dialog),
            "Les nouvelles notes seront ensuite ajoutées au semestre 2.");
        result = gtk_dialog_run(GTK_DIALOG(dialog));
        gtk_widget_destroy(dialog);

        if (result == GTK_RESPONSE_YES) {
            currentSem = 2;
            saveData();
            refreshUI(form->vboxListSem2, 2);
            updateSemesterState(form);
            showCurrentSemester(form);
        }
    } else {
        GtkWidget *dialog = gtk_message_dialog_new(
            GTK_WINDOW(gtk_widget_get_toplevel(button)),
            GTK_DIALOG_MODAL,
            GTK_MESSAGE_QUESTION,
            GTK_BUTTONS_YES_NO,
            "Clôturer le semestre 2 ?"
        );
        gtk_message_dialog_format_secondary_text(GTK_MESSAGE_DIALOG(dialog),
            "Vous ne pourrez plus ajouter de notes.");
        result = gtk_dialog_run(GTK_DIALOG(dialog));
        gtk_widget_destroy(dialog);

        if (result == GTK_RESPONSE_YES) {
            form->yearClosed = TRUE;
            updateSemesterState(form);
            validateForm(form);
        }
    }
}

static GtkWidget *makeField(GtkWidget **labelOut, const char *labelText, GtkWidget *input) {
    GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 6);
    GtkWidget *label = makeLabel(labelText, "field-label");
    gtk_label_set_mnemonic_widget(GTK_LABEL(label), input);
    gtk_box_pack_start(GTK_BOX(box), label, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(box), input, FALSE, FALSE, 0);
    if (labelOut != NULL) *labelOut = label;
    return box;
}

static GtkWidget *buildForm(GradeForm *form) {
    GtkWidget *card = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    addClass(card, "card");
    addClass(card, "form");

    GtkWidget *inner = gtk_box_new(GTK_ORIENTATION_VERTICAL, 16);
    gtk_container_set_border_width(GTK_CONTAINER(inner), 20);
    gtk_box_pack_start(GTK_BOX(card), inner, FALSE, FALSE, 0);

    GtkWidget *titleBox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 8);
    gtk_box_pack_start(GTK_BOX(titleBox), makeLabel("Ajouter une note", "card-title"), FALSE, FALSE, 0);
    form->labelTarget = gtk_label_new("");
    addClass(form->labelTarget, "target-badge");
    gtk_widget_set_halign(form->labelTarget, GTK_ALIGN_START);
    gtk_box_pack_start(GTK_BOX(titleBox), form->labelTarget, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(inner), titleBox, FALSE, FALSE, 0);

    form->comboSubject = gtk_combo_box_text_new();
    GtkComboBoxText *combo = GTK_COMBO_BOX_TEXT(form->comboSubject);
    gtk_combo_box_text_append(combo, "Maths", "Maths  ·  CBE");
    gtk_combo_box_text_append(combo, "Anglais", "Anglais  ·  CBE");
    gtk_combo_box_text_append(combo, "I", "I  ·  Informatique");
    gtk_combo_box_text_append(combo, "C", "C  ·  Informatique");
    gtk_combo_box_text_append(combo, "ECG", "ECG");
    gtk_combo_box_set_active_id(GTK_COMBO_BOX(combo), "I");
    gtk_box_pack_start(GTK_BOX(inner), makeField(NULL, "Matière", form->comboSubject), FALSE, FALSE, 0);

    form->entryModule = gtk_entry_new();
    gtk_entry_set_max_length(GTK_ENTRY(form->entryModule), 40);
    gtk_box_pack_start(GTK_BOX(inner), makeField(&form->labelModule, "Module", form->entryModule),
                       FALSE, FALSE, 0);

    form->entryGrade = gtk_entry_new();
    gtk_entry_set_max_length(GTK_ENTRY(form->entryGrade), 4);
    gtk_entry_set_placeholder_text(GTK_ENTRY(form->entryGrade), "1 à 6, ex. 5.5");
    gtk_entry_set_input_purpose(GTK_ENTRY(form->entryGrade), GTK_INPUT_PURPOSE_NUMBER);
    gtk_box_pack_start(GTK_BOX(inner), makeField(NULL, "Note", form->entryGrade), FALSE, FALSE, 0);

    GtkWidget *coefBox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 8);
    form->checkWeight = gtk_check_button_new_with_label("Note pondérée (coefficient)");
    gtk_box_pack_start(GTK_BOX(coefBox), form->checkWeight, FALSE, FALSE, 0);

    form->entryWeight = gtk_entry_new();
    gtk_entry_set_max_length(GTK_ENTRY(form->entryWeight), 5);
    gtk_entry_set_placeholder_text(GTK_ENTRY(form->entryWeight), "ex. 2");
    gtk_widget_set_sensitive(form->entryWeight, FALSE);
    form->revealWeight = gtk_revealer_new();
    gtk_revealer_set_transition_type(GTK_REVEALER(form->revealWeight),
                                     GTK_REVEALER_TRANSITION_TYPE_SLIDE_DOWN);
    gtk_container_add(GTK_CONTAINER(form->revealWeight), form->entryWeight);
    gtk_box_pack_start(GTK_BOX(coefBox), form->revealWeight, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(inner), coefBox, FALSE, FALSE, 0);

    form->btnAdd = gtk_button_new_with_label("Ajouter la note");
    addClass(form->btnAdd, "btn-primary");
    gtk_widget_set_sensitive(form->btnAdd, FALSE);
    gtk_widget_set_margin_top(form->btnAdd, 4);
    gtk_box_pack_start(GTK_BOX(inner), form->btnAdd, FALSE, FALSE, 0);

    form->labelFeedback = makeLabel("", "feedback-ok");
    gtk_label_set_line_wrap(GTK_LABEL(form->labelFeedback), TRUE);
    gtk_widget_set_no_show_all(form->labelFeedback, TRUE);
    gtk_box_pack_start(GTK_BOX(inner), form->labelFeedback, FALSE, FALSE, 0);

    g_signal_connect(form->comboSubject, "changed", G_CALLBACK(on_subjectChanged), form);
    g_signal_connect(form->entryModule, "changed", G_CALLBACK(on_formChanged), form);
    g_signal_connect(form->entryGrade, "changed", G_CALLBACK(on_formChanged), form);
    g_signal_connect(form->entryWeight, "changed", G_CALLBACK(on_formChanged), form);
    g_signal_connect(form->entryGrade, "insert-text", G_CALLBACK(on_numericInsert), NULL);
    g_signal_connect(form->entryWeight, "insert-text", G_CALLBACK(on_numericInsert), NULL);
    g_signal_connect(form->entryModule, "activate", G_CALLBACK(on_entryActivate), form);
    g_signal_connect(form->entryGrade, "activate", G_CALLBACK(on_entryActivate), form);
    g_signal_connect(form->entryWeight, "activate", G_CALLBACK(on_entryActivate), form);
    g_signal_connect(form->checkWeight, "toggled", G_CALLBACK(on_weightToggled), form);
    g_signal_connect(form->btnAdd, "clicked", G_CALLBACK(on_addGrade), form);

    form->cardForm = card;
    return card;
}

static void on_destroy(GtkWidget *widget, gpointer data) {
    gtk_main_quit();
}

static GtkWidget *makeScrollPage(GtkWidget **content) {
    GtkWidget *scroll = gtk_scrolled_window_new(NULL, NULL);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scroll), GTK_POLICY_NEVER, GTK_POLICY_AUTOMATIC);

    *content = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_widget_set_margin_start(*content, 4);
    gtk_widget_set_margin_end(*content, 16);
    gtk_widget_set_margin_bottom(*content, 24);
    gtk_container_add(GTK_CONTAINER(scroll), *content);
    return scroll;
}

static GtkWidget *buildHeader(void) {
    GtkWidget *header = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 16);
    addClass(header, "app-header");
    gtk_container_set_border_width(GTK_CONTAINER(header), 0);

    GtkWidget *inner = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 16);
    g_object_set(inner, "margin-start", 24, "margin-end", 24, "margin-top", 14, "margin-bottom", 14, NULL);
    gtk_box_pack_start(GTK_BOX(header), inner, TRUE, TRUE, 0);

    GdkPixbuf *logo = gdk_pixbuf_new_from_file_at_scale("./img/ETML-Grades.png", -1, 44, TRUE, NULL);
    if (logo != NULL) {
        GtkWidget *logoImage = gtk_image_new_from_pixbuf(logo);
        g_object_unref(logo);
        gtk_box_pack_start(GTK_BOX(inner), logoImage, FALSE, FALSE, 0);
    }

    GtkWidget *titles = gtk_box_new(GTK_ORIENTATION_VERTICAL, 2);
    gtk_widget_set_valign(titles, GTK_ALIGN_CENTER);
    gtk_box_pack_start(GTK_BOX(titles), makeLabel("ETML — Mes notes", "app-title"), FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(titles),
                       makeLabel("Notes, moyennes de modules et moyennes générales", "app-subtitle"),
                       FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(inner), titles, TRUE, TRUE, 0);

    GtkWidget *link = gtk_link_button_new("https://github.com/SHR-Zidane");
    gtk_button_set_label(GTK_BUTTON(link), "Made by SHR_Zidane");
    gtk_widget_set_valign(link, GTK_ALIGN_CENTER);
    gtk_box_pack_end(GTK_BOX(inner), link, FALSE, FALSE, 0);

    GtkWidget *comboYear = gtk_combo_box_text_new();
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(comboYear), "Année 1");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(comboYear), "Année 2");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(comboYear), "Année 3");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(comboYear), "Année 4");
    gtk_combo_box_set_active(GTK_COMBO_BOX(comboYear), 0);
    gtk_widget_set_valign(comboYear, GTK_ALIGN_CENTER);
    gtk_box_pack_end(GTK_BOX(inner), comboYear, FALSE, FALSE, 0);

    return header;
}

void create_main_window(int argc, char *argv[]) {
    static GradeForm form;

    gtk_init(&argc, &argv);
    applyStyle();

    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "ETML Grades");
    gtk_window_set_default_size(GTK_WINDOW(window), 1240, 860);
    g_signal_connect(window, "destroy", G_CALLBACK(on_destroy), NULL);

    GdkPixbuf *icon = gdk_pixbuf_new_from_file("../img/favicon.ico", NULL);
    if (icon != NULL) {
        gtk_window_set_icon(GTK_WINDOW(window), icon);
        g_object_unref(icon);
    }

    GtkWidget *root = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_container_add(GTK_CONTAINER(window), root);
    gtk_box_pack_start(GTK_BOX(root), buildHeader(), FALSE, FALSE, 0);

    GtkWidget *body = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 24);
    addClass(body, "app-body");
    g_object_set(body, "margin-start", 24, "margin-end", 8, "margin-top", 20, NULL);
    gtk_box_pack_start(GTK_BOX(root), body, TRUE, TRUE, 0);

    GtkWidget *sidebar = gtk_box_new(GTK_ORIENTATION_VERTICAL, 16);
    gtk_widget_set_size_request(sidebar, 330, -1);
    gtk_box_pack_start(GTK_BOX(sidebar), buildForm(&form), FALSE, FALSE, 0);

    form.btnCloseSem = gtk_button_new_with_label("Clôturer le semestre 1");
    addClass(form.btnCloseSem, "btn-danger");
    g_signal_connect(form.btnCloseSem, "clicked", G_CALLBACK(on_closeSem1), &form);
    gtk_box_pack_end(GTK_BOX(sidebar), form.btnCloseSem, FALSE, FALSE, 24);

    gtk_box_pack_start(GTK_BOX(body), sidebar, FALSE, FALSE, 0);

    GtkWidget *main = gtk_box_new(GTK_ORIENTATION_VERTICAL, 16);

    form.stack = gtk_stack_new();
    gtk_stack_set_transition_type(GTK_STACK(form.stack), GTK_STACK_TRANSITION_TYPE_CROSSFADE);
    form.pageSem1 = makeScrollPage(&form.vboxList);
    form.pageSem2 = makeScrollPage(&form.vboxListSem2);
    GtkWidget *pageYear = makeScrollPage(&form.vboxAvg);
    gtk_stack_add_titled(GTK_STACK(form.stack), form.pageSem1, "sem1", "Semestre 1");
    gtk_stack_add_titled(GTK_STACK(form.stack), form.pageSem2, "sem2", "Semestre 2");
    gtk_stack_add_titled(GTK_STACK(form.stack), pageYear, "year", "Année");

    GtkWidget *switcher = gtk_stack_switcher_new();
    gtk_stack_switcher_set_stack(GTK_STACK_SWITCHER(switcher), GTK_STACK(form.stack));
    gtk_widget_set_halign(switcher, GTK_ALIGN_START);

    gtk_box_pack_start(GTK_BOX(main), switcher, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(main), form.stack, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(body), main, TRUE, TRUE, 0);

    form.yearClosed = FALSE;
    refreshUI(form.vboxList, 1);
    refreshUI(form.vboxListSem2, 2);
    refreshAvg(form.vboxAvg);
    on_subjectChanged(form.comboSubject, &form);

    gtk_widget_show_all(window);
    updateSemesterState(&form);
    showCurrentSemester(&form);
    gtk_widget_grab_focus(form.entryModule);

    gtk_main();
}
