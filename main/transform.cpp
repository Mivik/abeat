
#include <cassert>
#include <cmath>
#include <stdexcept>

#include "transform.h"

namespace abeat {

std::vector<double> solve_equation(double **val, size_t n_equations, size_t n_vars) {
	assert(n_equations >= n_vars);
	for (size_t i = 0; i < n_vars; ++i) {
		size_t mx = i;
		for (size_t j = i + 1; j < n_equations; ++j)
			if (std::abs(val[j][i]) > std::abs(val[mx][i])) mx = j;
		if (mx != i)
			for (size_t j = 0; j <= n_vars; ++j) std::swap(val[i][j], val[mx][j]);
		for (size_t j = 0; j < n_equations; ++j) {
			if (i == j) continue;
			const double scale = val[j][i] / val[i][i];
			for (size_t k = i; k <= n_vars; ++k)
				val[j][k] -= val[i][k] * scale;
		}
	}
	std::vector<double> ret(n_vars);
	for (size_t i = 0; i < n_vars; ++i)
		ret[i] = val[i][n_vars] / val[i][i];
	return ret;
}

inline bool on_right(const glm::vec2 &A, const glm::vec2 &B, const glm::vec2 &C) {
	return (B.x - A.x) * (C.y - A.y) - (B.y - A.y) * (C.x - A.x) < 0;
}

glm::mat4 solve_transform(
	const glm::vec2 &lb,
	const glm::vec2 &rb,
	const glm::vec2 &lt,
	const glm::vec2 &rt
) {
	{
		const bool A = on_right(lb, rb, lt), B = on_right(rb, rt, lt),
			C = on_right(rt, lt, lb), D = on_right(lt, lb, rb);
		if (A != B || A != C || A != D)
			throw std::invalid_argument(
				"Four points used to determine the transform matrix should form a convex hull.");
	}
	double v[8][9] = {};
	int top = 0;
	auto add_val = [&](const glm::vec2 &fr, int offset, double result) {
		v[top][offset] = fr.x;
		v[top][offset + 1] = fr.y;
		v[top][offset + 2] = 1;
		v[top][6] = -fr.x * result;
		v[top][7] = -fr.y * result;
		v[top++][8] = result;
	};
	auto add = [&](const glm::vec2 &fr, const glm::vec2 &to) {
		add_val(fr, 0, to.x);
		add_val(fr, 3, to.y);
	};
	add({ -1, -1 }, lb);
	add({ 1, -1 }, rb);
	add({ -1, 1 }, lt);
	add({ 1, 1 }, rt);
	auto ans = abeat::solve_equation(v, 8, 8);
	return {
		ans[0], ans[3], 0, ans[6],
		ans[1], ans[4], 0, ans[7],
		0, 0, 1, 0,
		ans[2], ans[5], 0, 1
	};
}

} // namespace abeat
