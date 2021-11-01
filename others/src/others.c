#include "others.h"
#include "../../logging/src/logging.h"


#include <errno.h>
#include <unistd.h>
#include <stddef.h>




inline bool is_bad_byte_ptr (const void *ptr)
{
	if ( access((const char *) ptr, F_OK) &&
			errno == EFAULT )
		return true;
	else
		return false;
}


bool is_bad_mem (const void *ptr, size_t size)
{
	if_log ( size == 0, WARNING )
		return true;

	int result = is_bad_byte_ptr(ptr);
	if ( size == 1 || result )
		return result;
	else
		return is_bad_byte_ptr((const char *) ptr);
}
