#include <stdlib.h>
#include "node.h"
#include "process.h"

#ifndef ALLO_H
#define ALLO_H

static size_t alloc_mmry = 0;
static node* last_add = NULL;


size_t getFreeSpace(node* node, size_t* elmnt){
	*elmnt = *elmnt + 1;
	prcss* crrnt = (prcss*) node->data;
	if (*elmnt == 1)return crrnt->base;
	 
	if (node->next == NULL)return (MEMORY - (crrnt->base + crrnt->mmry));
 
	prcss* next = (prcss*) node->next->data;
	return next->base - (crrnt->base + crrnt->mmry);
}

void updt_lastAdded(node* list, node* last){
	if (last == NULL) return; 					//nothing added
	
	if (list != NULL && find_by_data(list, comp_prcss, last->data) == NULL)
		delete_node(last_add, delete_prcss);			//last added recorded
							//is not within the list of allocated
	last_add = last;
}

void listAvailable(node* list){
	if (MEMORY == alloc_mmry){
		printf("FULL\n");
		return;
	}
	
	size_t elmnt = 0;
	size_t free_pg_sz = getFreeSpace(list, &elmnt);

	if (free_pg_sz > 0) printf("(%lu, %d) ", free_pg_sz, 0);

	node* iterator = list;
	prcss* data = NULL;
	while(iterator != NULL){
		data = (prcss*) iterator->data;
		free_pg_sz = getFreeSpace(iterator, &elmnt);
		
		if (free_pg_sz > 0) 
		if (iterator->next != NULL)
			printf("(%lu, %lu) ", free_pg_sz, data->mmry + data->base);
		else printf("(%lu, %lu)\n", free_pg_sz, data->mmry + data->base);

		iterator = iterator->next;
	}

	//printf("\n");
}

node* release(node* _list, prcss* target){
	if (target == NULL) return NULL;
	if (_list == NULL) return NULL;

	node* rmvd = remove_node(&_list, comp_prcss, target);
	if (rmvd == NULL) return NULL;

	delete_prcss(target);

	//if removed is different than the last added it is completely
	//unlinked from the list
	if (comp_prcss(rmvd->data, last_add->data) != 0){
		rmvd->next = NULL;
	}

	//prcss* rmvd_prcss =  (prcss*) rmvd->data;	
	alloc_mmry -= rmvd_prcss->mmry;

	return rmvd;
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

	if (free_pg_sz >= _prcss->mmry) {
		*list = insert_before(*list, _prcss, NULL);
		alloc_mmry += _prcss->mmry;
		return *list;				//allocation before the first 
	}						//element in the list

	node* tgt_node = *list;

	while (tgt_node != NULL){
		free_pg_sz = getFreeSpace(tgt_node, &elmnt);
		if (free_pg_sz >= _prcss->mmry){
			alloc_mmry += _prcss->mmry; 
			return insert_after(tgt_node, _prcss, prcss_set_base);
		}
		tgt_node = tgt_node->next;
	}

	return NULL;	
}


#endif
