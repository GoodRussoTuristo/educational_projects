/*!
 *  \file
 *  This file contains realization of function used to sort text.
 */

#include "TextSort.h"

static const unsigned ATTEMPTS_TO_READ = 3;

bool get_existing_file_path ( char *file_path, const char *message)
{
	assert ( file_path );
	assert ( message );

	unsigned attempts_left = ATTEMPTS_TO_READ;

	puts(message);

	gets_s(file_path, BUFSIZ);
	
	while (! can_read(file_path) )
	{
		attempts_left--;

		puts("You wrote incorrect file path.\n"
		     "Please, try again: ");

		gets_s(file_path, BUFSIZ);

		if (! attempts_left )
		{
			puts("You haven't any attempts.\n");
			return false;
		}
	}
	
	return true;
}


string_array initialize_string_array (void)
{
	return (string_array) calloc(sizeof (string_array), 1);
}


bool read_file (string_array strings, const char *file_path)
{
	assert ( strings );
	assert ( file_path );
	
	int file_descriptor = open(file_path, O_RDONLY);
	
	if ( file_descriptor == -1)
		return false;
	
	strings->n_mbchars = file_size(file_descriptor);

	char *mbstrings = (char *) calloc(sizeof *mbstrings,
			 strings->n_mbchars + 1);
	
	if (! mbstrings )
		return false;

	if ( read(file_descriptor, mbstrings, strings->n_mbchars) == -1 )
		return false;

	strings->head = (wchar_t *) calloc(sizeof *strings->head,
					   strings->n_mbchars);
	if (! strings->head )
		return false;

	strings->n_chars = mbstowcs(strings->head, mbstrings, strings->n_mbchars);

	replacewc(strings->head, '\0', '\n');
	strings->head[strings->n_chars] = EOF;
	
	close(file_descriptor);
	free(mbstrings);
	
	strings->count = count_strings(strings);
	
	strings->head = (wchar_t *) realloc(strings->head,
					    (strings->n_chars + 1) * sizeof *strings->head);

	strings->element = (string *) calloc(sizeof *strings->element,
					     strings->count);
	
	if ( strings->element && strings->head )
	{
		restore_order(strings);
		return true;
	}
	else
	{
		return false;
	}
}


void sort_string_array (string_array strings)
{
	assert ( strings );

	sort(strings->element,
	     strings->count, sizeof *strings->element, straight_strcmp);
}


void reverse_sort_string_array (string_array strings)
{
	assert ( strings );

	sort(strings->element,
	     strings->count, sizeof *strings->element, reverse_strcmp);
}


bool write_to_file (const string_array strings, FILE *output)
{
	assert ( strings );
	assert ( strings->head );
	assert ( strings->element );
	assert ( output );

	char *mbstring = (char *) calloc(sizeof *mbstring,
					MB_CUR_MAX * strings->max_string_length + 1);
	if (! mbstring )
		return false;

	fput_char_line(output);

	for (int index = 0; index < strings->count; index++)
	{
		if ( *strings->element[index].begin )
		{
			wcstombs(mbstring,
				 strings->element[index].begin,
			 	 MB_CUR_MAX * strings->element[index].length);

			fputs(mbstring, output);
		}
		fputc('\n', output);
	}
	free(mbstring);
}


void clear_string_array_fields (string_array strings)
{
	assert ( strings );
	
	if ( strings->element )
	{
		free(strings->element);
		strings->element = NULL;
	}
	if ( strings->head )
	{
		free(strings->head);
		strings->head = NULL;
	}
}


void clear_string_array (string_array *strings_ptr)
{
	assert ( strings_ptr );

	if (! *strings_ptr )
		return;

	clear_string_array_fields(*strings_ptr);
	free(*strings_ptr);
	*strings_ptr = NULL;
}


bool can_read (const char *file_path)
{
	assert ( file_path );

	return access(file_path, R_OK) + 1;
}


void fput_char_line (FILE *output)
{
	assert ( output );

	fputs("\n--------------------------------------------------\n\n", output);
}


size_t count_strings (const string_array strings)
{
	assert ( strings );
	assert ( strings->head );

	wchar_t *cur_position = strings->head;
	size_t n_strings = 0;

	while ( *cur_position != EOF )
	{
		cur_position = wcschr(cur_position, '\0');
		n_strings++;
		cur_position++;
	}
	return n_strings;
}


