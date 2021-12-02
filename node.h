#ifndef _LIST_H
#define _LIST_H

typedef struct node{
	void* data;
	struct node* next;
}node;

/*	creates an unlinked node within the list
*	Arguments: a pointer to the data the node will containe within the list
*/
node* create_node(void* data){
	if (data == NULL) return NULL;
	node* temp = malloc(sizeof(node));
	
	if (temp != NULL){
		temp->data = data;
		temp->next = NULL;
	}

	return temp;
}

/*	prints a songle node into the standard output
*	Arguments: node* pntr, a pointer to the node which is going to be printed
*		void (*func) (void*) a pointer to a function that knows how to print the
*		contained data.
*/
void printNode(node* pntr, void (*func) (void*)){
	if (func == NULL) return;
	func(pntr->data);
}

/*	prints the data within the list that follow after the given _head
*	the print operation is inclusive printing goes from [_head, end]
*	Arguments: _head, the address of the pntr that contains the head of the list
*		func, the function that determines how the data stored within the list is
*		printed
*/
void printList(node** _head, void (*func) (void*)){
	if (*_head == NULL) {printf("\n"); return;}

	printNode(*_head, func);
	printf(" ");
	printList(&((*_head)->next), func);
}

/*	inserts a new node with the given data immediately after the given node
*	Arguments: crrnt, aftert this node the data will be added
*		dt_updt, is a pointer to a function that updates variables of the 
*		given data. if this pointer is NULL no necesary actions are required within
*		the data stored. the arguments of the function are (crrnt->data, data)
*/
node* insert_after(node* crrnt, void* data, void (*dt_updt) (void*, void*)){
	node* temp = create_node(data);
	if (dt_updt != NULL) dt_updt(crrnt->data, temp->data);
	if (temp != NULL){
		temp->next = crrnt->next;
		crrnt->next = temp;
	}
	return temp;
}

node* insert_before(node* crrnt, void* data, void (*dt_updt) (void*, void*)){
	node*temp = create_node(data);
	if (dt_updt != NULL) dt_updt(crrnt->data, temp->data);
	if (temp != NULL){
		temp->next = crrnt;
	}
	return temp;
}

/*	used with queues implementations of the structure. Queues the given data at the end
*	Arguments: tail, the last element present in the list
*		data, the data that is going to be queue up
*
*	if the user does not have a pointer to the tail application can be done as follows
*		node* tail = enqueue(find_last(head), data);
*/
node* enqueue_data(node* tail, void* data){
	return insert_after(tail, data, NULL);
}

node* enqueue_node(node* tail, node* toAdd){
	tail->next = toAdd;
	toAdd->next = NULL;
	return toAdd;
}

/*	removes the first element in the queue
*	Arguments: queue, the queue implementation of the structure
*/
node* pop(node** stack){
	if (*stack == NULL) return NULL;
	node* temp = *stack;
	*stack = temp->next;
	temp->next = NULL;
	return temp;
}

node* dequeue(node** queue){
	return pop(queue);
}

/*	creates a new node with the shallow copy of the data presented in the first element
*	of the queue.
*	Arguments: queue, the address pointing to the pointer of the head of the queue
*/

node* peek(node** queue){
	return create_node((*queue)->data);
}

/*	returns the last element in the queue
*	Arguments: the pnts of the head of the queue
*/
node* find_last(node* head){
	if (head == NULL) return NULL;
	node* temp = head;
	while (temp->next != NULL) temp = temp->next;
	return temp;
}

node* enqueue(node* queue, node* toAdd){
	node* tail = find_last(queue);
	tail->next = toAdd;
	return toAdd;
}

/*	finds a node in the list using a compare operation on the set of data in the node
*	Arguments: _head, the head or first element in the list
*		comp, the compare operation which evaluates every element in the list
*		data, the given identifier that determines if the element has been found
*/
node* find_by_data(node* _head, int (*comp) (void*, void*), void* data){
	node* temp = _head;
	
	if (comp == NULL) return NULL;

	while(temp != NULL){
		if (comp(temp->data, data) == 0) break;
		temp = temp->next;
	}
	
	return temp;
}

node* remove_node(node** head, int (*comp) (void*, void*), void* data){
	if (head == NULL || *head == NULL || comp == NULL || data == NULL) return NULL;
	
	node* temp = *head;
	if (comp(temp->data, data) == 0){
		*head = temp->next;
		temp->next = NULL;
		return temp;
	}
	
	while(temp->next != NULL && comp((temp->next)->data, data) != 0) temp = temp->next;

	node* found = temp->next;
	if (found == NULL) return NULL;

	temp->next = found->next;
	//found->next = NULL;		//dependency keep for use of NEXTFIT
					//within main, check if removed == last add
					//if true keep dependency until next add
					//if false remove dependency and delete element
	return found;	
}

/*	frees the memory that was allocated dynamically for the node
*	memory of the data stored in the node must be released ouside the method if it was
*	allocated dynamically
*/
void delete_node(node* item, void (*delete_data) (void*)){
	if (item == NULL) return;
	if (delete_data != NULL) delete_data(item->data);
	free(item);
}

/*	frees the memory of the list created from the node structure recurrently
*/
void delete_list(node* head, void (*delete_data) (void*)){
	if (head->next != NULL) delete_list(head->next, delete_data);
	
	delete_node(head, delete_data);
}

size_t getSize(node* head){
	if (head == NULL) return 0;
	size_t sz = 0;

	node* temp = head;
	while (temp != NULL) {
		sz++;
		temp = temp->next;
	}
	return sz;
}

void get_data_list(node* head, void** pntr_list){
	if (head == NULL) return;
	if (pntr_list == NULL) return;	

	size_t sz = getSize(head);
	size_t i = 0;

	node* temp = head;
	do{
		pntr_list[i] = temp->data;
		i++;
		temp = temp->next;
	}while(i < sz);

}

#endif
