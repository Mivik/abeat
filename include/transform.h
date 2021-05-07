
#pragma once

#include <glm/glm.hpp>

#include <cstddef>
#include <vector>

namespace abeat {

std::vector<double> solve_equation(double **val, size_t n_equations, size_t n_vars);

template<int N, int M>
std::vector<double> solve_equation(double (&arr)[N][M], size_t n_equations, size_t n_vars) {
	double *val[n_equations];
	for (size_t i = 0; i < n_equations; ++i) val[i] = arr[i];
	return solve_equation(val, n_equations, n_vars);
}

glm::mat4 solve_transform(
	const glm::vec2 &lb,
	const glm::vec2 &rb,
	const glm::vec2 &lt,
	const glm::vec2 &rt
);

} // namespace abeat
