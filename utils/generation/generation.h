
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