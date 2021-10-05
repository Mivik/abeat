
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
		using namespace animation;
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
		auto window = Transform::Builder(ctx)
			.input(spec)
			.function([](float x) {
				x = map_range(x, { -80, 0 }, { 0, 1 });
				return x * x;
			})
			.build();
		auto grav = Gravity::Builder(ctx)
			.input(window)
			.build();
		auto scale = Scale::Builder(ctx)
			.input(
				Transform::Builder(ctx)
					.input(Smoother::Builder(ctx).input(window).build())
					.function([](float x) {
						return map_range(x, { 0, 1 }, { 0.7, 1.5 });
					})
					.build()
			)
			.build();
		std::vector<Animation *> animations = {
			scale
		};
		auto curve = Curve::Builder(ctx)
			.input(grav)
			.circular(true)
			.filled(true)
			.animations(animations)
			.build();
		auto edge = Curve::Builder(ctx)
			.input(grav)
			.circular(true)
			.color({ 1, 1, 1, 1 })
			.animations(animations)
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
	conf.window.width = 512;
	conf.window.height = 512;
	conf.window.decorated = false;
	conf.window.draggable = true;
	conf.window.background = { 0, 0, 0, 0 };

	glfwSetErrorCallback([](int error, const char *desc) {
		std::cerr << "Error: " << desc << " (" << error << ")\n";
	});
	if (!glfwInit()) {
		std::cerr << "Failed to initialize glfw" << std::endl;
		return -1;
	}
	glfwWindowHint(GLFW_FLOATING, GLFW_TRUE);
	glfwWindowHint(GLFW_DECORATED, conf.window.decorated);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, GLFW_TRUE);
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