typedef struct Node
{
    process process;
    struct Node *next;
} Node;

typedef struct priority_queue
{
    Node *front;
} priority_queue;
