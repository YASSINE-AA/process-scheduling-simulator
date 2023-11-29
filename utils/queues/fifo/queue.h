
typedef struct proc_in_queue
{
    process value;
    struct proc_in_queue *next;
} proc_in_queue;

typedef struct proc_queue
{
    struct proc_in_queue *tail;
    struct proc_in_queue *head;

} proc_queue;

