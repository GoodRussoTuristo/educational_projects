/*! \file
 *  Logging library header file.
 */




#ifndef _LOGGING_H_

#define _LOGGING_H_


/*================= Connectiong headers ==================*/


#include "logging.config.h"


#if LOGGING == ON


#include <stddef.h>
#include <stdbool.h>


#endif




/*========================= Types ========================*/


typedef enum danger_status_t_
{
	EMPTY   = 0,
	OK      = 1,
	WARNING = 2,
	ERROR =   3,
} danger_status_t;




#define _CODE_POSITION_T_ const char *fname, const char *func, int line

#define _CODE_POSITION_ fname, func, line

#define _CURRENT_CODE_POSITION_ __FILE__, __func__, __LINE__


/*================== Function prototypes =================*/




#if LOGGING == ON


bool set_logfile (const char *fname);


void remove_logfile (void);


void set_stdout_logging (bool val);


void set_stderr_logging (bool val);


void start_logging_func_ (void);


void stop_logging_func_ (void);


void multilog_begin_at (const char *msg, const char *data,
		_CODE_POSITION_T_);


void multilog_end (danger_status_t min_printed_danger);


void add_sublog (const char *msg, const char *data,
		danger_status_t danger, int deep_lvl);


void add_table_log (const char *msg, const void *arr,
		size_t size, size_t element_size,
		void (*print_func)(char *, const void *),
		danger_status_t danger, int deep_lvl);


void write_log_at (const char *msg, const char *data,
		danger_status_t danger, int deep_lvl,
		_CODE_POSITION_T_);




/*================== Functional macros ===================*/


#define start_logging() \
{\
	start_logging_func_();\
	write_log("Logging was started...", " ", EMPTY, 0);\
} (void) 0


#define stop_logging() \
{\
	write_log("Logging was stopped...", "", EMPTY, 0);\
	stop_logging_func_();\
} (void) 0


#define multilog_begin(MSG_, DATA_) \
{\
	multilog_begin_at(MSG_, DATA_, _CURRENT_CODE_POSITION_);\
} (void) 0


#define if_log(ASSERTION_, DANGER_STATUS_) if (\
	(ASSERTION_) && (\
	write_log_at("Assertion failed:", #ASSERTION_, DANGER_STATUS_,\
			0, _CURRENT_CODE_POSITION_), true) )


#define write_log(MESSAGE_, DATA_, DANGER_STATUS_, DEEP_LVL_) \
{\
	write_log_at(MESSAGE_, DATA_,\
			DANGER_STATUS_, DEEP_LVL_, _CURRENT_CODE_POSITION_);\
} (void) 0




/*==================== Empty macros ======================*/


#else


#define start_logging() (void) 0


#define stop_logging() (void) 0


#define multilog_begin(MSG_, DATA_)


#define multilog_begin_at(MSG_, DATA_, _CODE_POSITION_T_)


#define multilog_end(min_printed_danger)


#define add_sublog(msg, data, danger, deep_lvl)


#define add_table_log(msg, arr, n, print_func, danger, deep_lvl)


#define if_log(ASSERTION_, DANGER_STATUS_) if ( ASSERTION_ )


#define write_log(MESSAGE_, DATA_, DANGER_STATUS_, DEEP_LVL_) (void) 0


#define set_logfile(fname) 1 /* true */


#define remove_logfile() (void) 0


#define set_stdout_logging(val) (void) 0


#define set_stderr_logging(val) (void) 0


#define start_logging_func_() (void) 0


#define stop_logging_func_() (void) 0


#define write_log_at(msg, data, danger, deep_lvl, fname, func, line) (void) 0


#endif


#endif
