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

// TYPES
#include "./types.h"
#include "./utils/algorithms/queue_data_struct.c"

// ALGORITHMS
#include "./utils/algorithms/useful.c"
#include "./utils/algorithms/round_robin.c"
#include "./utils/algorithms/FIFO.c"
#include "./utils/algorithms/priority.c"
#include "./utils/algorithms/multilevel.c"
#include "./utils/algorithms/SRT.c"

// File generation
#include "./utils/generation/generation.c"

// Config filename
const char *filename = "generated_config.json";
int config_file_size = 0;
int executed_tasks_size = 0;
ExecutedTask tasks[100];
process *proc_head;
GtkWidget *window, *drawing_area, *vbox;
options ops;

process *read_config_file(const char *filename)
{
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
// Options
enum {
    GEN_FILE
};

// Algorithm options
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

static gboolean on_draw_event(GtkWidget *widget, cairo_t *cr, gpointer data)
{
    gint width = gtk_widget_get_allocated_width(widget);
    gint height = gtk_widget_get_allocated_height(widget);
    double bar_height = height / (double)(executed_tasks_size + 1);
    double bar_width = width / (double)tasks[executed_tasks_size - 1].finish;
    cairo_set_source_rgb(cr, 0, 0, 0); // Black color
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
            cairo_set_source_rgb(cr, 0, 0, 0); // Black color
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

    // Draw tasks
    for (int i = 0; i < executed_tasks_size; i++)
    {
        double x = tasks[i].start * bar_width;
        double y = i * bar_height;
        double task_width = (tasks[i].finish - tasks[i].start) * bar_width;

        cairo_set_source_rgba(cr, 0, 0, 1, 0.4);
        cairo_rectangle(cr, x, y, task_width, bar_height);
        cairo_fill_preserve(cr);

        cairo_set_source_rgb(cr, 0, 0, 0); // Black color
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

static void on_option_selected(GtkMenuItem *menuitem, gpointer fnc)
{
 switch (GPOINTER_TO_INT(fnc))
    {
        case GEN_FILE:
            generate_config_file(ops);
            proc_head = read_config_file("generated_config.json");
            break;
    }
}static void on_algorithm_selected(GtkMenuItem *menuitem, gpointer fnc)
{
    ExecutedTask *task = NULL;
    executed_tasks_size = 0;

    switch (GPOINTER_TO_INT(fnc))
    {
        case FIFO:
            task = get_fifo_output(proc_head, config_file_size, &executed_tasks_size);
            break;

        case SRT:
            task = get_srt_output(proc_head, config_file_size, &executed_tasks_size);
            break;

        case PRIORITY_P:
            task = get_priority_output(proc_head, config_file_size, true, &executed_tasks_size);
            break;

        case PRIORITY:
            task = get_priority_output(proc_head, config_file_size, false, &executed_tasks_size);
            break;

        case RR:
            task = get_round_robin_output(ops.quantum, proc_head, config_file_size, &executed_tasks_size);
            break;

        default:
            break;
    }

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


// Create the menu
static GtkWidget *create_menu(GtkWidget *drawing_area)
{

    GtkWidget *menubar, *menu, *menuitem, *options_menu, *generate_config, *options_;
    menubar = gtk_menu_bar_new();

    // ALGORITHMS MENU
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
    menuitem = gtk_menu_item_new_with_label("Algorithm");
    gtk_menu_item_set_submenu(GTK_MENU_ITEM(menuitem), menu);
    gtk_menu_shell_append(GTK_MENU_SHELL(menubar), menuitem);

    //OPTIONS MENU
    options_menu = gtk_menu_new();
    generate_config = gtk_menu_item_new_with_label("Generate .json config file");
    g_signal_connect(generate_config, "activate", G_CALLBACK(on_option_selected), GINT_TO_POINTER(GEN_FILE));
    gtk_menu_shell_append(GTK_MENU_SHELL(options_menu), generate_config);
    options_ = gtk_menu_item_new_with_label("Options");
    gtk_menu_item_set_submenu(GTK_MENU_ITEM(options_), options_menu);

    gtk_menu_shell_append(GTK_MENU_SHELL(menubar), options_);

    return menubar;
}

// Main function
int main(int argc, char *argv[])
{
    printf("test");

    if (argc < 2)
    {
        printf("Please input the config file.\n");
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

        // Lire fichier configuration
        proc_head = read_config_file(argv[1]);
        ExecutedTask *task = get_fifo_output(proc_head, config_file_size, &executed_tasks_size);

        if (task != NULL)
        {
            for (int i = 0; i < executed_tasks_size; i++)
            {
                tasks[i] = task[i];
            }
        }
    }

    gtk_init(&argc, &argv);

    srand(time(NULL));
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Process scheduler");
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
#endif //SCHEDULER_H