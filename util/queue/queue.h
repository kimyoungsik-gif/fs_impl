#ifndef __QUEUE__
#define __QUEUE__

struct queue_item {
	void *data;
	struct queue_item *next;
};

struct queue {
	struct queue_item *front;
	struct queue_item *tail;
	int size;
};

void queue_push (struct queue *queue, void *data);
void *queue_front (struct queue *queue);
void *queue_pop (struct queue *queue);
int queue_empty (struct queue *queue);
int queue_size (struct queue *queue);

struct queue *queue_init();
void queue_destroy(struct queue *queue);

#endif
