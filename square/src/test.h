/*
 * \file
 * \brief It is header for test.c
 */


#ifndef TEST_H_

#define TEST_H_

#include <ctype.h>

#include "square.h"

#define DEBUG_MODE

/*! \brief Unit test for solve_quadratic_equation function
 * 
 * This function solve_quadratic_equation results
 * with values written to file UnitTest.
 * \param[in] filename___name_of_file___i_am_lazy
 */
void solve_quadratic_equation_test(const char * filename___name_of_file___i_am_lazy);

/*! This function compares obtained values with the reference.
 * 
 * \param[in] correct_n correct number of roots.
 * \param[in] n number of roots obtained.
 * \param[in] correct_x1,correct_x2 correct larger root.
 * \param[in] correct_x2 correct larger root.
 * \param[in] x1 larger root obtained.
 * \param[in] x1 smaller root obtained.
 * \return if comparison failed function returns true, else it returns false.
 */
bool incorrect_answer(const enum count_of_roots correct_n,
		const enum count_of_roots n, const double correct_x1, const double x1,
		const double correct_x2, const double x2);


#endif
