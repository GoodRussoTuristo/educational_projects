

/*================= Connectiong headers ==================*/


#include "secure_stack.h"
#include "../../others/src/others.h"

#if HASH == ON
	#include "../../hash/src/hash.h"
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>




/*================== Local functions =====================*/


static size_t reduce_capacity (size_t capacity, size_t new_size)
{
	size_t new_capacity = capacity;
	if ( new_capacity > 256 )
	{
		new_capacity -= 256;
	}
	else
	{
		new_capacity &= (new_capacity - 1);
	}

	if ( new_capacity >= new_size )
	{
		if ( new_capacity == 0 )
			return 1;
		else
			return new_capacity;
	}
	else
		return capacity;
}

static size_t increase_capacity (size_t capacity, size_t new_size)
{
	if ( new_size == capacity )
	{
		if ( capacity < 256 )
		{
			if ( (capacity & 1) == 1 )
				capacity <<= 1;
			capacity |= capacity - 1;
		}
		else
		{
			capacity += 256;
		}
	}
	return capacity;
}


static void insert_canary (void *data)
{
	*(unsigned long long *) data = CANARY;
}


static void *stack_last_element_ptr(stack_t *stack)
{
	void *result = stack->data +
			(stack->size - 1) * stack->element_size;

	#if CANARIES == ON
		result += sizeof CANARY;
	#endif

	return result;
}


#if HASH == ON

#define stack_calculate_hash(STACK_) stack_calculate_hash_func_(STACK_)

uint64_t stack_calculate_hash_func_(stack_t *stack)
{
	stack->hash = 0;
	uint64_t hash = (stack->size) % 256;

	hash ^= pearson_hash64(stack, sizeof *stack);

	if ( stack->size != 0 )
		#if CANARIES == ON
			hash ^= pearson_hash64(stack->data,
					stack->capacity + 2 * sizeof CANARY);
		#else
			hash ^= pearson_hash64(stack->data,
					stack->capacity);
		#endif

	stack->hash = hash;

	return hash;
}

#else

#define stack_calculate_hash(STACK_)

#endif


void print_byte (char *dest, const void *byte)
{
	sprintf(dest, "%X", *(const unsigned char *) byte);
}


bool check_stack_data (stack_t *stack, char *str)
{
	bool result = true;
	size_t data_length = stack->element_size * stack->capacity;
	unsigned char *start = (unsigned char *) stack->data;

	#if CANARIES == ON
	
		unsigned long long left_canary = *(unsigned long long*)(stack->data),
			right_canary = *(unsigned long long *) (stack->data
				+ sizeof CANARY + data_length);

		sprintf(str, "Left canary = %llx. Right canary = %llx. "
				"CANARY = %llx", left_canary, right_canary, CANARY);
		if ( left_canary != CANARY || right_canary != CANARY )
		{
			add_sublog("Canaries in stack data corrupted!", str,
					WARNING, 3);
			result = false;
		}
		else
			add_sublog("Canaries in stack data are good.", str, OK, 3);

		start += sizeof CANARY;

	#endif

	bool data_good = true;

	for (unsigned char *ptr = start + stack->size * stack->element_size;
			ptr < start + data_length && data_good;
			ptr += stack->element_size)
	{
		if ( *ptr != POISON )
			data_good = false;
	}

	if (! data_good )
	{
		add_table_log("Data is corrupted!", start, data_length, 1,
				print_byte, WARNING, 3);
		return false;
	}
	add_table_log("Data isn't corrupted.", start, data_length, 1,
			print_byte, OK, 3);

	return result;
}


bool check_hash (stack_t *stack, char *str)
{
	#if HASH == ON

		uint64_t old_hash = stack_get_hash(stack);
		stack_calculate_hash(stack);

		sprintf(str, "%s->hash = %lu. Must be %lu", stack->name,
				old_hash, stack->hash);
		if ( stack_get_hash(stack) != old_hash )
		{
			add_sublog("Hash incorrect!", str, WARNING, 2);
			return false;
		}
		add_sublog("Hash correct.", str, OK, 2);

	#endif

	return true;
}




/*=================== Global functions ===================*/


