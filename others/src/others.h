/*! \file
 *  This file includes definition of some different functions.
 */




#ifndef OTHERS_H_

#define OTHERS_H_




/*================= Connectiong headers ==================*/


#include <stddef.h>
#include <stdbool.h>




/*================== Function prototypes =================*/


/*! This function checks if a byte is readable.
 *
 *  \param[in] ptr - pointer to checking byte.
 *
 *  \return True if byte is not readable else false.
 *
 *  \note This function uses linux syscalls.
 */
bool is_bad_byte_ptr (const void *ptr);


/*! This function checks if memory is readable.
 *
 *  \param[in] ptr  - pointer to the begining of the memory being checked.
 *  \param[in] size - size of checking memory.
 *
 *  \return True if memory is not readable else false.
 *
 *  \note This function uses Linux system calls.
 */
bool is_bad_mem (const void *ptr, size_t size);


/*! This macro checks if the value pointed to by the pointer can be read.
 *
 *  \param PTR_ - pointer that will be checked.
 *
 *  \return true if value can't be read else false.
 *
 *  \note This macro uses Linox system call.
 */
#define is_bad_ptr(PTR_) is_bad_mem(PTR_, sizeof (PTR_))


#endif
