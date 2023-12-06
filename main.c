

#include <gtk/gtk.h>
#include <gdk/gdk.h>
#include <stdlib.h>
#include <time.h>
#include <stdlib.h>
#include <stdbool.h>
#include <cjson/cJSON.h>
#include <stdio.h>
#include <string.h>
#include <regex.h>
#include <stddef.h>

#include "./include/types.h"
#include "./utils/scheduling/useful/useful.h"
#include "./utils/queues/fifo/queue.h"
#include "./utils/queues/priority/priority_queue.h"
#include "./utils/gantt/format.h"
#include "./utils/scheduling/SJF/SJF.h"
#include "./utils/scheduling/RR/round_robin.h"
#include "./utils/scheduling/FIFO/FIFO.h"
#include "./utils/scheduling/priority/priority.h"
#include "./utils/scheduling/priority_np/priority_np.h"
#include "./utils/scheduling/multilevel/multilevel.h"
#include "./utils/scheduling/SRT/SRT.h"
#include "./utils/metrics/metrics.h"
#include "./utils/config/IO/write/write.h"
#include "./utils/config/IO/read/read.h"

const char *filename = "generated_config.json";
int config_file_size = 0;
int executed_tasks_size = 0;
ExecutedTask tasks[100];
process *proc_head;
GtkWidget *window, *drawing_area, *vbox, *dialog, *metrics_window, *metrics_table, *open_metrics, *settings_window, *view_settings, *max_exec_input, *max_proc_input, *max_arrival_input, *max_priority_input;
char *exec_range = "1-2";

char *max_proc_range = "1-2";
char *priority_range = "1-10";
char *arrival_range = "1-10";

cairo_surface_t *global_surface = NULL;
options ops;
bool is_metrics_open = false;

Algorithm current_algorithm = FIFO;

