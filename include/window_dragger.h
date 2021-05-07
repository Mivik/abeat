
#pragma once

#include <GLFW/glfw3.h>

namespace abeat {

class WindowDragger {
public:
	static void initialize(GLFWwindow *win);
private:
	static void mouse_button_callback(GLFWwindow *win, int button, int action, int modes);
	static void cursor_pos_callback(GLFWwindow *win, double x, double y);

	static GLFWwindow *window;
	static bool dragging;
	static int st_x, st_y;
};

} // namespace abeat
