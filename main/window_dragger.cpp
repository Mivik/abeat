
#include <cmath>

#include "window_dragger.h"

namespace abeat {

GLFWwindow *WindowDragger::window = nullptr;
bool WindowDragger::dragging = false;
int WindowDragger::st_x = 0, WindowDragger::st_y = 0;

void WindowDragger::initialize(GLFWwindow *win) {
	window = win;
	glfwSetMouseButtonCallback(window, mouse_button_callback);
	glfwSetCursorPosCallback(window, cursor_pos_callback);
}

void WindowDragger::mouse_button_callback(GLFWwindow *win, int button, int action, int modes) {
	if (button != GLFW_MOUSE_BUTTON_LEFT) return;
	if (action == GLFW_PRESS) {
		dragging = true;
		double x, y;
		glfwGetCursorPos(window, &x, &y);
		st_x = (int) std::round(x);
		st_y = (int) std::round(y);
	} else if (action == GLFW_RELEASE) dragging = false;
}

void WindowDragger::cursor_pos_callback(GLFWwindow *win, double x, double y) {
	if (!dragging) return;
	int wx, wy;
	glfwGetWindowPos(window, &wx, &wy);
	glfwSetWindowPos(window, wx + (int)std::round(x - st_x), wy + (int)std::round(y - st_y));
}


} // namespace abeat