char *concat(const char *s1, const char *s2)
{
    char *result = malloc(strlen(s1) + strlen(s2) + 1);
    if (result == NULL)
    {
        printf("Err alloc");
        exit(-1);
    }
    strcpy(result, s1);
    strcat(result, s2);
    return result;
}
void load_algorithm(Algorithm fnc)
{
    ExecutedTask *task = NULL;
    executed_tasks_size = 0;
    char *window_name_suffix = "";

    switch (fnc)
    {
    case FIFO:
        task = get_fifo_output(proc_head, config_file_size, &executed_tasks_size);
        current_algorithm = FIFO;
        window_name_suffix = " (FIFO)";
        break;

    case SRT:
        task = get_srt_output(proc_head, config_file_size, &executed_tasks_size);
        current_algorithm = SRT;
        window_name_suffix = " (SRT)";

        break;
    case SJF:
        task = get_sjf_output(proc_head, config_file_size, &executed_tasks_size);
        current_algorithm = SJF;
        window_name_suffix = " (SJF)";
        break;

    case PRIORITY_P:
        task = get_priority_output(proc_head, config_file_size, &executed_tasks_size);
        current_algorithm = PRIORITY_P;
        window_name_suffix = " (PRIORITY PREEMPTIVE)";

        break;

    case PRIORITY:
        task = get_priority_np_output(proc_head, config_file_size, &executed_tasks_size);
        current_algorithm = PRIORITY;
        window_name_suffix = " (PRIORITY NON-PREEMPTIVE)";

        break;

    case RR:
        task = get_round_robin_output(ops.quantum, proc_head, config_file_size, &executed_tasks_size);
        current_algorithm = RR;
        window_name_suffix = " (RR)";

        break;
    case MULTILEVEL:
        task = get_multilevel_output(ops.quantum, proc_head, config_file_size, &executed_tasks_size);
        current_algorithm = MULTILEVEL;
        window_name_suffix = " (MULTILEVEL)";

    default:
        break;
    }

    gtk_window_set_title(GTK_WINDOW(window), concat("Process Scheduler ", window_name_suffix));

    if (task != NULL)
    {
        for (int i = 0; i < executed_tasks_size; i++)
        {
            tasks[i] = task[i];
        }

        free(task);
    }

    gtk_widget_queue_draw(drawing_area);
}
GtkWidget *draw_metrics_table()
{
    GtkWidget *table = gtk_grid_new();
    gtk_grid_set_row_spacing(GTK_GRID(table), 10);
    gtk_grid_set_column_spacing(GTK_GRID(table), 60);

    GtkWidget *label1 = gtk_label_new(NULL);
    gtk_label_set_markup(GTK_LABEL(label1), "<b> Process Name</b>");
    gtk_widget_set_halign(label1, GTK_ALIGN_CENTER);
    gtk_widget_set_valign(label1, GTK_ALIGN_FILL);
    gtk_grid_attach(GTK_GRID(table), label1, 0, 0, 1, 1);

    GtkWidget *label2 = gtk_label_new(NULL);
    gtk_label_set_markup(GTK_LABEL(label2), "<b> Waiting Time</b>");

    gtk_widget_set_halign(label2, GTK_ALIGN_CENTER);
    gtk_widget_set_valign(label2, GTK_ALIGN_FILL);
    gtk_grid_attach(GTK_GRID(table), label2, 1, 0, 1, 1);

    GtkWidget *label3 = gtk_label_new(NULL);
    gtk_label_set_markup(GTK_LABEL(label3), "<b> Rotation Time </b>");

    gtk_widget_set_halign(label3, GTK_ALIGN_CENTER);
    gtk_widget_set_valign(label3, GTK_ALIGN_FILL);
    gtk_grid_attach(GTK_GRID(table), label3, 2, 0, 1, 1);

    int row = 1;
    int col = 1;
    int total_rotation_time = 0;
    int total_waiting_time = 0;
    for (int i = 0; i < config_file_size; i++)
    {
        GtkWidget *label4 = gtk_label_new(proc_head[i].name);
        gtk_widget_set_halign(label4, GTK_ALIGN_CENTER);
        gtk_widget_set_valign(label4, GTK_ALIGN_FILL);
        gtk_grid_attach(GTK_GRID(table), label4, 0, row, 1, 1);

        char waiting_string[20];
        int waiting_time = get_waiting_time(proc_head[i].name, tasks, executed_tasks_size, proc_head, config_file_size);
        total_waiting_time += waiting_time;
        sprintf(waiting_string, "%d", waiting_time);
        GtkWidget *label5 = gtk_label_new(waiting_string);
        gtk_widget_set_halign(label5, GTK_ALIGN_CENTER);
        gtk_widget_set_valign(label5, GTK_ALIGN_FILL);
        gtk_grid_attach(GTK_GRID(table), label5, 1, row, 1, 1);

        char rotation_string[20];
        int rotation_time = get_rotation_time(proc_head[i].name, tasks, executed_tasks_size);
        total_rotation_time += rotation_time;
        sprintf(rotation_string, "%d", rotation_time);
        GtkWidget *label6 = gtk_label_new(rotation_string);
        gtk_widget_set_halign(label6, GTK_ALIGN_CENTER);
        gtk_widget_set_valign(label6, GTK_ALIGN_FILL);
        gtk_grid_attach(GTK_GRID(table), label6, 2, row, 1, 1);

        row++;
    }
    gtk_window_set_default_size(GTK_WINDOW(metrics_window), 400, 10 * row);
    GtkWidget *total_label = gtk_label_new(NULL);
    gtk_label_set_markup(GTK_LABEL(total_label), "<b> Average: </b>");

    gtk_widget_set_halign(total_label, GTK_ALIGN_CENTER);
    gtk_widget_set_valign(total_label, GTK_ALIGN_FILL);
    gtk_grid_attach(GTK_GRID(table), total_label, 0, row, 1, 1);

    char total_waiting_string[20];
    sprintf(total_waiting_string, "%.2f units", (float)total_waiting_time / (config_file_size));
    GtkWidget *total_waiting_label = gtk_label_new(total_waiting_string);
    gtk_widget_set_halign(total_waiting_label, GTK_ALIGN_CENTER);
    gtk_widget_set_valign(total_waiting_label, GTK_ALIGN_FILL);
    gtk_grid_attach(GTK_GRID(table), total_waiting_label, 1, row, 1, 1);

    char total_rotation_string[20];
    sprintf(total_rotation_string, "%.2f units", (float)total_rotation_time / (config_file_size));
    GtkWidget *total_rotation_label = gtk_label_new(total_rotation_string);
    gtk_widget_set_halign(total_rotation_label, GTK_ALIGN_CENTER);
    gtk_widget_set_valign(total_rotation_label, GTK_ALIGN_FILL);
    gtk_grid_attach(GTK_GRID(table), total_rotation_label, 2, row, 1, 1);

    return table;
}
gboolean on_delete_event(GtkWidget *widget, GdkEvent *event, gpointer user_data)
{

    switch (GPOINTER_TO_INT(user_data))
    {
    case SETTINGS_WINDOW:
        gtk_check_menu_item_set_active(GTK_CHECK_MENU_ITEM(view_settings), FALSE);
        break;

    case METRICS_WINDOW:
        gtk_check_menu_item_set_active(GTK_CHECK_MENU_ITEM(open_metrics), FALSE);
        break;
    }

    return FALSE;
}
bool matchRegex(const char *str)
{
    regex_t re;
    regmatch_t match[3];
    int ret;

    if (regcomp(&re, "([0-9]+)-([0-9]+)", REG_EXTENDED) != 0)
        return false;

    ret = regexec(&re, str, sizeof(match) / sizeof(match[0]), match, 0);
    regfree(&re);

    if (ret == 0)
    {
        char group1Str[64], group2Str[64];
        snprintf(group1Str, sizeof(group1Str), "%.*s", (int)(match[1].rm_eo - match[1].rm_so), str + match[1].rm_so);
        snprintf(group2Str, sizeof(group2Str), "%.*s", (int)(match[2].rm_eo - match[2].rm_so), str + match[2].rm_so);
        int start = strtol(group1Str, NULL, 10);
        int end = strtol(group2Str, NULL, 10);
        if (start < end)
            return true;
    }

    return false;
}

