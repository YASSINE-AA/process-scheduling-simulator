
// Process Type
typedef struct process
{
    int arrived_at;
    int execution_time;
    int priority;
    char name[100];
} process;


// Options Type
typedef struct options
{
    int quantum;
    int algorithm;
} options;

// ================ FIFO ======================

// Queue Element (process in queue)
typedef struct proc_in_queue
{
    process value;
    struct proc_in_queue* next;
} proc_in_queue;


// Queue Type
typedef struct proc_queue
{
    struct proc_in_queue* tail;
    struct proc_in_queue* head;
    
} proc_queue;

typedef struct
{
    int start;
    int finish;
    const char* label;
    int color[4];
} ExecutedTask;
