
#include <chrono>
#include <iostream>

#include "common.h"
#include "context.h"
#include "fps_printer.h"
#include "main_config.h"
#include "objects.h"
#include "window_dragger.h"

Ptr<abeat::FPSPrinter> fps_printer;
abeat::MainConfig conf;
Ptr<mgl::MSAA> msaa;
std::unique_ptr<abeat::Context> ctx_ptr;

inline void mainloop(GLFWwindow *window) {
	using namespace abeat;
	using namespace std::chrono;

	time_point<steady_clock> cur_time, last_time;
	if (conf.fps_printer.enabled)
		fps_printer = std::make_unique<FPSPrinter>(conf.fps_printer.interval);

	auto &ctx = *ctx_ptr;

	{
		using namespace stream;
		using namespace widget;
		auto input = PAInput::Builder(ctx)
			.build();
		auto fft = FFT::Builder(ctx)
			.input(input)
			.size(2048)
			.build();
		auto spec = Spectrum::Builder(ctx)
			.input(fft)
			.freq({ 0, 1500 })
			.initial_value(-80)
			.build();
		auto window = Window::Builder(ctx)
			.input(spec)
			.from_range({ -80, 0 })
			.to_range({ 0, 1 })
			.build();
		auto grav = Gravity::Builder(ctx)
			.input(window)
			.build();
		auto bars = Bars::Builder(ctx)
			.input(grav)
			.build();
	}

	auto objects = ctx.get_objects();

	const auto frame_gap_time = microseconds(1000 * 1000 / conf.render.max_fps - 100);

	while (!glfwWindowShouldClose(window)) {
		cur_time = steady_clock::now();
		const float dt = duration<float>(cur_time - last_time).count();

		if (msaa) msaa->bind();
		glClearColor(EX_COLOR(conf.window.background));
		glClear(GL_COLOR_BUFFER_BIT);

		for (auto &obj : objects) obj->update(dt);

		if (msaa) msaa->blit();
		glfwSwapBuffers(window);
		glfwPollEvents();
		if (fps_printer) fps_printer->count_frame();
		std::this_thread::sleep_until(cur_time + frame_gap_time);
		last_time = cur_time;
	}
	msaa = nullptr;
	if (fps_printer) fps_printer->stop();
}

int main(int argc, char *argv[]) {
	conf.window.width = 70;
	conf.window.height = 70;

	if (!glfwInit()) {
		std::cerr << "Failed to initialize glfw" << std::endl;
		return -1;
	}
	glfwWindowHint(GLFW_DECORATED, conf.window.decorated);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, 1);
	GLFWwindow *window = glfwCreateWindow(conf.window.width, conf.window.height, "abeat", nullptr, nullptr);
	if (!window) {
		std::cerr << "Failed to open GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
		std::cerr << "Failed to initialize GLAD" << std::endl;
		glfwTerminate();
		return -1;
	}
	if (conf.render.MSAA)
		msaa = std::make_unique<mgl::MSAA>(conf.render.samples, conf.window.width, conf.window.height);
	glEnable(GL_MULTISAMPLE);
	glfwSetFramebufferSizeCallback(window, [](GLFWwindow *window, int width, int height) {
		if (msaa) msaa->resize(width, height);
		glViewport(0, 0, width, height);
	});
	if (conf.window.draggable) abeat::WindowDragger::initialize(window);

	ctx_ptr = std::make_unique<abeat::Context>();
	mainloop(window);
	ctx_ptr.reset();
	glfwTerminate();
	return 0;
}