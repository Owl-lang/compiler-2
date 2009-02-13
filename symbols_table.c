#include "symbols_table.h"
#include <stdlib.h> /* malloc */
#include <string.h> /* strcmp */

static int global_num = 0;

struct st_node *
st_node_init(struct st_node *parent)
{
	struct st_node *ret, *ptr;
	
	ret = (__typeof__(ret)) malloc(sizeof(ret));
	ret->num = global_num++;
	ret->parent = parent;
	
	if (parent)
	{
		if (parent->childs)
		{
			ptr = parent->childs;
			while(ptr->childs)
				ptr = ptr->childs;
			ptr->childs = ret;
		}
		else
			parent->childs = ret;
	}

	ret->childs = 0;	
	ret->entries = 0;
	
	return ret;
}

void
st_node_free(struct st_node *node)
{
	__typeof__(node) ptr, ptr2;

	if (!node)
		return;

	ptr = node->childs;

	st_entries_free(node->entries);

	while (ptr)
	{
		ptr2 = ptr->childs;
		st_node_free(ptr);
		ptr = ptr2;
	}

	free(node);
}

struct st_node *
st_node_add_entry(
	struct st_node *node,
	struct st_entry *entry)
{
	if (node->entries)
	{
		__typeof(node->entries) ptr;
		ptr = node->entries;
		while (ptr->next)
			ptr = ptr->next;
		ptr = entry;
	}
	else
		node->entries = entry;

	return node;
}

void
st_entries_free(struct st_entry *entry)
{
	if (!entry)
		return;

	free(entry->name);

	if (entry->next)
		st_entries_free(entry->next);

	free(entry);
}

struct st_entry *
st_entry_init(
	char *name,
	enum st_enum_type type)
{
	struct st_entry *ret;
	ret = (__typeof__(ret)) malloc(sizeof(ret));

	ret->name = name;
	ret->type = type;
	ret->next = 0;

	return ret;
}

/* return the first hierarchical entry */
struct st_entry *
st_node_lookup_entry(
	struct st_node *node,
	char *name)
{
	__typeof__(node) ptrn;
	struct st_entry *ptre, *ret;

	ret = 0;
	ptrn = node;

	while ((!ret) && (ptrn))
	{
		ptre = ptrn->entries;
		while ((!ret) && (ptre))
		{
			if (!strcmp(ptre->name,name))
				ret = ptre;
			else
				ptre = ptre->next;
		}
		ptrn = ptrn->parent;
	}

	return ret;
}
