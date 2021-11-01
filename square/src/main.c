#include "square.h"
#include "test.h"

/*! Solves quadratic equation with coefficients
 * entered from stdin. In debug mode this function runs unit tests.
 */
int main(void)
{
	#ifdef DEBUG_MODE
	 	solve_quadratic_equation_test("UnitTest");
	
	#else
		enum count_of_roots n_roots = ZERO_ROOTS;
		double a = NAN, b = NAN, c = NAN, x1 = NAN, x2 = NAN;
	
		if (!get_input(&a, &b, &c))
		{
			puts("The program ended ahead of shedule.\n");
			return EXIT_FAILURE;
		}

		n_roots = solve_quadratic_equation(a, b, c, &x1, &x2);

		print_results(n_roots, x1, x2);

	#endif

	return 0;
}
