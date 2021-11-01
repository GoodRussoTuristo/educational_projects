/*
 * \file
 * \brief It is header for square.c
 */


#ifndef SQUARE_H_

#define SQUARE_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <assert.h>


/*! The number of possible roots of a quadratic equation. */
enum count_of_roots
{
	INF_ROOTS = -1,
	ZERO_ROOTS = 0,
	ONE_ROOT = 1,
	TWO_ROOTS = 2
};

/*! Presicion of real numbers comparison. */
const double ACCURACY = 1.0e-4;

/*! Number of data entry attempts. */
const unsigned int N_ATTEMPTS = 5;

/*! This function solves quadratic equation with given
 *  coefficients.
 *
 * \param[in]  a  coefficient at x squared of quadratic equation.
 * \param[in]  b  coefficient at x of quadratic equation.
 * \param[in]  c  coefficient at \fS x powered 0 of quadratic equation.
 * \param[out] x1 pointers to variables to which this function will assign larger root values.
 * \param[out] x2 pointers to variables to which this function will assign smaller root values.
 *
 * \return        Number of roots.
 * 
 * \note If equation has one root x1 and x2 get the same values.
 */
enum count_of_roots solve_quadratic_equation(const double a,
		                             const double b, 
					     const double c, 
					     double *x1, 
					     double *x2);

/*! This function solves linear equation with given coefficients.
 *
 * \param[in] k coefficient at x of linear equation,
 * \param[in] b coefficient at x powered 0  of linear equation,
 * \param[out] x pointer to variable to which this function will assign root value.
 * \return number of roots.
 * \note IF equation haven't roots x doesn't get value
 */
enum count_of_roots solve_linear_equation(const double k, const double b, double *x);

/*! This function reads three double values from stdin.
 *
 * \param[out] a,b,c Pointers to variables that will be assigned values obtained from stdin.
 * \return function returns true if values are read successfuly else it returns false
 */
bool get_input(double *a, double *b, double *c);

/*! This function cleans input buffer.
 * \note This function uses getchar()
 */
void clean_buffer(void);

/*! This function outputs roots of quadratic equation to stdout.
 *
 * \param[in] n_roots number of roots.
 * \param[in] x1,x2 roots of quadratic equation.
 */
void print_results(const enum count_of_roots n_roots, const double x1, const double x2);

/*! This function compares two double numbers with accurancy.
 *
 * \param[in] a,b compared numbers.
 * \return if numbers equals function returns true, else it returns false.
 */
int equals_for_doubles(const double a, const double b);

#endif
