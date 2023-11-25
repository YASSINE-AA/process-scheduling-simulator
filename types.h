typedef struct process
{
    int arrived_at;
    int execution_time;
    int priority;
    char name[100];
    bool execute_full;
} process;

typedef struct options
{
    int quantum;
    int algorithm;
} options;

typedef struct proc_in_queue
{
    process value;
    struct proc_in_queue* next;
} proc_in_queue;

typedef struct proc_queue
{
    struct proc_in_queue* tail;
    struct proc_in_queue* head;
    
} proc_queue;

typedef struct
{
    int arrival_time;
    int start;
    int finish;
    const char* label;
    int color[4];
} ExecutedTask;

typedef struct priority_queue {
    process* array;
    int size;
    int capacity;
} priority_queue;

typedef struct {
    char* name;
    int finish;
} finish_track;
