#include <stdio.h>
#include "process.h"
#include "list.h"
#include "script.h"

static char*  SCRIPTS[] = {"REQUEST", "RELEASE", "LIST AVAILABLE", "LIST ASSIGNED", "FIND"};

#define TOTAL_COMMANDS (5)

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

	delete_node(_removed, NULL);
	delete_list(_head, NULL);
	// testing script
	
	ASSIGN_COMMANDS(SCRIPTS, TOTAL_COMMANDS);

	node* command_queue = get_command_queue("tests/input.txt", getPrcss);
	printList(&command_queue, print_command_prcss);
	
	size_t sz = getSize(command_queue);

	printf("%lu\n", sz);
	script* commands[sz];
	get_data_list(command_queue, (void**) commands);

	delete_list(command_queue, NULL);
	for (int i = 0; i < sz; i++) delete_scrpt_object(commands[i], delete_prcss);
	return EXIT_SUCCESS;
}
