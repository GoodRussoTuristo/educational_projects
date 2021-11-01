
#include "string_.h"


int puts_(const char *str)
{
	assert(str);

	while (*str)
	{
		putchar(*str);
		str++;
	}
	return putchar('\n');
}

char *strchr_(const char *str, const char ch)
{
	assert(str);

	while (*str != ch || *str)
	{
		str++;
	}
	return (*str == ch) ? (char *)str : NULL;
}

size_t strlen_(const char *str)
{
	assert(str);

	size_t n_chars = 0;

	while (*str)
	{
		n_chars++;
		str++;
	}
	return n_chars;
}

char *strcpy_(char *dest, const char *source)
{
	assert(dest);
	assert(source);
	assert(dest != source);
	
	size_t index = 0;

	while (source[index])
	{
		dest[index] = source[index];
		index++;
	}
	dest[index] = '\0';
	
	return dest;
}


char *strncpy_(char *dest, const char *source, const size_t number)
{
	assert(dest);
	assert(source);
	assert(dest != source);

	size_t index = 0, last_index_of_dest = number - 1;

	while (source[index] && index < last_index_of_dest)
	{
		dest[index] = source[index];
		index++;
	}
	dest[index] = '\0';

	return dest;
}

char *strcat_(char *str1, const char *str2)
{
	assert(str1);
	assert(str2);
	assert(str1 != str2);

	size_t index_str1 = strlen_(str1), index_str2 = 0;

	while (str2[index_str2])
	{
		str1[index_str1] = str2[index_str2];
		index_str1++;
		index_str2++;
	}
	str1[index_str1] = '\0';

	return str1;
}

char *strncat_(char *str1, const char *str2, const size_t number)
{
	assert(str1);
	assert(str2);
	assert(str1 != str2);
	
	size_t index_str1 = 0, index_str2 = 0,
	       last_index_of_str1 = strlen_(str1) + number;

	while (str2[index_str2] &&
		    index_str1 < last_index_of_str1)
	{
		str1[index_str1] = str2[index_str2];
		index_str1++;
		index_str2++;
	}
	str1[index_str1] = '\0';

	return str1;
}

char *fgets_(char *str, const size_t number, FILE *stream)
{
	assert(str);
	assert(stream);

	size_t index = 0, last_index_of_str = number - 1;

	while (index < last_index_of_str &&
	       (str[index] = fgetc(stream)) != '\n' &&
	       str[index] != EOF)
	{
		index++;
	}
	return (str[index] != EOF) ? str : NULL;
}

char *strdup_(const char *source)
{
	assert(source);

	char *duplicate = (char *)calloc( strlen_(source) + 1, sizeof *duplicate);

	if (duplicate)
		strcpy_(duplicate, source);

	return duplicate;
}

size_t getline_(char **lineptr, size_t *number, FILE *stream)
{
	assert(lineptr);
	assert(number);

	const size_t SIZE_OF_ALLOC_BLOCK = 8;
	char temp_char = EOF;
	
	*number = 0;
	
	if ( (temp_char = getc(stream)) == EOF)
		return *number;
	
	*lineptr = (char *) calloc(SIZE_OF_ALLOC_BLOCK, sizeof **lineptr);
	
	if (*lineptr) 
	do
	{
		(*lineptr)[*number] = temp_char;
		
		(*number)++;
		
		if (*number % SIZE_OF_ALLOC_BLOCK == 0)
		{
			*lineptr = (char *) realloc(*lineptr,
					*number + SIZE_OF_ALLOC_BLOCK);
			
			if (!*lineptr)
			{
				*number = 0;
				break;
			}	}
	
	} while ( (temp_char = getc(stream)) != '\n');
	(*number)++;

	*lineptr = (char *) realloc(*lineptr, *number);

	if (*lineptr)
	{
		(*lineptr)[*number - 1] = '\0';
	}
	else
		*number = 0;

	return *number;
}

int strcmp_(const char *str1, const char *str2)
{
	assert(str1);
	assert(str2);

	while (*str1 == *str2 && str1)
	{
		str1++;
		str2++;
	}
	return *str1 - *str2;
}

int atoi_(const char *str)
{
	assert(str);

	int integer = 0, sign = 1;

	if (*str == '-')
	{
		sign = -1;
		str++;
	}
	
	while (isdigit_(*str))
	{
		integer = 10 * integer + todigit_(*str);
		str++;
	}
	return sign * integer;
}

bool isdigit_(const char ch)
{
	return '0' <= ch && ch <= '9';
}

int todigit_(const char ch)
{
	return ch - '0';
}