void restore_order (string_array strings)
{
	assert ( strings );
	assert ( strings->head );
	assert ( strings->element );
	
	wchar_t *start = strings->head, *end = NULL;

	for (int index = 0; index < strings->count; index++)
	{
		end = wcschr(start, '\0');

		strings->element[index].begin = start;
		strings->element[index].length = end - start;

		if (strings->max_string_length < strings->element[index].length)
		{
			strings->max_string_length = strings->element[index].length;
		}

		start = end + 1;
	}
}


off_t file_size (const int file_descriptor)
{
	struct stat stat_buf = {};

	if ( fstat(file_descriptor, &stat_buf) )
	{
		return -1;
	}
	else
	{
		return stat_buf.st_size;
	}
}


char *gets_s (char *str, const size_t n)
{
	assert ( str );

	if (fgets(str, n, stdin))
	{
		str[strlen(str) - 1] = '\0';
		return str;
	}
	else
	{
		return NULL;
	}
}


void replacewc (wchar_t *str, const wchar_t to, const wchar_t from)
{
	assert ( str );

	while (str = wcschr(str, from))
	{
		*str = to;
		str++;
	}
}


void sort (void *start, const size_t n, const size_t size,
	   int (*comparator) (const void *, const void *))
{
	assert( start );
	assert( n >= 0 );
	assert( size > 0 );
	assert( comparator );

	void *end = start + (n - 1) * size;

	for (void *first = start; first <= end; first += size)
		for (void *second = first + size; second <= end; second += size)
			if ( comparator(first, second) > 0 )
				mem_swap(first, second, size);
}


int straight_strcmp(const void *str1_ptr, const void *str2_ptr)
{
	assert ( str1_ptr );
	assert ( str2_ptr );

	string str1 = *( (string *) str1_ptr);
	string str2 = *( (string *) str2_ptr);
	
	wchar_t *wc_ptr1 = str1.begin;
	wchar_t *wc_ptr2 = str2.begin;

	while ( *wc_ptr1 && *wc_ptr2 )
	{
		if (! iswalpha(*wc_ptr1) )
		{
			wc_ptr1++;
		}
		else if (! iswalpha(*wc_ptr2) )
		{
			wc_ptr2++;
		}
		else if ( *wc_ptr1 != *wc_ptr2 )
		{
			return *wc_ptr1 - *wc_ptr2;
		}
		else
		{
			wc_ptr1++;
			wc_ptr2++;
		}
	}
	return *wc_ptr1 - *wc_ptr2;
}

int reverse_strcmp (const void *str1_ptr, const void *str2_ptr)
{
	assert ( str1_ptr );
	assert ( str2_ptr );
	
	string str1 = *( (string *) str1_ptr);
	string str2 = *( (string *) str2_ptr);

	wchar_t *wc_ptr1 = str1.begin + str1.length - 1;
	wchar_t *wc_ptr2 = str2.begin + str2.length - 1;

	while ( wc_ptr1 >= str1.begin && wc_ptr2 >= str2.begin )
	{
		if (! iswalpha(*wc_ptr1) )
		{
			wc_ptr1--;
		}
		else if (! iswalpha(*wc_ptr2) )
		{
			wc_ptr2--;
		}
		else if ( *wc_ptr1 != *wc_ptr2 )
		{
			return *wc_ptr1 - *wc_ptr2;
		}
		else
		{
			wc_ptr1--;
			wc_ptr2--;
		}
	}
	if ( wc_ptr1 >= str1.begin )
	{
		return 1;
	}
	else if ( wc_ptr2 >= str2.begin )
	{
		return -1;
	}
	else
	{
		return 0;
	}
}

#define MEM_SWAP_N( A_PTR, B_PTR, SIZE_LEFT, TYPE ) \
{\
	while ( SIZE_LEFT >= sizeof (TYPE) )\
	{\
		TYPE *A_ = (TYPE *) A_PTR, *B_ = (TYPE *) B_PTR, TMP_ = *A_;\
		*A_ = *B_;\
		*B_ = TMP_;\
\
		A_PTR += sizeof (TYPE);\
		B_PTR += sizeof (TYPE);\
		SIZE_LEFT -= sizeof (TYPE);\
	}\
}

void mem_swap (void *a_ptr, void *b_ptr, size_t size)
{
	assert ( a_ptr );
	assert ( b_ptr );

	MEM_SWAP_N(a_ptr, b_ptr, size, __int128_t);
	MEM_SWAP_N(a_ptr, b_ptr, size, __int64_t );
	MEM_SWAP_N(a_ptr, b_ptr, size, __int32_t );
	MEM_SWAP_N(a_ptr, b_ptr, size, __int16_t );
	MEM_SWAP_N(a_ptr, b_ptr, size, __int8_t  );

	assert (! size );
}
