// gcc gui.c -o gui `pkg-config --cflags --libs gtk+-3.0` -lcjson

#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <gtk/gtk.h>
#include <gdk/gdk.h>
#include <stdlib.h>
#include <time.h>
#include <stdlib.h>
#include <stdbool.h>
#include <cjson/cJSON.h>
#include <stdio.h>
#include <string.h>

#include "./types.h"
#include "./utils/algorithms/useful.c"
#include "./utils/algorithms/queue_data_struct.c"

#include "./utils/gui/format.c"

#include "./utils/algorithms/round_robin.c"
#include "./utils/algorithms/FIFO.c"
#include "./utils/algorithms/priority.c"
#include "./utils/algorithms/priority_np.c"
#include "./utils/algorithms/multilevel.c"
#include "./utils/algorithms/SRT.c"

#include "./utils/metrics/metrics.c"

#include "./utils/generation/generation.c"

const char *filename = "generated_config.json";
int config_file_size = 0;
int executed_tasks_size = 0;
ExecutedTask tasks[100];
process *proc_head;
GtkWidget *window, *drawing_area, *vbox, *dialog, *metrics_window, *metrics_table;
options ops;

process *read_config_file(const char *filename)
{
    config_file_size = 0;
    process *process_array = malloc(12 * sizeof(process));
    if (process_array == NULL)
    {
        printf("Error: memory allocation failed\n");
        return NULL;
    }

    FILE *fp = fopen(filename, "r");

    if (fp == NULL)
    {
        printf("Error: could not open file %s\n", filename);
        free(process_array);
        return NULL;
    }

    fseek(fp, 0, SEEK_END);
    long file_size = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    char *config = (char *)malloc(file_size + 1);
    if (config == NULL)
    {
        printf("Error: memory allocation failed\n");
        fclose(fp);
        free(process_array);
        return NULL;
    }

    fread(config, 1, file_size, fp);
    config[file_size] = '\0';

    fclose(fp);

    cJSON *config_json = cJSON_Parse(config);
    if (config_json == NULL)
    {
        const char *error_ptr = cJSON_GetErrorPtr();
        if (error_ptr != NULL)
        {
            fprintf(stderr, "Error before: %s\n", error_ptr);
        }
        free(config);
        free(process_array);
        return NULL;
    }

    const cJSON *nested_process = NULL;
    const cJSON *process_list = NULL;
    int status = 0;

    process_list = cJSON_GetObjectItemCaseSensitive(config_json, "process");
    cJSON_ArrayForEach(nested_process, process_list)
    {
        cJSON *arrived_at = cJSON_GetObjectItemCaseSensitive(nested_process, "arrived_at");
        cJSON *execution_time = cJSON_GetObjectItemCaseSensitive(nested_process, "execution_time");
        cJSON *priority = cJSON_GetObjectItemCaseSensitive(nested_process, "priority");
        cJSON *name = cJSON_GetObjectItemCaseSensitive(nested_process, "name");

        if (!cJSON_IsNumber(arrived_at) || !cJSON_IsNumber(execution_time) || !cJSON_IsNumber(priority) || !cJSON_IsString(name))
        {
            status = 0;
            break;
        }
        else
        {
            process proc;
            proc.arrived_at = arrived_at->valueint;
            proc.execution_time = execution_time->valueint;
            proc.priority = priority->valueint;
            strcpy(proc.name, name->valuestring);
            process_array[config_file_size] = proc;
            config_file_size++;
            status = 1;
        }
    }

    cJSON_Delete(config_json);
    free(config);
    return process_array;
}

enum
{
    GEN_FILE
};