bool save_settings(GtkWidget *btn, gpointer user_data)
{
    char *max_exec_input_txt = gtk_entry_get_text(GTK_ENTRY(max_exec_input));
    g_print("%d\n", matchRegex(max_exec_input_txt));
    char *max_proc_input_txt = gtk_entry_get_text(GTK_ENTRY(max_proc_input));
    char *max_priority_input_txt = gtk_entry_get_text(GTK_ENTRY(max_priority_input));
    char *max_arrival_input_txt = gtk_entry_get_text(GTK_ENTRY(max_arrival_input));

    if (matchRegex(max_exec_input_txt) && matchRegex(max_proc_input_txt) && matchRegex(max_priority_input_txt) && matchRegex(max_arrival_input_txt))
    {
        exec_range = max_exec_input_txt;
        max_proc_range = max_proc_input_txt;
        priority_range = max_priority_input_txt;
        arrival_range = max_arrival_input_txt;
    }
    else
    {
        show_message_box_("Invalid input. Please follow start-end format.");
    }

    return FALSE;
}

void on_slider_value_changed(GtkWidget *slider, gpointer user_data)
{
    GtkLabel *label = GTK_LABEL(user_data);
    int value = gtk_range_get_value(GTK_RANGE(slider));
    gchar *label_text = g_strdup_printf("Current Quantum: %d", value);
    modify_quantum_val(value);

    proc_head = read_config_file("generated_config.json", &config_file_size, &ops);
    if (current_algorithm == RR)
        load_algorithm(RR);
    gtk_label_set_text(label, label_text);
    g_free(label_text);
}

