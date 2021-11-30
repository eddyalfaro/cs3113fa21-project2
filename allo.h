#include <stdlib.h>
#include "node.h"
#include "process.h"

#ifndef ALLO_H
#define ALLO_H

static size_t alloc_mmry = 0;

size_t getFreeSpace(node* node, size_t* elmnt){
	*elmnt = *elmnt + 1;
	prcss* crrnt = (prcss*) node->data;
	if (*elmnt == 1)return crrnt->base;
	 
	if (node->next == NULL)return (MEMORY - (crrnt->base + crrnt->mmry));
 
	prcss* next = (prcss*) node->next->data;
	return next->base - (crrnt->base + crrnt->mmry);
}

node* firstFit(node** list, prcss* _prcss){
	if (MEMORY < _prcss->mmry) return NULL;
	if (_prcss->mmry > (MEMORY - alloc_mmry)) {
		return NULL;				//not enough memory
	}
	
	if (*list == NULL) {
		*list = create_node(_prcss);
		alloc_mmry += _prcss->mmry;
		return *list;				//first ellement allocation
	}
	
	size_t elmnt = 0;
	size_t free_pg_sz = getFreeSpace(*list, &elmnt);

	if (free_pg_sz > _prcss->mmry) {
		*list = insert_before(*list, _prcss, NULL);
		alloc_mmry += _prcss->mmry;
		return *list;				//allocation before the first 
	}						//element in the list

	node* tgt_node = *list;

	while (tgt_node != NULL){
		free_pg_sz = getFreeSpace(tgt_node, &elmnt);
		if (free_pg_sz > _prcss->mmry){
			alloc_mmry += _prcss->mmry; 
			return insert_after(tgt_node, _prcss, prcss_set_base);
		}
		tgt_node = tgt_node->next;
	}

	return NULL;	
}


#endif
