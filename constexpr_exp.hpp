#include <type_traits>

//! Implementation of cexpr_exp_less_than_1.
template <class FloatType, class = std::enable_if<std::is_floating_point<FloatType>::value>>
FloatType constexpr constexpr_exp_less_than_1_impl(FloatType x, FloatType answer, FloatType xn, FloatType factorial, size_t n)
{
	FloatType next_answer = answer + xn / factorial;
	if (answer == next_answer) {
		return answer;
	}
	n++;
	return constexpr_exp_less_than_1_impl(x, next_answer, xn * x, factorial * n, n);
}

//! Calculate exp(x) for small (-1 < x < 1) x.
template <class FloatType, class = std::enable_if<std::is_floating_point<FloatType>::value>>
FloatType constexpr constexpr_exp_less_than_1(FloatType x)
{
	return constexpr_exp_less_than_1_impl(x, FloatType(1), x, FloatType(1), 1);
}

//! Calculate exp(x).
/*!
 * This function calculate exp(x) by series expansion.
 * For large x, numerator/denominator of intermediate terms of series expansion becomes +-inf.
 * To avoid this, separate exp(x) into exp(n) * exp(x - n) for integer n.
 */
template <class FloatType, class = std::enable_if<std::is_floating_point<FloatType>::value>>
FloatType constexpr constexpr_exp(FloatType x)
{
	FloatType constexpr e = constexpr_exp_less_than_1<FloatType>(1);

	using IntType = int32_t;
	
	if (x == 0) {
		return 1;
	} else if (x > std::numeric_limits<IntType>::max()) {
		return std::numeric_limits<FloatType>::infinity();
	} else if (x < std::numeric_limits<IntType>::lowest()) {
		return 0;
	}
	
	IntType n = (IntType)x;
	FloatType const residual = x - n;
	FloatType answer = 1;
	if (x > 0) {
		for (IntType i = 0; i < n; i++) {
			if (answer > std::numeric_limits<FloatType>::max() / e) {
				return std::numeric_limits<FloatType>::infinity();
			}
			answer *= e;
		}
	} else {
		n = -n;
		for (IntType i = 0; i < n; i++) {
			answer /= e;
			if (answer == 0) {
				return 0;
			}
		}
	}
	FloatType const odd = constexpr_exp_less_than_1(residual);
	if (odd > 1 && answer > std::numeric_limits<FloatType>::max() / odd) {
		return std::numeric_limits<FloatType>::infinity();
	}
	return answer * odd;
}