void show_settings_window()
{

    settings_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    g_signal_connect(settings_window, "delete-event", G_CALLBACK(on_delete_event), GINT_TO_POINTER(SETTINGS_WINDOW));

    gtk_window_set_title(GTK_WINDOW(settings_window), "Settings");
    gtk_window_set_default_size(GTK_WINDOW(settings_window), 400, 430);
    gtk_window_set_resizable(GTK_WINDOW(settings_window), FALSE);

    GtkWidget *vbox = gtk_vbox_new(FALSE, 5);

    PangoFontDescription *font_desc = pango_font_description_new();
    pango_font_description_set_size(font_desc, 20 * PANGO_SCALE);
    pango_font_description_set_style(font_desc, PANGO_STYLE_OBLIQUE);
    pango_font_description_set_weight(font_desc, PANGO_WEIGHT_BOLD);

    PangoFontDescription *sub_font_desc = pango_font_description_new();
    pango_font_description_set_size(sub_font_desc, 14 * PANGO_SCALE);
    pango_font_description_set_style(sub_font_desc, PANGO_STYLE_OBLIQUE);
    pango_font_description_set_weight(sub_font_desc, PANGO_WEIGHT_BOLD);
    GtkWidget *round_robin_label = gtk_label_new("Round robin: ");
    gtk_widget_set_margin_top(round_robin_label, 15);
    gtk_widget_set_margin_bottom(round_robin_label, 15);
    gtk_box_pack_start(GTK_BOX(vbox), round_robin_label, FALSE, FALSE, 0);
    gtk_widget_override_font(round_robin_label, sub_font_desc);

    pango_font_description_free(font_desc);

    char *quantum_label_content = g_strdup_printf("Current Quantum: %d", ops.quantum);
    GtkWidget *quantum_label = gtk_label_new(quantum_label_content);

    g_free(quantum_label_content);
    gtk_box_pack_start(GTK_BOX(vbox), quantum_label, FALSE, FALSE, 0);

    GtkWidget *slider = gtk_scale_new_with_range(GTK_ORIENTATION_HORIZONTAL, 1, 10, 1);
    gtk_range_set_value(GTK_RANGE(slider), ops.quantum);
    gtk_box_pack_start(GTK_BOX(vbox), slider, FALSE, FALSE, 0);
    g_signal_connect(slider, "value-changed", G_CALLBACK(on_slider_value_changed), quantum_label);
    GtkWidget *separator1 = gtk_separator_new(GTK_ORIENTATION_HORIZONTAL);
    gtk_box_pack_start(GTK_BOX(vbox), separator1, FALSE, FALSE, 0);

    GtkWidget *config_label = gtk_label_new("Process generation: ");
    gtk_widget_set_margin_top(config_label, 15);
    gtk_widget_set_margin_bottom(config_label, 15);
    gtk_box_pack_start(GTK_BOX(vbox), config_label, FALSE, FALSE, 0);
    gtk_widget_override_font(config_label, sub_font_desc);
    GtkWidget *max_proc_hbox = gtk_hbox_new(FALSE, 20);
    GtkWidget *max_proc_label = gtk_label_new("Max processes: ");
    max_proc_input = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(max_proc_input), "Ex: 2-10");
    gtk_widget_set_size_request(max_proc_input, 10, 20);
    gtk_box_pack_start(GTK_BOX(max_proc_hbox), max_proc_label, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(max_proc_hbox), max_proc_input, FALSE, FALSE, 0);
    gtk_widget_set_margin_bottom(max_proc_hbox, 15);
    gtk_widget_set_margin_left(max_proc_hbox, 60);
    gtk_box_pack_start(GTK_BOX(vbox), max_proc_hbox, FALSE, FALSE, 0);

    GtkWidget *max_exec_hbox = gtk_hbox_new(FALSE, 20);
    GtkWidget *max_exec_label = gtk_label_new("Burst time limit: ");
    max_exec_input = gtk_entry_new();
    gtk_widget_set_size_request(max_exec_input, 10, 20);
    gtk_entry_set_placeholder_text(GTK_ENTRY(max_exec_input), "Ex: 3-10");
    gtk_widget_set_margin_bottom(max_exec_hbox, 15);
    gtk_box_pack_start(GTK_BOX(max_exec_hbox), max_exec_label, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(max_exec_hbox), max_exec_input, FALSE, FALSE, 0);
    gtk_widget_set_margin_left(max_exec_hbox, 60);
    gtk_box_pack_start(GTK_BOX(vbox), max_exec_hbox, FALSE, FALSE, 0);

    GtkWidget *max_priority_hbox = gtk_hbox_new(FALSE, 20);
    GtkWidget *max_priority_label = gtk_label_new("Priority limit: ");
    max_priority_input = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(max_priority_input), "Ex: 3-10");
    gtk_widget_set_size_request(max_priority_input, 10, 20);
    gtk_box_pack_start(GTK_BOX(max_priority_hbox), max_priority_label, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(max_priority_hbox), max_priority_input, FALSE, FALSE, 0);
    gtk_widget_set_margin_left(max_priority_hbox, 60);
    gtk_box_pack_start(GTK_BOX(vbox), max_priority_hbox, FALSE, FALSE, 0);
    gtk_widget_set_margin_bottom(max_priority_hbox, 15);
    GtkWidget *max_arrival_hbox = gtk_hbox_new(FALSE, 20);
    GtkWidget *max_arrival_label = gtk_label_new("Arrival limit: ");

    max_arrival_input = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(max_arrival_input), "Ex: 3-10");
    gtk_widget_set_size_request(max_arrival_input, 10, 20);
    gtk_box_pack_start(GTK_BOX(max_arrival_hbox), max_arrival_label, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(max_arrival_hbox), max_arrival_input, FALSE, FALSE, 0);
    gtk_widget_set_margin_left(max_arrival_hbox, 60);
    gtk_box_pack_start(GTK_BOX(vbox), max_arrival_hbox, FALSE, FALSE, 0);

    GtkWidget *save_btn = gtk_button_new();
    gtk_widget_set_margin_top(save_btn, 25);
    gtk_button_set_label(save_btn, (gchar *)"Save");

    g_signal_connect(save_btn, "clicked", G_CALLBACK(save_settings), NULL);
    gtk_box_pack_start(GTK_BOX(vbox), save_btn, FALSE, FALSE, 0);
    gtk_container_add(GTK_CONTAINER(settings_window), vbox);
    gtk_widget_show_all(settings_window);
}

