/*!
 * \file
 * \prief This file helps you to solve your quadratic equation.
 */

#include "square.h"

enum count_of_roots solve_quadratic_equation(const double a,
		const double b, const double c, double *x1, double *x2)
{
	assert(x1 != x2);
	assert(x1);
	assert(x2);
	assert(isfinite(a));
	assert(isfinite(b));
	assert(isfinite(c));

	double d = NAN;

	if (equals_for_doubles(a, 0))
	{
		enum count_of_roots n_linear_equation_roots = solve_linear_equation(b, c, x1);

		*x2 = *x1;

		return n_linear_equation_roots;
	}
	else
	{
		double doubled_a = 2 * a;

		double d = b * b - 4 * a * c;

		if (equals_for_doubles(d, 0))
		{
			*x1 = *x2 = -b / (doubled_a);

			return ONE_ROOT;
		}
		else
			if (d > 0)
			{
				double sqrt_d = sqrt(d);

				*x1 = (-b + sqrt_d) / (doubled_a);
				*x2 = (-b - sqrt_d) / (doubled_a);

				return TWO_ROOTS;
			}
			else
			{
				return ZERO_ROOTS;
			}
	}
}

enum count_of_roots solve_linear_equation(const double k,
		const double b, double *x)
{
	assert(x);
	assert(isfinite(k));
	assert(isfinite(b));

	if (equals_for_doubles(k, 0))
	{
		if (equals_for_doubles(b, 0))
			return INF_ROOTS;

		else
			return ZERO_ROOTS;
	}
	else
	{
		*x = -b / k;
		return ONE_ROOT;
	}
}

bool get_input(double *a, double *b, double *c)
{
	assert(a);
	assert(b);
	assert(c);
	assert(a != b);
	assert(b != c);
	assert(a != c);

	puts("Solve your quadratic equation.\n\n");

	puts("Enter quadratic coefficients: ");
	unsigned int attempts_left = N_ATTEMPTS;

	while (scanf("%lg %lg %lg", a, b, c) != 3 && attempts_left--)
	{
		puts("You have a mistake.\nPlease, try again\n");
		printf("%u attempts left.", attempts_left);

		clean_buffer();
	}
	
	if (!attempts_left)
	{
		puts("You haven't attempts left.\n");
		return false;
	}
	
	return true;
}


void clean_buffer(void)
{
	while (getchar() != '\n')
		continue;
}

void print_results(const enum count_of_roots n_roots, const double x1, const double x2)
{
	assert(isfinite(x1));
	assert(isfinite(x2));

	switch (n_roots)
	{
		case ZERO_ROOTS:
			puts("Your equation haven't roots.\n");
			break;
	
		case INF_ROOTS:
			puts("You have infinitly many roots.\n");
			break;
	
		case ONE_ROOT:
			printf("x = %f\n", x1);
			break;
	
		case TWO_ROOTS:
			printf("x1 = %.4f, x2 = %.4f\n", x1, x2);
			break;
	
		default:
			fputs("ERROR!\n", stderr);
			break;
	}
}

int equals_for_doubles(const double a, const double b)
{
	if (isnan(a) && isnan(b))
		return true;
	else if (a == b)
		return true;		
	else
		return fabs( a-b ) < ACCURACY;
}


