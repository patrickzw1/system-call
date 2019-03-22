// Test application for the process_ancestors syscall.

#include <stdio.h>
#include <unistd.h>
#include <sys/syscall.h>
#include <assert.h>
#include <stdlib.h>
#include <stdint.h>
#include <errno.h>
#include "process_ancestors.h"

#define _PORCESS_ANCESTORS_ 342

void test_print_process(struct process_info* pro_info, long size){
	for (int i = 0; i < size; i++){
		printf("-------------------  process:  %d --------------\n", i);
		printf("pid          => %ld \n", pro_info[i].pid);
		printf("name         => %s  \n", pro_info[i].name);
		printf("state        => %ld \n", pro_info[i].state);
		printf("uid          => %ld \n", pro_info[i].uid);
		printf("nvcsw        => %ld \n", pro_info[i].nvcsw);
		printf("nivcsw       => %ld \n", pro_info[i].nivcsw);
		printf("num_children => %ld \n", pro_info[i].num_children);
		printf("num_siblings => %ld \n", pro_info[i].num_siblings);
		printf("-------------------- end ----------------------------\n");
	}
}

//static int current_syscall_test_num = 0;

/***********************************************************
 * Functions to actually make the sys-call and test results
 ***********************************************************/
// static int do_syscall(struct process_info *stats, long size, long *num_filled)
// {
// 	current_syscall_test_num++;
// 	printf("\nTest %d: ..Diving to kernel level\n", current_syscall_test_num);
// 	int result = syscall(_PORCESS_ANCESTORS_, stats, size, &num_filled);
// 	int my_errno = errno;
// 	printf("..Rising to user level w/ result = %d", result);
// 	if (result < 0) {
// 		printf(", errno = %d", my_errno);
// 	} else {
// 		my_errno = 0;
// 	}
// 	printf("\n");
// 	return my_errno;

// }

int main (int argc, char* argv[]){
	long size = 10;
	struct process_info* pro_info = malloc(sizeof(struct process_info)*size);
	long num_filled;

	int result = syscall(_PORCESS_ANCESTORS_, pro_info, size, &num_filled);
	//int result = do_syscall(pro_info, size, num_filled);

	test_print_process(pro_info, num_filled);

	assert(result == 0);
	printf("num_filled -> %ld \n", num_filled);
	free(pro_info);
	return 0;
}