void close_settings_window()
{
    gtk_window_close(GTK_WINDOW(settings_window));
}

void show_metrics_window()
{
    metrics_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    g_signal_connect(metrics_window, "delete-event", G_CALLBACK(on_delete_event), GINT_TO_POINTER(METRICS_WINDOW));

    gtk_window_set_title(GTK_WINDOW(metrics_window), "Metrics");
    gtk_window_set_resizable(GTK_WINDOW(metrics_window), FALSE);
    metrics_table = draw_metrics_table();
    gtk_container_add(GTK_CONTAINER(metrics_window), metrics_table);
    gtk_widget_show_all(metrics_window);
    is_metrics_open = true;
}

void close_metrics_window()
{
    gtk_window_close(GTK_WINDOW(metrics_window));
}

void update_metrics_window()
{
    if (metrics_window == NULL || metrics_table == NULL)
    {
        return;
    }

    gtk_widget_set_visible(metrics_table, TRUE);
    gtk_container_remove(GTK_CONTAINER(metrics_window), metrics_table);
    if (metrics_table != NULL)
    {
        gtk_widget_destroy(metrics_table);
    }

    metrics_table = draw_metrics_table();

    if (metrics_table != NULL)
    {
        gtk_container_add(GTK_CONTAINER(metrics_window), metrics_table);
        gtk_widget_show_all(metrics_window);
        gtk_widget_queue_draw(GTK_WIDGET(metrics_table));
    }
    else
    {
        g_print("Error creating metrics table\n");
    }
}

