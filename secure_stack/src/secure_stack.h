#ifndef STACK_H_
#define STACK_H_




#include "secure_stack.config.h"
#include "../../logging/src/logging.h"

#include <stddef.h>
#include <stdbool.h>


#if HASH == ON
	#include <stdint.h>
#endif




typedef struct stack_t_
{
	#if CANARIES == ON
		unsigned long long left_canary;
	#endif

	#if HASH == ON
		uint64_t hash;
	#endif
	
	void *data;
	size_t element_size;
	size_t size;
	size_t capacity;
	char   name[64];

	#if CANARIES == ON
		unsigned long long right_canary;
	#endif
} stack_t;




typedef enum stack_error_t_
{
	STACK_OK         = 0,
	STACK_EMPTY      = 1,
	ALLOCATION_ERROR = 2,
	INVALID_PTR      = 3,
	INVALID_DATA_PTR = 4,
	SOME_ERROR       = 5,

} stack_error_t;




/*================= Function prototypes ==================*/


stack_t *stack_create_func_ (const char *name, size_t size_element);


stack_t stack_constructor_func_ (const char *name, size_t size_element);


stack_error_t stack_delete (stack_t *stack_ptr);


stack_error_t stack_deconstructor (stack_t *stack);


stack_error_t stack_check_func_ (stack_t *stack, _CODE_POSITION_T_);


stack_error_t stack_top (stack_t *stack, void *result);


stack_error_t stack_pop (stack_t *stack, void *result);


stack_error_t stack_push (stack_t *stack, const void *pushed_value);




/*================== Functional macros ===================*/


#define stack_constructor(NAME_, TYPE_) \
	stack_t NAME_ = stack_constructor_func_(#NAME_, sizeof(TYPE_))


#define stack_create(NAME_, TYPE_) \
	stack_t *NAME_ = stack_create_func_(#NAME_, sizeof(TYPE_))


#define stack_element_size(STACK_) (STACK_)->element_size


#define stack_element_size_err(STACK_, ERROR_) (ERROR_ = stack_check(STACK_),\
		                                stack_element_size(STACK_))


#define stack_size(STACK_) (STACK_)->size


#define stack_size_err(STACK_, ERROR_) (ERROR_ = stack_check(STACK_),\
		                        stack_size(STACK_))


#define stack_check(STACK_) stack_check_func_(STACK_, _CURRENT_CODE_POSITION_)


#if HASH == ON

#define stack_get_hash(STACK_) ((STACK_)->hash)

#else

#define stack_get_hash(STACK_) (void) 0

#endif


#endif
