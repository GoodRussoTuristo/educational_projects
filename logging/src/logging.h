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


/*! This type includes different warnings
 *  which can be displayed in log file
 */
typedef enum danger_status_t_
{
	EMPTY   = 0, /*!< Nothing will be printed */
	OK      = 1, /*!< [OK] will be printed */
	WARNING = 2, /*!< ==> WARNING will be printed */
	ERROR =   3, /*!< !!! ERROR will be printed*/
} danger_status_t;




/*! This macro used in function declarations.
 */
#define _CODE_POSITION_T_ const char *fname, const char *func, int line

/*! This macro used in some functions to shorten the code.
 */
#define _CODE_POSITION_ fname, func, line

/*! This macro defines current position in source code.
 */
#define _CURRENT_CODE_POSITION_ __FILE__, __func__, __LINE__


/*================== Function prototypes =================*/




#if LOGGING == ON


/*! This function sets the file for logging.
 *
 *  \param[in] fname - name of the file where logs will be written.
 */
bool set_logfile (const char *fname);


/*! This function stops logging to file.
 *
 */
void remove_logfile (void);


/*! This function sets status of logging to stdout
 * 
 *  \param[in] val - a variable that determines
 *  whether logging to stdout will be done.
 */
void set_stdout_logging (bool val);


/*! This function sets status of logging to stderr
 * 
 *  \param[in] val - a variable that determines
 *  whether logging to stderr will be done.
 */
void set_stderr_logging (bool val);


/*! This function starts logging process.
 *
 *  \note Use macro start_logging() instead of this function.
 */
void start_logging_func_ (void);


/*! This function stops logging process.
 *
 * \note Use macro stop_logging() instead of this function.
 */
void stop_logging_func_ (void);


/*! This function sets the beginning of a large log
 *  consisting of many nested logs.
 *
 *  \param[in] msg  - description of multilog.
 *  \param[in] data - data to be printed
 *  in the header of a multi-log.
 *  \param[in] _CODE_POSITION_T_ - file name, function and line
 *  that will be printed in the header of the multi-log.
 */
void multilog_begin_at (const char *msg, const char *data,
		_CODE_POSITION_T_);


/*! THis function sets the end of multi-log and print it
 *  if max warning status in multi-log is not less
 *  than the min_printed_danger.
 *
 *  \param[in] min_printed_danger - min warning status,
 *  starting from which the multi-log will be printed.
 */
void multilog_end (danger_status_t min_printed_danger);


/*! This function adds log to multi-log.
 *
 *  \param[in] msg       - description of log.
 *  \param[in] data      - data that should be printed in this log.
 *  \param[in] danger    - warning status of this log.
 *  \param[int] deep_lvl - nesting level of this log.
 */
void add_sublog (const char *msg, const char *data,
		danger_status_t danger, int deep_lvl);


/*! This function adds data array in the form of a table
 *  to the multi-log.
 *
 *  \param[in] msg          - message that will be printed in the head of the table.
 *  \param[in] arr          - data that will be printed.
 *  \param[in] size         - size of array.
 *  \param[in] element_size - size of one element in printed array.
 *  \param[in] print_func   - function which converts the data to a string.
 *  \param[in] danger       - warning status of this log.
 *  \param[in] deep_lvl     - nesting level of this log.
 */
void add_table_log (const char *msg, const void *arr,
		size_t size, size_t element_size,
		void (*print_func)(char *, const void *),
		danger_status_t danger, int deep_lvl);


/*! This function writes log to the previously assigned places.
 *
 *  \param[in] msg             - description of log.
 *  \param[in] data            - data that should be printed in this log.
 *  \param[in] danger          - warning status of this log.
 *  \param[in] deep_lvl        - nesting level of this log.
 *  \param[in] _CODE_POSITION_ - file name, function and line
 *  that will be printed in the header of the multi-log.
 *
 *  \note If logging failed function crashes the program.
 */
void write_log_at (const char *msg, const char *data,
		danger_status_t danger, int deep_lvl,
		_CODE_POSITION_T_);




/*================== Functional macros ===================*/


/*! This macro starts logging process.
 *
 */
#define start_logging() \
{\
	start_logging_func_();\
	write_log("Logging was started...", " ", EMPTY, 0);\
} (void) 0


/*! This macro stops the logging process.
 *
 */
#define stop_logging() \
{\
	write_log("Logging was stopped...", "", EMPTY, 0);\
	stop_logging_func_();\
} (void) 0


/*! This macro sets the beginning of a large log
 *  consisting of many nested logs determining the current position
 *  in the source code.
 *
 *  \param[in] MSG_  - description of multilog.
 *  \param[in] DATA_ - data to be printed
 *  in the header of a multi-log.
 */
#define multilog_begin(MSG_, DATA_) \
{\
	multilog_begin_at(MSG_, DATA_, _CURRENT_CODE_POSITION_);\
} (void) 0


/*! This macro is a replacement for the usual if, which writes the log
 *  if the assertion is true.
 *
 *  \param[in] ASSERTION_    - boolean expression to be checked for truth.
 *  \param[in] DANGER_STATUS - warning status of log that may be written.
 */
#define if_log(ASSERTION_, DANGER_STATUS_) if (\
	(ASSERTION_) && (\
	write_log_at("Assertion failed:", #ASSERTION_, DANGER_STATUS_,\
			0, _CURRENT_CODE_POSITION_), true) )


/*! This macro writes log to the previously assigned places
 *  determining the current position in the source code..
 *
 *  \param[in] msg             - description of log.
 *  \param[in] data            - data that should be printed in this log.
 *  \param[in] danger          - warning status of this log.
 *  \param[in] deep_lvl        - nesting level of this log.
 */
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