static void on_algorithm_selected(GtkMenuItem *menuitem, gpointer fnc)
{
    load_algorithm(GPOINTER_TO_INT(fnc));
    update_metrics_window();
}
gboolean save_pixbuf_to_png(GdkPixbuf *pixbuf, const gchar *filename)
{
    if (pixbuf == NULL || filename == NULL)
    {
        g_printerr("Invalid arguments for save_pixbuf_to_png\n");
        return FALSE;
    }

    GError *error = NULL;
    gboolean success = gdk_pixbuf_savev(pixbuf, filename, "png", NULL, NULL, &error);

    if (!success)
    {
        g_printerr("Error saving Pixbuf to PNG: %s\n", error->message);
        g_error_free(error);
    }

    return success;
}
void save_to_png(GtkWidget *drawing_area)
{

    gint width, height;
    gtk_window_get_size(GTK_WINDOW(window), &width, &height);
    GdkPixbuf *pixbuf = gdk_pixbuf_get_from_window(
        gtk_widget_get_window(drawing_area),
        0, 0,
        width, height);

    if (pixbuf)
    {
        save_pixbuf_to_png(pixbuf, "output.png");
        g_object_unref(pixbuf);
    }
}

static gboolean on_draw_event(GtkWidget *widget, cairo_t *cr, gpointer data)
{
    gint width = gtk_widget_get_allocated_width(widget);
    gint height = gtk_widget_get_allocated_height(widget);
    double bar_height = height / (double)(executed_tasks_size + 1);
    double bar_width = width / (double)tasks[executed_tasks_size - 1].finish;
    cairo_set_source_rgb(cr, 0, 0, 0);
    cairo_set_line_width(cr, 2.0);
    cairo_move_to(cr, 0, height - bar_height);
    cairo_line_to(cr, width, height - bar_height);
    cairo_stroke(cr);

    for (int i = 0; i <= executed_tasks_size; i++)
    {
        double y = i * bar_height;

        cairo_move_to(cr, 0, y);
        cairo_line_to(cr, width, y);
        cairo_stroke(cr);

        if (i < executed_tasks_size)
        {
            cairo_set_source_rgb(cr, 0, 0, 0);
            cairo_move_to(cr, 5, y + bar_height / 2);
            cairo_show_text(cr, tasks[i].label);
            char start_time_str[10];
            snprintf(start_time_str, sizeof(start_time_str), "%d", tasks[i].start);
            cairo_move_to(cr, tasks[i].start * bar_width, height - bar_height / 2);
            cairo_show_text(cr, start_time_str);
            char end_time_str[10];
            snprintf(end_time_str, sizeof(end_time_str), "%d", tasks[i].finish);
            cairo_move_to(cr, tasks[i].finish * bar_width, height - bar_height / 2);
            cairo_show_text(cr, end_time_str);
        }
    }

    for (int i = 0; i < executed_tasks_size; i++)
    {
        double x = tasks[i].start * bar_width;
        double y = i * bar_height;
        double task_width = (tasks[i].finish - tasks[i].start) * bar_width;

        cairo_set_source_rgba(cr, 0, 0, 1, 0.4);
        cairo_rectangle(cr, x, y, task_width, bar_height);
        cairo_fill_preserve(cr);

        cairo_set_source_rgb(cr, 0, 0, 0);
        cairo_set_line_width(cr, 1.0);
        cairo_stroke(cr);

        cairo_set_source_rgb(cr, 0, 0, 0);
        cairo_select_font_face(cr, "Sans", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_BOLD);
        cairo_set_font_size(cr, 10.0);

        cairo_text_extents_t extents;
        cairo_text_extents(cr, tasks[i].label, &extents);

        double text_x = x + (task_width - extents.width) / 2;
        double text_y = y + (bar_height + extents.height) / 2;

        cairo_move_to(cr, text_x, text_y);
        cairo_show_text(cr, tasks[i].label);
    }

    return FALSE;
}
void show_message_box_(const gchar *message)
{
    GtkDialogFlags flags = GTK_DIALOG_DESTROY_WITH_PARENT;
    GtkWidget *dialog = gtk_message_dialog_new(GTK_WINDOW(window),
                                               flags,
                                               GTK_MESSAGE_OTHER,
                                               GTK_BUTTONS_CLOSE,
                                               "%s", message);

    g_signal_connect_swapped(dialog, "response",
                             G_CALLBACK(gtk_widget_destroy),
                             dialog);
    gtk_widget_show(dialog);
}