stack_t *stack_create_func_ (const char *name, size_t element_size)
{
	#if VALIDATION == ON

	if_log ( element_size <= 0, ERROR )
		return NULL;
	
	#endif

	stack_t *stack_ptr = (stack_t *) calloc(sizeof *stack_ptr, 1);
	
	if ( stack_ptr )
		*stack_ptr = stack_constructor_func_(name, element_size);
	
	return stack_ptr;
}


stack_t stack_constructor_func_ (const char *name, size_t element_size)
{
	stack_t stack;

	#if VALIDATION == OK

	if_log ( element_size <= 0, ERROR )
		element_size = 1;

	if_log ( is_bad_ptr(name), ERROR )
		stack.name = "UNKNOWN";

	#endif	

	strcpy(stack.name, name);
	stack.data         = POISON_PTR;
	stack.element_size = element_size;
	stack.size         = 0;
	stack.capacity     = 1;

	#if CANARIES == ON
		stack.left_canary = stack.right_canary = CANARY;
	#endif

	stack_calculate_hash(&stack);

	return stack;
}


stack_error_t stack_delete (stack_t *stack_ptr)
{
	stack_error_t error = STACK_OK;
	if ( stack_ptr )
	{
		error = stack_deconstructor(stack_ptr);
		free(stack_ptr);
	}
	return error;
}


stack_error_t stack_deconstructor (stack_t *stack)
{
	#if VALIDATION == ON

	stack_error_t error = stack_check(stack);
	
	if ( error == STACK_OK )
	{

	#endif

		stack->size     = 0;
		stack->capacity = 1;
		
		if ( stack->data != POISON_PTR )
		{
			free(stack->data);
			stack->data = POISON_PTR;
		}

		stack_calculate_hash(stack);

		return STACK_OK;

	#if VALIDATION == ON

	}
	return error;

	#endif
}


stack_error_t stack_check_func_ (stack_t *stack, _CODE_POSITION_T_)
{
	bool error = false;
	char str[200];

	if ( is_bad_ptr(stack) )
	{
		sprintf(str, "stack_t *unknown = %p", stack);
		write_log("Pointer to stack is bad!", str, ERROR, 0);
		return INVALID_PTR;
	}

	if ( is_bad_ptr(stack->name) )
	{
		sprintf(str, "stack_t *unknown; unknown->name = %p", stack->name);
		write_log("Pointer of ame of stack is bad!", str, ERROR, 0);
		return INVALID_PTR;
	}

	sprintf(str, "stack_t %s", stack->name);
	multilog_begin_at("Stack checking...", str, _CODE_POSITION_);

	sprintf(str, "%s = %p", stack->name, stack);
	add_sublog("Pointer to stack is good.", str, OK, 1);

	sprintf(str, "%s->name = %p", stack->name, stack->name);
	add_sublog("Pointer to name of stack is good.", str, OK, 2);

	sprintf(str, "%s->element_size = %zd", stack->name, stack->element_size);
	if ( stack->element_size == 0 )
	{
		add_sublog("Element size incorrect!", str, ERROR, 2);
		error = true;
	}
	add_sublog("Element size is good.", str, OK, 2);

	sprintf(str, "%s->size = %zd, %s->capacity = %zd",
			stack->name, stack->size, stack->name, stack->capacity);
	if ( (stack->size == 0 && stack->capacity != 1) || stack->capacity == 0 )
	{
		add_sublog("Size or capacity incorrect!", str, ERROR, 2);
		error = true;
	}
	add_sublog("Size and capacity values are good.", str, OK, 2);

	size_t stack_length = stack->capacity;

	#if CANARIES == ON
		
		stack_length += 2 * sizeof CANARY;

		sprintf(str, "%s->left_canary = %llx, %s->right_canary = %llx, "
				"CANARY = %llx", stack->name, stack->left_canary,
				stack->name, stack->right_canary, CANARY);
		if ( stack->left_canary != CANARY || stack->right_canary != CANARY )
		{
			add_sublog("Canaries incorrect!", str, WARNING, 2);
			error = true;
		}
		add_sublog("Canaries are good.", str, OK, 2);
	
	#endif

	sprintf(str, "%s->data = %p", stack->name, stack->data);
	if ( (stack->size == 0 && stack->data != POISON_PTR) ||
	     (stack->size > 0 && is_bad_mem(stack->data, stack_length)) )
	{
		add_sublog("Pointer to stack data is bad!", str, ERROR, 2);
		multilog_end(WARNING);
		return INVALID_DATA_PTR;
	}
	add_sublog("Pointer to stack data is good.", str, OK, 2);

	if ( stack->size > 0 )
		error = ! check_stack_data(stack, str);
	
	error = ! check_hash(stack, str);

	multilog_end(WARNING);

	if ( error )
		return SOME_ERROR;
	else
		return STACK_OK;
}


