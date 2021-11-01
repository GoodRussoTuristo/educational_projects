/*! \file
 *  This header file contains a description of the stack
 *  aand functions for working with it.
 */




#ifndef STACK_H_
#define STACK_H_




/*================= Connectiong headers ==================*/


#include "secure_stack.config.h"
#include "../../logging/src/logging.h"

#include <stddef.h>
#include <stdbool.h>


#if HASH == ON
	#include <stdint.h>
#endif




/*========================= Types ========================*/


/*! It is stack type.
 *
 */
typedef struct stack_t_
{
	#if CANARIES == ON
		unsigned long long left_canary; /*!< left_canary - left protective variable. */
	#endif

	#if HASH == ON
		uint64_t hash; /*!< hash - hash value */
	#endif
	
	void *data;          /*!< data         - pointer to stack data.                   */
	size_t element_size; /*!< element_size - size of one element in stack.            */
	size_t size;         /*!< size         - number of stack elements.                */
	size_t capacity;     /*!< capacity     - size of allocated memory for stack data. */
	char   name[64];     /*!< name         - name of stack_t variable.                */

	#if CANARIES == ON
		unsigned long long right_canary; /*!< right_canary - right protective variable. */
	#endif
} stack_t;




/*! This enum describes all the errors
 *  that can occur when working with the stack.
 */
typedef enum stack_error_t_
{
	STACK_OK         = 0, /*!< STACK_OK         - stack is good                                */
	STACK_EMPTY      = 1, /*!< STACK_EMPTY      - stack is empty.                              */
	ALLOCATION_ERROR = 2, /*!< ALLOCATION_ERROR - error resulting from allocating stack memory */
	INVALID_PTR      = 3, /*!< INVALID_PTR      - pointer to stack is bad.                     */
	INVALID_DATA_PTR = 4, /*!< INVALID_DATA_PTR - ponter to stack data is bad.                 */
	SOME_ERROR       = 5, /*!< SOME_ERROR       - some fields of the stack are corrupted.      */

} stack_error_t;




/*================= Function prototypes ==================*/


/*! This function creates stack on heap.
 *
 * \param[in] name         - name of stack variable.
 * \param[in] size_element - size of one element in stack.
 *
 * \return pointer to initialized stack_t value.
 *
 * \note Don't forget to free heap memory using stack_delete().
 *
 * \note Use stack_create() macro instead of this function.
 */
stack_t *stack_create_func_ (const char *name, size_t size_element);


/*! This function initializes stack struct in correct way.
 *
 * \param[in] name         - name of the stack variable.
 * \param[in] size_element - size of one element in stack.
 *
 * \return initialized stack.
 *
 * \note Don't forget to free heap memory using stack_deconstructor()
 *
 * \note Use stack_constructor() macro instead of this function.
 */
stack_t stack_constructor_func_ (const char *name, size_t size_element);


/*! This function frees heap memory that stack_t* value used.
 *
 *  \param[in,out] stack - pointer to the stack to be freed.
 *
 *  \return stack_error.
 */
stack_error_t stack_delete (stack_t *stack);


/*! This function returns the stack to its original state.
 *
 * \param[in,out] stack - pointer to stack.
 *
 * \return stack_error
 */
stack_error_t stack_deconstructor (stack_t *stack);


/*! This function checks stack for integrity.
 *
 * \param[in] stack           - stack to be checked.
 * \param[in] _CODE_POSITION_ - position in source code.
 *
 * \return stack_error
 *
 * \note Use stack_check() macro instead of this function.
 */
stack_error_t stack_check_func_ (stack_t *stack, _CODE_POSITION_T_);


/*! This function returns the value from the top of the stack.
 *
 * \param[in] stack   - pointer to the stack.
 * \param[out] result - pointer to memory where the result will be written.
 *
 * \return stack_error
 */
stack_error_t stack_top (stack_t *stack, void *result);


/*! This function returns the value from the top of the stack and delete its.
 *
 * \param[in] stack   - pointer to the stack.
 * \param[out] result - pointer to memory where the result will be written.
 *
 * \return stack_error
 */
stack_error_t stack_pop (stack_t *stack, void *result);


/*! This function pushes a value to the stack.
 *
 *  \param[in,out] stack - pointer to the stack.
 *  \param[in] pushed_value - pointer to the value that will be pushed.
 *
 *  \return stack_error
 */
stack_error_t stack_push (stack_t *stack, const void *pushed_value);




/*================== Functional macros ===================*/

/*! This macro initializes stack struct in correct way.
 *
 */
#define stack_constructor(NAME_, TYPE_) \
	stack_t NAME_ = stack_constructor_func_(#NAME_, sizeof(TYPE_))


/*! This macro creates stack on heap.
 *
 */
#define stack_create(NAME_, TYPE_) \
	stack_t *NAME_ = stack_create_func_(#NAME_, sizeof(TYPE_))


/*! This macro returns the size of one element in the stack.
 *
 * \param[in] STACK_ - pointer to the stack.
 *
 *  \return size of one element in the stack.
 */
#define stack_element_size(STACK_) (STACK_)->element_size


/*! This macro returns the size of one element in the stack and checks
 *  the stack for some errors.
 *
 * \param[in] STACK_  - pointer to the stack.
 * \param[out] ERROR_ - variable to which the stack error will be written.
 *
 *  \return size of one element in the stack.
 */
#define stack_element_size_err(STACK_, ERROR_) (ERROR_ = stack_check(STACK_),\
		                                stack_element_size(STACK_))

/*! This macro returns the number of elements in the stack.
 *
 * \param[in] STACK_ - pointer to the stack.
 *
 *  \return number of elements in the stack.
 */
#define stack_size(STACK_) (STACK_)->size


/*! This macro returns the number of elements in the stack.
 *
 * \param[in] STACK_ - pointer to the stack.
 * \param[out] ERROR_ - variable to which the stack error will be written.
 * 
 * \return number of elements in the stack.
 */
#define stack_size_err(STACK_, ERROR_) (ERROR_ = stack_check(STACK_),\
		                        stack_size(STACK_))


/*! This macro checks stack for integrity.
 *
 * \param[in] stack - stack to be checked.
 *
 * \return stack_error
 *
 * \note Use stack_check() macro instead of this function.
 */
#define stack_check(STACK_) stack_check_func_(STACK_, _CURRENT_CODE_POSITION_)


#if HASH == ON

/*! This macro gets hash from tha stack.
 *
 *  \param[in] STACK_ - pointer to stack.
 *
 *  \return hash of the stack.
 */
#define stack_get_hash(STACK_) ((STACK_)->hash)

#else

#define stack_get_hash(STACK_) (void) 0

#endif


#endif