static void on_option_selected(GtkMenuItem *menuitem, gpointer fnc)
{
    switch (GPOINTER_TO_INT(fnc))
    {
    case GEN_FILE:
        generate_config_file(ops, max_proc_range, exec_range, priority_range, arrival_range);
        proc_head = read_config_file("generated_config.json", &config_file_size, &ops);
        if (proc_head != NULL)
        {
            show_message_box_("Successfully generated config file.");
            load_algorithm(current_algorithm);

            gtk_widget_queue_draw(drawing_area);
        }

        break;

    case OPEN_METRICS:

        if (!gtk_check_menu_item_get_active(GTK_CHECK_MENU_ITEM(menuitem)))
        {
            close_metrics_window();
        }
        else
        {
            show_metrics_window();
        }
        break;

    case OPEN_SETTINGS:
        if (!gtk_check_menu_item_get_active(GTK_CHECK_MENU_ITEM(menuitem)))
        {
            close_settings_window();
        }
        else
        {
            show_settings_window();
        }
        break;

    case EXPORT_PNG:
        save_to_png(drawing_area);
        break;
    }
}

static GtkWidget *create_menu(GtkWidget *drawing_area)
{

    GtkWidget *menubar, *menu, *menuitem, *options_menu, *generate_config, *options_, *view_menu, *view, *export, *export_png, *export_sub;
    menubar = gtk_menu_bar_new();

    menu = gtk_menu_new();
    menuitem = gtk_menu_item_new_with_label("First Come First Serve (FIFO)");
    g_signal_connect(menuitem, "activate", G_CALLBACK(on_algorithm_selected), GINT_TO_POINTER(FIFO));
    gtk_menu_shell_append(GTK_MENU_SHELL(menu), menuitem);
    menuitem = gtk_menu_item_new_with_label("Priority (preemptive)");
    g_signal_connect(menuitem, "activate", G_CALLBACK(on_algorithm_selected), GINT_TO_POINTER(PRIORITY_P));
    gtk_menu_shell_append(GTK_MENU_SHELL(menu), menuitem);
    menuitem = gtk_menu_item_new_with_label("Priority (non-preemptive)");
    g_signal_connect(menuitem, "activate", G_CALLBACK(on_algorithm_selected), GINT_TO_POINTER(PRIORITY));
    gtk_menu_shell_append(GTK_MENU_SHELL(menu), menuitem);
    menuitem = gtk_menu_item_new_with_label("Round Robin");
    g_signal_connect(menuitem, "activate", G_CALLBACK(on_algorithm_selected), GINT_TO_POINTER(RR));
    gtk_menu_shell_append(GTK_MENU_SHELL(menu), menuitem);
    menuitem = gtk_menu_item_new_with_label("Shortest Remaining Time (SRT)");
    g_signal_connect(menuitem, "activate", G_CALLBACK(on_algorithm_selected), GINT_TO_POINTER(SRT));
    gtk_menu_shell_append(GTK_MENU_SHELL(menu), menuitem);
    menuitem = gtk_menu_item_new_with_label("Shortest Job First (SJF)");
    g_signal_connect(menuitem, "activate", G_CALLBACK(on_algorithm_selected), GINT_TO_POINTER(SJF));
    gtk_menu_shell_append(GTK_MENU_SHELL(menu), menuitem);
    menuitem = gtk_menu_item_new_with_label("Multilevel");
    g_signal_connect(menuitem, "activate", G_CALLBACK(on_algorithm_selected), GINT_TO_POINTER(MULTILEVEL));
    gtk_menu_shell_append(GTK_MENU_SHELL(menu), menuitem);

    menuitem = gtk_menu_item_new_with_label("Algorithms");
    gtk_menu_item_set_submenu(GTK_MENU_ITEM(menuitem), menu);
    gtk_menu_shell_append(GTK_MENU_SHELL(menubar), menuitem);

    options_menu = gtk_menu_new();
    generate_config = gtk_menu_item_new_with_label("Generate .json config file");
    g_signal_connect(generate_config, "activate", G_CALLBACK(on_option_selected), GINT_TO_POINTER(GEN_FILE));
    gtk_menu_shell_append(GTK_MENU_SHELL(options_menu), generate_config);

    export = gtk_menu_item_new_with_label("Export to");
    gtk_menu_shell_append(GTK_MENU_SHELL(options_menu), export);

    export_sub = gtk_menu_new();
    export_png = gtk_menu_item_new_with_label(".png");
    g_signal_connect(export_png, "activate", G_CALLBACK(on_option_selected), GINT_TO_POINTER(EXPORT_PNG));
    gtk_menu_shell_append(GTK_MENU_SHELL(export_sub), export_png);
    gtk_menu_item_set_submenu(GTK_MENU_ITEM(export), export_sub);

    options_ = gtk_menu_item_new_with_label("Options");
    gtk_menu_item_set_submenu(GTK_MENU_ITEM(options_), options_menu);

    gtk_menu_shell_append(GTK_MENU_SHELL(menubar), options_);

    view_menu = gtk_menu_new();
    open_metrics = gtk_check_menu_item_new_with_label("View Metrics");
    g_signal_connect(open_metrics, "activate", G_CALLBACK(on_option_selected), GINT_TO_POINTER(OPEN_METRICS));
    gtk_menu_shell_append(GTK_MENU_SHELL(view_menu), open_metrics);
    view_settings = gtk_check_menu_item_new_with_label("View Settings");
    g_signal_connect(view_settings, "activate", G_CALLBACK(on_option_selected), GINT_TO_POINTER(OPEN_SETTINGS));

    gtk_menu_shell_append(GTK_MENU_SHELL(view_menu), view_settings);
    view = gtk_menu_item_new_with_label("View");
    gtk_menu_item_set_submenu(GTK_MENU_ITEM(view), view_menu);
    gtk_menu_shell_append(GTK_MENU_SHELL(menubar), view);
    return menubar;
}

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        printf("Please input the config file.\n");
        exit(0);
    }
    else
    {

        ops.algorithm = 10;
        ops.quantum = 3;

        if (strcmp(argv[1], "G") == 0 || strcmp(argv[1], "g") == 0)
        {
            generate_config_file(ops, max_proc_range, exec_range, priority_range, arrival_range);
            return 0;
        }

        proc_head = read_config_file(argv[1], &config_file_size, &ops);
    }

    gtk_init(&argc, &argv);

    srand(time(NULL));
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);

    load_algorithm(current_algorithm);
    gtk_window_set_default_size(GTK_WINDOW(window), 800, 600);
    g_signal_connect(G_OBJECT(window), "destroy", G_CALLBACK(gtk_main_quit), NULL);

    vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_container_add(GTK_CONTAINER(window), vbox);

    GtkWidget *menubar = create_menu(drawing_area);
    gtk_box_pack_start(GTK_BOX(vbox), menubar, FALSE, FALSE, 0);

    drawing_area = gtk_drawing_area_new();
    gtk_widget_set_size_request(drawing_area, 600, 400);
    g_signal_connect(G_OBJECT(drawing_area), "draw", G_CALLBACK(on_draw_event), NULL);
    gtk_box_pack_start(GTK_BOX(vbox), drawing_area, TRUE, TRUE, 0);

    gtk_widget_show_all(window);

    gtk_main();
    if (proc_head != NULL)
    {
        free(proc_head);
    }
    return 0;
}
