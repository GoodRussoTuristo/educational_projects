/*! \file
 *  Logging library source code file.
 *  
 *  \note To compile it use key -rdynamic.
 */




/*================= Connectiong headers ==================*/


#include "logging.h"


#if LOGGING == ON


#include "../../others/src/others.h"

#include <execinfo.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>




/*========================= Types ========================*/


typedef struct log_t_
{
	char            msg[MAX_LOGGING_STRING_LENGTH];
	char            data[MAX_LOGGING_STRING_LENGTH];
	danger_status_t danger;
	int             deep_lvl;
} log_t;


struct _SUBLOG_T_
{
	log_t      *logs;
	size_t      count;
	const char *fname;
	const char *func;
	int         line;
};


struct _LOG_STATUS_T_
{
	FILE *file;
	bool  log_stdout;
	bool  log_stderr;
	bool  log_started;
};




/*=================== Local variables ====================*/


static struct _SUBLOG_T_ _SUBLOG_ =
{
	NULL,
	0,
	NULL,
	NULL,
	0,
};


static struct _LOG_STATUS_T_ _LOG_STATUS_ =
{
	NULL,
	false,
	false,
	false,
};




/*==================== Local functions ===================*/


static inline void fput_nchars (char ch, int n, FILE* fp)
{
	for (int i = 0; i < n; i++)
		fputc(ch, fp);
}


static int print_stack_trace (char **stack_trace, int size, FILE *fp)
{
	int printed = fputs("============> STACK TRACE <=============\n", fp);
	for (int i = 2; i < size; i++)
	{
		printed += fputs(stack_trace[i], fp);
		fputc('\n', fp);
	}
	fputc('\n', fp);
	return printed;
}


static bool fprint_log_ (FILE *fp, const char *msg, const char *data,
		const char *danger_str, int deep_lvl, _CODE_POSITION_T_,
		char **stack_trace, int stack_trace_size)
{
	int printed = -1;

	fput_nchars('\t', deep_lvl, fp);
	if ( deep_lvl == 0 )
		printed = fprintf(fp, "%s In %s: %s():%d: %s\n",
				danger_str, fname, func, line, msg);
	else
		printed = fprintf(fp, "%s %s\n", danger_str, msg);


	fput_nchars('\t', deep_lvl, fp);
	printed += fputs(data, fp);
	fputc('\n', fp);
	fputc('\n', fp);

	if ( deep_lvl == 0 )
	{
		printed += print_stack_trace(stack_trace,
				stack_trace_size, fp);
	}
	fputc('\n', fp);

	return printed > 0;
}


static inline char **get_stack_trace (int *size)
{
	const size_t BUF_SIZE = 1000;
	void *buffer[BUF_SIZE];
	
	*size = backtrace(buffer, BUF_SIZE);
	*size -= 2;
	 return backtrace_symbols(buffer, *size);
}


static void gen_danger_str (char danger_str[], danger_status_t danger,
		            bool colorful)
{
	switch ( danger )
	{
		case EMPTY:
			strcpy(danger_str, "");
			break;
		case OK:
			if ( colorful )
				strcpy(danger_str, "\033[1;32m[OK]:\033[0m");
			else
				strcpy(danger_str,           "[OK]:");
			break;

		case WARNING:
			if ( colorful )
				strcpy(danger_str, "\033[1;33m==> WARNING:\033[0m");
			else
				strcpy(danger_str,           "==> WARNING:");
			break;

		case ERROR:
			if ( colorful )
				strcpy(danger_str, "\033[1;31m!!! ERROR:\033[0m");
			else	
				strcpy(danger_str,           "!!! ERROR:");
			break;
		default:
			strcpy(danger_str, "--> UNKNOWN:");
	}
}


static void reset_sublog (void)
{
	_SUBLOG_.logs  = NULL;
	_SUBLOG_.count = 0;
	_SUBLOG_.fname = NULL;
	_SUBLOG_.func  = NULL;
	_SUBLOG_.line  = 0;
}




/*=================== Global functions ===================*/


bool set_logfile (const char *fname)
{
	if (! fname )
		return false;
	
	FILE *fp = fopen(fname, "a");
	if (! fp )
		return false;

	_LOG_STATUS_.file = fp;

	return true;
}


void remove_logfile (void)
{
	if ( _LOG_STATUS_.file )
	{
		fclose(_LOG_STATUS_.file);
		_LOG_STATUS_.file = NULL;
	}
}


void set_stdout_logging (bool val)
{
	_LOG_STATUS_.log_stdout = val;
}


void set_stderr_logging (bool val)
{
	_LOG_STATUS_.log_stderr = val;
}


void start_logging_func_ (void)
{
	_LOG_STATUS_.log_started = true;
}


void stop_logging_func_ (void)
{
	_LOG_STATUS_.log_started = false;
}


void multilog_begin_at (const char *msg, const char *data,
		_CODE_POSITION_T_)
{
	if ( _SUBLOG_.count != 0 &&
			! is_bad_mem(_SUBLOG_.logs, _SUBLOG_.count) )
	{
		write_log("Multilog wasn't ended.", "", WARNING, 0);
		multilog_end(EMPTY);
	}
	else if ( _SUBLOG_.count != 0 )
	{
		char str[50];
		sprintf(str, "_SUBLOG_.count = %zd, _SUBLOG_.logs = %p",
				_SUBLOG_.count, _SUBLOG_.logs);
		write_log("Sublog corrupted!", str, ERROR, 0);
	}

	_SUBLOG_.fname = fname;
	_SUBLOG_.func = func;
	_SUBLOG_.line = line;

	_SUBLOG_.logs = (log_t *) calloc(1, sizeof *_SUBLOG_.logs);
	if (! _SUBLOG_.logs )
	{
		write_log("Sublog cannot be created!", "Allocation error",
				ERROR, 0);
		write_log(msg, data, EMPTY, 1);
		abort();
	}

	strcpy(_SUBLOG_.logs[0].msg, msg);
	strcpy(_SUBLOG_.logs[0].data, data);
	_SUBLOG_.logs[0].danger = EMPTY;
	_SUBLOG_.logs[0].deep_lvl = 0;

	_SUBLOG_.count = 1;
}


