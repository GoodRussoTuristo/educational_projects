#include "TextSort.h"

int main (void)
{
	setlocale(LC_ALL, "ru_RU.UTF-8");

	char file_path[BUFSIZ] = { };
	string_array strings = initialize_string_array();

	if (! get_existing_file_path(file_path, "Enter the input file path: ") )
	{
		return 0;
	}
	read_file(strings, file_path);

	puts("Enter the output file path: ");
	gets_s(file_path, BUFSIZ);
	
	FILE *output = fopen(file_path, "w");
	if (! output )
	{
		puts("File can't be written to the disk."
		     "The program was interrupted!");
		return 1;
	}

	sort_string_array(strings);
	write_to_file(strings, output);

	reverse_sort_string_array(strings);
	write_to_file(strings, output);

	restore_order(strings);
	write_to_file(strings, output);

	fclose(output);
	clear_string_array(&strings);

	return 0;
}
