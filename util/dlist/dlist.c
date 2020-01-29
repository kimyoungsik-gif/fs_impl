#include "dlist.h"
#include <stdlib.h>
#include <string.h>

struct dlist_item *dlist_add_front (struct dlist *dlist, void *data) {
	struct dlist_item *item = (struct dlist_item *) malloc (sizeof(struct dlist_item));
	item->data = data;

	if (!dlist->size) {
		dlist->front = item;
		dlist->tail = item;
		item->next = NULL;
		item->prev = NULL;
		dlist->size++;
		return item;
	}
	dlist->front->prev = item;
	item->next = dlist->front;
	item->prev = NULL;

	dlist->front = item;
	dlist->size++;

	return item;
}

struct dlist_item *dlist_add_tail (struct dlist *dlist, void *data) {
	struct dlist_item *item = (struct dlist_item *) malloc (sizeof(struct dlist_item));
	item->data = data;

	if (!dlist->size) {
		dlist->front = item;
		dlist->tail = item;
		item->next = NULL;
		item->prev = NULL;
		dlist->size++;
		return item;
	}
	dlist->tail->next = item;
	item->prev = dlist->tail;
	item->next = NULL;

	dlist->tail = item;
	dlist->size++;

	return item;
}

struct dlist_item *dlist_search (struct dlist *dlist, void *data, int data_size) {
	struct dlist_item *cur;

	if (!dlist->size) {
		return NULL;
	}

	for (cur = dlist->front; cur != NULL; cur = cur->next) {
		if (memcmp(cur->data, data, data_size) == 0) {
			return cur;
		}
	}
	return NULL;
}

void* dlist_del_front (struct dlist *dlist) {
	struct dlist_item *cur = dlist->front;
	void *data;

	if (!dlist->size || !cur) {
		return NULL;
	}

	if (dlist->size == 1) {
		data = cur->data;
		dlist->size--;
		dlist->front = dlist->tail = NULL;
		return data;
	}

	data = cur->data;

	dlist->size--;
	dlist->front = cur->next;
	dlist->front->prev = NULL;
	free(cur);

	return data;
}

void* dlist_del_tail (struct dlist *dlist) {
	struct dlist_item *cur = dlist->tail;
	void *data;

	if (!dlist->size || !cur) {
		return NULL;
	}
	
	if (dlist->size == 1) {
		data = cur->data;
		dlist->size--;
		dlist->front = dlist->tail = NULL;
		return data;
	}

	data = cur->data;

	dlist->size--;
	dlist->tail = cur->prev;
	dlist->tail->next = NULL;
	free(cur);

	return data;
}

void* dlist_del (struct dlist *dlist, struct dlist_item *item) {
	struct dlist_item *next_item;
	struct dlist_item *prev_item;
	void *data;

	if (!dlist->size || !item) {
		return NULL;
	}

	if (dlist->front == item) {
		data = dlist_del_front(dlist);
		return data;
	}

	if (dlist->tail == item) {
		data = dlist_del_tail(dlist);
		return data; 

	}

	next_item = item->next;
	prev_item = item->prev;

	next_item->prev = prev_item;
	prev_item->next = next_item;

	data = item->data;

	dlist->size--;
	free(item);

	return data;
}

void* dlist_move_to_tail (struct dlist *dlist, struct dlist_item *item) {
	struct dlist_item *next_item;
	struct dlist_item *prev_item;

	if (!dlist->size || !item) {
		return NULL;
	}

	if (dlist->tail == item) {
		return item->data;
	}

	if (dlist->front == item) {
		dlist->front = item->next;
		dlist->front->prev = NULL;

		dlist->tail->next = item;
		item->prev = dlist->tail;
		item->next = NULL;
		dlist->tail = item;

		return item->data;
	}

	next_item = item->next;
	prev_item = item->prev;

	next_item->prev = prev_item;
	prev_item->next = next_item;

	dlist->tail->next = item;
	item->prev = dlist->tail;
	item->next = NULL;
	dlist->tail = item;

	return item->data;
}


int dlist_empty (struct dlist *dlist) {
	return !dlist->size;
}

int dlist_size (struct dlist *dlist) {
	return dlist->size;
}

struct dlist *dlist_init () {
	struct dlist *d = (struct dlist *) malloc (sizeof (struct dlist));
	d->front = NULL;
	d->tail = NULL;
	d->size = 0;

	return d;
}

void dlist_destroy (struct dlist *dlist) {
	struct dlist_item *d_item, *cur;

	if (!dlist->front) {
		free(dlist);
		return;
	}

	for (cur = dlist->front; cur != NULL;) {
		d_item = cur;
		cur = d_item->next;
		free(d_item);
	}
	free(dlist);

	return;
}

