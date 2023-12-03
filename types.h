typedef struct process
{
    int arrived_at;
    int execution_time;
    int priority;
    char name[100];
    bool ignore_equal;
} process;

typedef struct options
{
    int quantum;
    int algorithm;
} options;

typedef struct
{
    int arrival_time;
    int start;
    int finish;
    const char *label;
    int color[4];
} ExecutedTask;


typedef struct
{
    char *name;
    int finish;
} finish_track;

typedef enum
{
    GEN_FILE,
    OPEN_METRICS, 
    EXPORT_PNG
} MenuFuncs;

typedef enum
{
    FIFO,
    MULTILEVEL,
    PRIORITY,
    PRIORITY_P,
    RR,
    SRT,
    SJF
} Algorithm;
