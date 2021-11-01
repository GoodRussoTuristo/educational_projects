#include "secure_stack.h"
#include <stdio.h>
#define SZ 10

int main (void)
{
	set_stdout_logging(true);
	start_logging();
	
	int res;

	stack_create(my_stack, int);
	
	for (int i = 1; i <= SZ; i++)
	{
		stack_push(my_stack, &i);
	}
	
	printf("Stack size = %zd\n", stack_size(my_stack));

	/*
	NEVER DO THIS!
	I messed up the stack on purpose to show how the logging works.
	*/
	my_stack->hash = 0;

	for (int i = 0; i <= SZ; i++)
	{
		stack_pop(my_stack, &res);
		printf("%d\n", res);
	}

	stack_delete(my_stack);
	
	stop_logging();

	return 0;
}
