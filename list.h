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
	if (*_head == NULL) return;

	printNode(*_head, func);
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

/*	used with queues implementations of the structure. Queues the given data at the end
*	Arguments: tail, the last element present in the list
*		data, the data that is going to be queue up
*
*	if the user does not have a pointer to the tail application can be done as follows
*		node* tail = enqueue(find_last(head), data);
*/
node* enqueue(node* tail, void* data){
	return insert_after(tail, data, NULL);
}

/*	removes the first element in the queue
*	Arguments: queue, the queue implementation of the structure
*/
node* pop(node** queue){
	node* temp = *queue;
	*queue = temp->next;
	temp->next = NULL;
	return temp;
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
void delete_node(node* item){
	if (item == NULL) return;
	free(item);
}

/*	frees the memory of the list created from the node structure recurrently
*/
void delete_list(node* head){
	if (head->next != NULL) delete_list(head->next);
	
	delete_node(head);
}

#endif