stack_error_t stack_top (stack_t *stack, void *result)
{
	#if VALIDATION == ON

		if_log ( is_bad_ptr(stack), ERROR )
			return INVALID_PTR;
		if_log ( is_bad_ptr(result), ERROR )
			return INVALID_PTR;

		stack_error_t error = stack_check(stack);
		if ( error != STACK_OK )
			return error;

	#endif

	if (! stack_size(stack) )
		return STACK_EMPTY;

	void *last_element = stack_last_element_ptr(stack);
	memcpy(result, last_element, stack->element_size);

	return STACK_OK;
}


stack_error_t stack_pop (stack_t *stack, void *result)
{
	#if VALIDATION == ON

		if_log ( is_bad_ptr(stack), ERROR )
			return INVALID_PTR;
		if_log ( is_bad_ptr(result), ERROR )
			return INVALID_PTR;

	#endif

	stack_error_t error = stack_top(stack, result);

	if ( error != STACK_OK )
		return error;
	
	void *last_element = stack_last_element_ptr(stack);
	memset(last_element, POISON, stack->element_size);

	stack->size--;
	
	size_t new_capacity =
		reduce_capacity(stack->capacity, stack->size);
	
	if ( stack->size == 0 )
	{
		free(stack->data);
		stack->data = POISON_PTR;
		stack->capacity = new_capacity;
	}
	else if ( stack->capacity != new_capacity )
	{
		size_t need_memory = new_capacity * stack->element_size;
		#if CANARIES == ON
			need_memory += 2 * sizeof CANARY;
			insert_canary(stack->data + sizeof CANARY +
				      stack->element_size * new_capacity);
		#endif

		void *realloc_check = realloc(stack->data, need_memory);
		if (! realloc_check )
			return ALLOCATION_ERROR;
		realloc_check = stack->data;

		stack->capacity = new_capacity;
	}
	stack_calculate_hash(stack);

	return STACK_OK;
}


stack_error_t stack_push (stack_t *stack, const void *pushed_value)
{
	#if VALIDATION == ON
	
		if_log ( is_bad_ptr(stack), ERROR )
			return INVALID_PTR;
		if_log ( is_bad_ptr(pushed_value), WARNING )
			return INVALID_PTR;

		stack_error_t error = stack_check(stack);

		if ( error != STACK_OK )
			return error;

	#endif

	stack->size++;

	void *last_element_ptr;

	size_t new_capacity =
		increase_capacity(stack->capacity, stack->size);

	if ( new_capacity != stack->capacity )
	{
		size_t need_memory = new_capacity * stack->element_size;

		#if CANARIES == ON
			need_memory += 2 * sizeof CANARY;
		#endif
		
		if ( stack->data == POISON_PTR )
			stack->data = NULL;

		void *realloc_check = realloc(stack->data, need_memory);
		if (! realloc_check )
			return ALLOCATION_ERROR;
		stack->data = realloc_check;

		#if CANARIES == ON
			if ( stack->size == 1 )
				insert_canary(stack->data);
			insert_canary(stack->data + sizeof CANARY +
				      new_capacity * stack->element_size);
		#endif
		
		last_element_ptr = stack_last_element_ptr(stack);

		memset(last_element_ptr + stack->element_size, POISON,
		       (new_capacity - stack->size) * stack->element_size);

		stack->capacity = new_capacity;
	}
	else
	{
		last_element_ptr = stack_last_element_ptr(stack);
	}
	memcpy(last_element_ptr, pushed_value, stack->element_size);

	stack_calculate_hash(stack);

	return STACK_OK;
}
