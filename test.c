#include <stdio.h>
#include "process.h"
#include "list.h"

int main(){
	printf("%ld\n", MEMORY);
	prcss _data1 = {25, 0, "OS"};
	print_prcss(&_data1);
	
	prcss _data2 = {50, 0, "P1"};
	print_prcss(&_data2);

	int result = SET_MAX_MEMORY(75);
	printf("%ld, %d\n", MEMORY, result);

	result = SET_MAX_MEMORY(75);
	printf("%ld, %d\n", MEMORY, result);

	node* _head = create_node(&(_data1));
	node* _lastAdd = insert_after(_head, &_data2, prcss_set_base);	
	
	printList(&_head, print_prcss);
	printNode(find_by_data(_head, comp_prcss, &_data2), print_prcss);
	printNode(find_by_data(_head, comp_prcssID, "P1"), print_prcss);
	
	prcss _data3 = {3236, 0, "P2"};
	_lastAdd = insert_after(_lastAdd, &_data3, prcss_set_base);
	printList(&_head, print_prcss);

	node* _removed = remove_node(&_head, comp_prcssID, "P2");
	printList(&_head, print_prcss);
	printNode(_removed, print_prcss);

	delete_list(_head);
}
