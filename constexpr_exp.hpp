#include <type_traits>

//! Implementation of cexpr_exp_less_than_1.
template <class FloatType, class = std::enable_if<std::is_floating_point<FloatType>::value>>
FloatType constexpr cexpr_exp_impl(FloatType x, FloatType answer, FloatType xn, FloatType factorial, size_t n)
{
	FloatType next_answer = answer + xn / factorial;
	if (answer == next_answer) {
		return answer;
	}
	n++;
	return cexpr_exp_impl(x, next_answer, xn * x, factorial * n, n);
}

//! Calculate exp(x) for small (-1 < x < 1) x.
template <class FloatType, class = std::enable_if<std::is_floating_point<FloatType>::value>>
FloatType constexpr cexpr_exp_less_than_1(FloatType x)
{
	return cexpr_exp_impl(x, FloatType(1), x, FloatType(1), 1);
}

//! Calculate exp(x).
/*!
 * This function calculate exp(x) by series expansion.
 * For large x, numerator/denominator of intermediate terms of series expansion becomes +-inf.
 * To avoid this, separate exp(x) into exp(n) * exp(x - n) for integer n.
 */
template <class FloatType, class = std::enable_if<std::is_floating_point<FloatType>::value>>
FloatType constexpr cexpr_exp(FloatType x)
{
	FloatType constexpr e = cexpr_exp_less_than_1(FloatType(1));

	int n = (int)x;
	FloatType const residual = x - n;
	FloatType answer = 1;
	if (x > 0) {
		for (int i = 0; i < n; i++) {
			answer *= e;
		}
	} else {
		n = -n;
		for (int i = 0; i < n; i++) {
			answer /= e;
		}
	}
	FloatType const odd = cexpr_exp_less_than_1(residual);
	return answer * odd;
}