typedef enum
{
    FIFO,
    MULTILEVEL,
    PRIORITY,
    PRIORITY_P,
    RR,
    SRT
} Algorithm;

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
        task = get_multilevel_output(proc_head, config_file_size, &executed_tasks_size);
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
    gtk_grid_set_column_spacing(GTK_GRID(table), 40);

    GtkWidget *label1 = gtk_label_new("Process Name");
    gtk_widget_set_halign(label1, GTK_ALIGN_CENTER);
    gtk_widget_set_valign(label1, GTK_ALIGN_FILL);
    gtk_grid_attach(GTK_GRID(table), label1, 0, 0, 1, 1);

    GtkWidget *label2 = gtk_label_new("Waiting time");
    gtk_widget_set_halign(label2, GTK_ALIGN_CENTER);
    gtk_widget_set_valign(label2, GTK_ALIGN_FILL);
    gtk_grid_attach(GTK_GRID(table), label2, 1, 0, 1, 1);

    GtkWidget *label3 = gtk_label_new("Rotation time");
    gtk_widget_set_halign(label3, GTK_ALIGN_CENTER);
    gtk_widget_set_valign(label3, GTK_ALIGN_FILL);
    gtk_grid_attach(GTK_GRID(table), label3, 2, 0, 1, 1);

    int row = 1;
    int col = 1;
    int total_rotation_time = 0;
    int total_waiting_time = 0;
    printf("waiting time for p1: %d", get_waiting_time("p1", tasks, executed_tasks_size, proc_head, config_file_size));
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
    gtk_window_set_default_size(GTK_WINDOW(metrics_window), 300, 10 * row);
    GtkWidget *total_label = gtk_label_new("Average: ");
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

void show_metrics_window()
{
    metrics_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(metrics_window), "Metrics");
    gtk_window_set_resizable(GTK_WINDOW(metrics_window), FALSE);
    metrics_table = draw_metrics_table();
    gtk_container_add(GTK_CONTAINER(metrics_window), metrics_table);
    gtk_widget_show_all(metrics_window);
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

void show_message_box_()
{
    GtkDialogFlags flags = GTK_DIALOG_DESTROY_WITH_PARENT;
    dialog = gtk_message_dialog_new(GTK_WINDOW(window),
                                    flags,
                                    GTK_MESSAGE_OTHER,
                                    GTK_BUTTONS_CLOSE,
                                    "Successfully generated config file.");

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
        generate_config_file(ops);
        proc_head = read_config_file("generated_config.json");
        if (proc_head != NULL)
        {
            show_message_box_();
            load_algorithm(current_algorithm);

            gtk_widget_queue_draw(drawing_area);
        }

        break;
    }
}

static GtkWidget *create_menu(GtkWidget *drawing_area)
{

    GtkWidget *menubar, *menu, *menuitem, *options_menu, *generate_config, *options_;
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
    menuitem = gtk_menu_item_new_with_label("SRT");
    g_signal_connect(menuitem, "activate", G_CALLBACK(on_algorithm_selected), GINT_TO_POINTER(SRT));
    gtk_menu_shell_append(GTK_MENU_SHELL(menu), menuitem);
    menuitem = gtk_menu_item_new_with_label("MULTILEVEL");
    g_signal_connect(menuitem, "activate", G_CALLBACK(on_algorithm_selected), GINT_TO_POINTER(MULTILEVEL));
    gtk_menu_shell_append(GTK_MENU_SHELL(menu), menuitem);
    menuitem = gtk_menu_item_new_with_label("Algorithm");
    gtk_menu_item_set_submenu(GTK_MENU_ITEM(menuitem), menu);
    gtk_menu_shell_append(GTK_MENU_SHELL(menubar), menuitem);

    options_menu = gtk_menu_new();
    generate_config = gtk_menu_item_new_with_label("Generate .json config file");
    g_signal_connect(generate_config, "activate", G_CALLBACK(on_option_selected), GINT_TO_POINTER(GEN_FILE));
    gtk_menu_shell_append(GTK_MENU_SHELL(options_menu), generate_config);
    options_ = gtk_menu_item_new_with_label("Options");
    gtk_menu_item_set_submenu(GTK_MENU_ITEM(options_), options_menu);

    gtk_menu_shell_append(GTK_MENU_SHELL(menubar), options_);

    return menubar;
}

int main(int argc, char *argv[])
{
    printf("test");

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
            generate_config_file(ops);
            return 0;
        }

        proc_head = read_config_file(argv[1]);
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
    show_metrics_window();
    gtk_main();
    if (proc_head != NULL)
    {
        free(proc_head);
    }
    return 0;
}
#endif