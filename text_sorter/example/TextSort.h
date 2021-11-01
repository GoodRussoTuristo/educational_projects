/*! \file
 *  This file contains definition of structures and functions
 *  used to sort text.
 */

#ifndef TEXT_SORT_H_
#define TEXT_SORT_H_


#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>
#include <wctype.h>
#include <locale.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include <assert.h>


/*!
 *  This structure contains pointer to first char in string
 *  and length of this string.
 */
typedef struct
{
	wchar_t *begin;
	size_t   length;
} string;

/*!
 *  This structure contains pointer to start string array
 *  and pointers to all strings
 *  and number of these strings.
 */
typedef struct
{
	string  *element;
	wchar_t *head;
	size_t   max_string_length;
	size_t   count;
	size_t   n_chars;
	size_t   n_mbchars;
} *string_array;


/*!
 * This function gets from stdin path of existing file.
 * It gives ATTEMPTS_TO_READ attempts to enter correct file path.
 * 
 * \param[out] file_path - variable where the file path will be written.
 * \param[in]  message   - prompt text.
 * 
 * \return If reading success it returns true else it returns false.
 */
bool get_existing_file_path (char *file_path, const char *message);


/*!
 *  This function returns initialized string_array variable.
 *  
 *  \note All variables that have string_array must be initialized by this function.
 *  
 *  \return Initialized variable of string_array type.
 */
string_array initialize_string_array (void);


/*!
 *  This function reads text from file to variable that has string_array type
 *  on correct way.
 *  
 *  \param[out] strings   - variable to which the reading text will be written.
 *  \param[in]  file_path - file path of file that can be readen.
 *  
 *  \note This function uses Unix syscalls.
 *  
 *  \return If reading success this function returns true else it returns false.
 */
bool read_file (string_array strings, const char *file_path);


/*!
 *  This sorts string_array by begining of these strings.
 *  
 *  \param[in,out] strings - string array that will be sorted.
 */
void sort_string_array (string_array strings);


/*!
 *  This function sorts string array by end of these strings.
 *  
 *  \param[in,out] strings - string array that will be sorted.
 */
void reverse_sort_string_array (string_array strings);


/*!
 *  This function writes string array into file.
 *  
 *  \param[in] strings - string array that will be written.
 *  \param[in] output  - the file to which the string array will be written.
 *  
 *  \return True if writing success else false.
 */
bool write_to_file (string_array strings, FILE *output);


/*!
 *  This function sets default field values and frees the heap they are using.
 *  
 *  \param[in,out] strings - string array that will be seted to default.
 */
void clear_string_array_fields (string_array strings);


/*!
 *  This function frees the heap that array string using.
 *  
 *  \param[in,out] strings - string array that needs to delete.
 */
void clear_string_array (string_array *strings_ptr);


/*!
 *  This function checks access to read file.
 *  
 *  \param[in] file_path - path to the file that needs to check.
 *  
 *  \note This function uses Unix syscall.
 *  
 *  \return Can program read this file.
 */
bool can_read (const char *file_path);


/*!
 *  This function print line of '_' into the file
 *  
 *  \param[in] Path to the file to which the line of '_' will be written.
 */
void fput_char_line (FILE *output);


/*!
 *  This function counts strings in string array.
 *  
 *  \param[in] strings - string array in which the strings will be counted.
 *  
 *  \return number of strings.
 */
size_t count_strings (const string_array strings);


/*!
 *  This function restores default order of strings in string array.
 *  
 *  \param[in,out] strings - string array in which this function restore order of strings.
 */
void restore_order (string_array strings);


/*!
 *  This function returns size of file in bytes.
 *  
 *  \param[in] descriptor - file descriptor that size will be returned.
 *  
 *  \note This function uses stat structure.
 *  
 *  \return Length of file in bytes. If the file doesn't exist function returns -1.
 */
off_t file_size (const int file_descriptor);


/*!
 *  Memory safety gets analog.
 *  
 *  \param[out] str - string to which the read string will be written.
 *  \param[in]  n   - max length of read string.
 *  
 *  \return pointer to start of read string.
 */
char *gets_s (char *str, const size_t n);


/*!
 *  This function replaces all first characters to other characters in string of wchar_t.
 *  
 *  \param[in,out] str  - wchar_t string in which this function will replaces characters.
 *  \param[in]     to   - character for substitution.
 *  \param[in]     from - character that will be replaced.
 */
void replacewc (wchar_t *str, const wchar_t to, const wchar_t from);


/*!
 *  This function is qsort() analog.
 * 
 *  \param[in,out] start      - array that will be sorted.
 *  \param[in]     n          - number of elements in array.
 *  \param[in]     size       - size of element in this array.
 *  \param[in]     comparator - function which can compare array elements.
 */
void sort (void *start, const size_t n, const size_t size,
	   int (*comparator) (const void *, const void *));


/*!
 *  This function applies lexicorphyse comparison to two wchar_t strings.
 *  
 *  \param[in] str1 - first string to comparison.
 *  \param[in] str2 - second string to comparison.
 *  
 *  \return negative value if str1 < str2, 0 if str1 = str2, positive value if str1 > str2.
 */
int straight_strcmp (const void *str1, const void *str2);


/*!
 *  This function applies reverse lexicorphyse comparison to two wchar_t strings.
 *  
 *  \param[in] str1 - first string to comparison.
 *  \param[in] str2 - second string to comparison.
 *  
 *  \return negative value if str1 < str2, 0 if str1 = str2, positive value if str1 > str2.
 */
int reverse_strcmp (const void *str1, const void *str2);


/*!
 *  This function swaps two values.
 *  
 *  \param[in,out] a_ptr - pointer to first element that will be swapped.
 *  \param[in,out] a_ptr - pointer to second element that will be swapped.
 *  \param[in]     size  - size of swapped elements.
 */
void mem_swap (void *a_ptr, void *b_ptr, size_t size);


#endif
