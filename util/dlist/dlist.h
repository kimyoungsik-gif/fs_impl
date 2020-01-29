#ifndef __DLIST__
#define __DLIST__

struct dlist_item {
	void *data;
	struct dlist_item *next;
	struct dlist_item *prev;
};

struct dlist {
	struct dlist_item *front;
	struct dlist_item *tail;
	int size;
};

struct dlist_item *dlist_add_front (struct dlist *dlist, void *data);
struct dlist_item *dlist_add_tail (struct dlist *dlist, void *data);
struct dlist_item *dlist_search (struct dlist *dlist, void *data, int data_size);
void* dlist_del_front (struct dlist *dlist);
void* dlist_del_tail (struct dlist *dlist);
void* dlist_del (struct dlist *dlist, struct dlist_item *item);
void* dlist_move_to_tail (struct dlist *dlist, struct dlist_item *item);
int dlist_empty (struct dlist *dlist);
int dlist_size (struct dlist *dlist);

struct dlist *dlist_init ();
void dlist_destroy (struct dlist *dlist);

#endif
