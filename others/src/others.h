#ifndef OTHERS_H_

#define OTHERS_H_


#include <stddef.h>
#include <stdbool.h>




bool is_bad_byte_ptr (const void *ptr);


bool is_bad_mem (const void *ptr, size_t size);


#define is_bad_ptr(PTR_) is_bad_mem(PTR_, sizeof (PTR_))


#endif