void multilog_end (danger_status_t min_printed_danger)
{
	if ( _SUBLOG_.count == 0 )
	{
		write_log("Sublog hasn't start point.",
			"--------------------------------------",
			WARNING, 0);
		reset_sublog();
		return;
	}
	else if ( is_bad_mem(_SUBLOG_.logs,
				_SUBLOG_.count * sizeof *_SUBLOG_.logs) )
	{
		write_log("Sublog was corrupted!", "", ERROR, 0);
		reset_sublog();
		return;
	}
	
	if ( _SUBLOG_.logs[0].danger >= min_printed_danger )
	{
		for (int log = 0; log < _SUBLOG_.count; log++)
			write_log_at(_SUBLOG_.logs[log].msg, _SUBLOG_.logs[log].data,
			  _SUBLOG_.logs[log].danger, _SUBLOG_.logs[log].deep_lvl,
			  _SUBLOG_.fname, _SUBLOG_.func, _SUBLOG_.line);
	}

	free(_SUBLOG_.logs);
	reset_sublog();
}


void add_sublog (const char *msg, const char *data,
		danger_status_t danger, int deep_lvl)
{
	void *realloc_check = realloc(_SUBLOG_.logs,
			(_SUBLOG_.count + 1) * sizeof *_SUBLOG_.logs);
	if (! realloc_check )
	{
		write_log("Failed adding new sublog!",
				"----------------------------------------------",
				ERROR, deep_lvl);
		write_log(msg, data, danger, deep_lvl);
	}
	_SUBLOG_.logs = (log_t *) realloc_check;

	strcpy(_SUBLOG_.logs[_SUBLOG_.count].msg, msg);
	strcpy(_SUBLOG_.logs[_SUBLOG_.count].data, data);
	_SUBLOG_.logs[_SUBLOG_.count].danger = danger;
	_SUBLOG_.logs[_SUBLOG_.count].deep_lvl = deep_lvl;
	
	_SUBLOG_.count++;

	if ( danger > _SUBLOG_.logs[0].danger )
		_SUBLOG_.logs[0].danger = danger;
}


void add_table_log (const char *msg, const void *arr,
		size_t size, size_t element_size,
		void (*print_func)(char *, const void *),
		danger_status_t danger, int deep_lvl)
{
	if ( size < 1 )
	{
		add_sublog("Size of logging array must be positive.", "",
				WARNING, deep_lvl);
		return;
	}
	if ( is_bad_mem(arr, size) )
	{
		add_sublog("Pointer to logging array is bad!", "",
				WARNING, deep_lvl);
		return;
	}

	add_sublog(msg, "==============> Values <==============",
			danger, deep_lvl);
	
	char str[MAX_LOGGING_STRING_LENGTH] = "\0",
	     strtmp[MAX_LOGGING_STRING_LENGTH];
	size_t length = 0, size_data = size * element_size;

	for (size_t i = 0; i < size_data; i += element_size)
	{
		print_func(strtmp, (const char *)arr + i);
		if ( length >= 40 )
		{
			add_sublog("", str, EMPTY, deep_lvl + 1);
			strcpy(str, "");
			length = 0;
		}
		else
		{
			strcpy(str + length, strtmp);
			length += strlen(strtmp) + 2;
			str[length - 2] = ' ';
			str[length - 1] = ' ';
		}
	}
	if ( length > 0 )
		add_sublog("", str, EMPTY, deep_lvl + 1);
}


void write_log_at (const char *msg, const char *data,
		danger_status_t danger, int deep_lvl,
		_CODE_POSITION_T_)
{
	if (! _LOG_STATUS_.log_started )
		return;

	bool logging_success = true;

	char **stack_trace = NULL;
	int stack_trace_size = 0;
	if ( deep_lvl == 0 )
		stack_trace = get_stack_trace(&stack_trace_size);
	
	char danger_str[25];

	if ( _LOG_STATUS_.file )
	{
		gen_danger_str(danger_str, danger, false);
		logging_success &= fprint_log_(_LOG_STATUS_.file, msg, data,
				danger_str, deep_lvl, fname, func, line,
				stack_trace, stack_trace_size);
		fflush(_LOG_STATUS_.file);
	}

	if ( _LOG_STATUS_.log_stdout )
	{
		gen_danger_str(danger_str, danger, true);
		logging_success &= fprint_log_(stdout, msg, data,
				danger_str, deep_lvl, fname, func, line,
				stack_trace, stack_trace_size);
	}

	if ( _LOG_STATUS_.log_stderr )
	{
		gen_danger_str(danger_str, danger, true);
		logging_success &= fprint_log_(stderr, msg, data,
				danger_str, deep_lvl, fname, func, line,
				stack_trace, stack_trace_size);
	}

	free(stack_trace);
	
	if (! logging_success )
	{
		perror("Logging failed!!!\n"
				"The program was interrupted.");
		abort();
	}
}


#endif
