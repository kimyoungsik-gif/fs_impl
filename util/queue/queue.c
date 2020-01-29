#include "queue.h"
#include <stdlib.h>

void queue_push (struct queue *queue, void *data) {
	struct queue_item *item = (struct queue_item *) malloc (sizeof(struct queue_item));
	item->data = data;
	item->next = NULL;
	queue->size++;

	if (!queue->front) {
		queue->front = item;
		queue->tail = item;
		return;
	}
	queue->tail->next = item;
	queue->tail = item;

	return;
}

void *queue_front (struct queue *queue) {
	struct queue_item *cur = queue->front;

	if (cur) {
		return cur->data;
	}
	else {
		return NULL;
	}
}

void *queue_pop (struct queue *queue) {
	struct queue_item *cur = queue->front;
	void *data;

	if (cur) {
		data = cur->data;
	}
	else {
		return NULL;
	}
	queue->size--;
	queue->front = cur->next;
	free(cur);

	return data;
}

int queue_empty (struct queue *queue) {
	return !queue->size;
}

int queue_size (struct queue *queue) {
	return queue->size;
}

struct queue *queue_init() {
	struct queue *q = (struct queue *) malloc (sizeof (struct queue));
	q->front = NULL;
	q->tail = NULL;
	q->size = 0;

	return q;
}

void queue_destroy(struct queue *queue) {
	while (queue_pop(queue));

	free(queue);

	return;
}

