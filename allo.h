#include <stdlib.h>
#include "node.h"
#include "process.h"

#ifndef ALLO_H
#define ALLO_H

static size_t alloc_mmry = 0;
static node* last_add = NULL;


size_t getFreeSpace(node* node, size_t* elmnt){
	if (node == NULL) return 0;
	
	prcss* crrnt = (prcss*) node->data;
	if (elmnt != NULL){
		*elmnt = *elmnt + 1;
		if (*elmnt == 1)return crrnt->base;
	}
	 
	if (node->next == NULL)return (MEMORY - (crrnt->base + crrnt->mmry));
 
	prcss* next = (prcss*) node->next->data;
	return next->base - (crrnt->base + crrnt->mmry);
}

void updt_lastAdded(node* list, node* last){
	if (last == NULL || list == NULL) return; 					//nothing added
	
	if (last_add != NULL && find_by_data(list, comp_prcss, last_add->data) == NULL)
		delete_node(last_add, delete_prcss);			//last added recorded
							//is not within the list of allocated
	last_add = last;
}

void listAvailable(node* list){
	if ((MEMORY - alloc_mmry) == 0){
		printf("FULL\n");
		return;
	}

	if (list == NULL && MEMORY > 0){
		printf("(%lu, %d)\n", MEMORY, 0);
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
		
		if (free_pg_sz > 0){ 
			if (iterator->next != NULL)
				printf("(%lu, %lu) ", free_pg_sz, data->mmry + data->base);
			else printf("(%lu, %lu)", free_pg_sz, data->mmry + data->base);
		}
		iterator = iterator->next;
	}

	printf("\n");
}

node* release(node** _list, prcss* target){
	if (target == NULL) return NULL;
	if (_list == NULL) return NULL;

	node* rmvd = remove_node(_list, comp_prcss, target);
	if (rmvd == NULL) return NULL;
	//printf("removed = %lu, next = %lu\n", &(*rmvd), &(*rmvd->next));
	delete_prcss(target);

	//if removed is different than the last added it is completely
	//unlinked from the list
	if (comp_prcss(rmvd->data, last_add->data) != 0){
	//	printf("Unlinking bookmark\n");
		rmvd->next = NULL;
	}

	prcss* rmvd_prcss =  (prcss*) rmvd->data;	
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

node* bestFit(node** list, prcss* _prcss){
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
	node* (*insert) (node*, void*, void (*updt) (void*, void*)) = NULL; 

	size_t free_pg_sz = getFreeSpace(*list, &elmnt);
		
	size_t crrnt_free_pg = MEMORY - alloc_mmry + 1;
	node* crrnt_wnnr = NULL;
	
	if (free_pg_sz >= _prcss->mmry && free_pg_sz < crrnt_free_pg){
		insert = insert_before;
		crrnt_wnnr = *list;
		crrnt_free_pg = free_pg_sz;
	}

	node* iterator = *list;

	while (iterator != NULL){
		free_pg_sz = getFreeSpace(iterator, &elmnt);

		if (free_pg_sz >= _prcss->mmry && free_pg_sz < crrnt_free_pg){
			//printf("FOUND NEW SPACE\n");
			crrnt_free_pg = free_pg_sz;
			crrnt_wnnr = iterator;
			insert = insert_after;
		}
		
		iterator = iterator->next;
	}

	if (crrnt_wnnr != NULL){
		alloc_mmry += _prcss->mmry;
		if (insert == insert_before){
			*list =  insert(crrnt_wnnr, _prcss, prcss_set_base);
			return *list;
		} else return insert(crrnt_wnnr, _prcss, prcss_set_base);
	}

	return NULL;
}

node* worstFit(node** list, prcss* _prcss){
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
	node* (*insert) (node*, void*, void (*updt) (void*, void*)) = NULL; 

	size_t free_pg_sz = getFreeSpace(*list, &elmnt);
		
	size_t crrnt_free_pg = 0;
	node* crrnt_wnnr = NULL;
	
	if (free_pg_sz >= _prcss->mmry && free_pg_sz > crrnt_free_pg){
		insert = insert_before;
		crrnt_wnnr = *list;
		crrnt_free_pg = free_pg_sz;
	}

	node* iterator = *list;

	while (iterator != NULL){
		free_pg_sz = getFreeSpace(iterator, &elmnt);

		if (free_pg_sz >= _prcss->mmry && free_pg_sz > crrnt_free_pg){
			//printf("FOUND NEW SPACE\n");
			crrnt_free_pg = free_pg_sz;
			crrnt_wnnr = iterator;
			insert = insert_after;
		}
		
		iterator = iterator->next;
	}

	if (crrnt_wnnr != NULL){
		alloc_mmry += _prcss->mmry;
		if (insert == insert_before){
			*list =  insert(crrnt_wnnr, _prcss, prcss_set_base);
			return *list;
		} else return insert(crrnt_wnnr, _prcss, prcss_set_base);
	}

	return NULL;
}

node* nextFit(node** list, prcss* _prcss){
	if (MEMORY < _prcss->mmry) return NULL;
	if (_prcss->mmry > (MEMORY - alloc_mmry)) {
		return NULL;				//not enough memory
	}
	
	if (*list == NULL) {
		*list = create_node(_prcss);
		alloc_mmry += _prcss->mmry;
		return *list;				//first ellement allocation
	}

//	printf("bookmark = %d \tlist_head = %d\n", &(*last_add), &(**list));
//	printf("after-Bookmark = %d \tlist_head = %d\n", &(*last_add->next), &(**list));
	
	size_t free_pg_sz;
	node* iterator = last_add;
	
	while(iterator != NULL){
		free_pg_sz = getFreeSpace(iterator, NULL);
		if (free_pg_sz >= _prcss->mmry){
			alloc_mmry += _prcss->mmry;
			return insert_after(iterator, _prcss, prcss_set_base);
		}
		iterator = iterator->next;	
	}
	
	//return firstFit(list, _prcss);
	iterator = *list;
	size_t elmnt = 0;
	
	free_pg_sz = getFreeSpace(iterator, &elmnt);
	if (free_pg_sz >= _prcss->mmry){
		alloc_mmry += _prcss->mmry;
		*list = insert_before(*list, _prcss, NULL);

		//printList(list, print_prcss);
		//printf("*****\n");
		return *list;
	}

	while(iterator != NULL && comp_prcss(iterator->data, last_add->data) != 0){
		free_pg_sz = getFreeSpace(iterator, &elmnt);

		if (free_pg_sz >= _prcss->mmry){
			alloc_mmry += _prcss->mmry;
			return insert_after(iterator, _prcss, prcss_set_base); 
		}

		iterator = iterator->next;
	}

	return NULL;
}

#endif
