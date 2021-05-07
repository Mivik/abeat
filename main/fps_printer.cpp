
#include <chrono>
#include <cmath>
#include <iostream>

#include <fmt/format.h>

#include "fps_printer.h"

namespace abeat {

FPSPrinter::FPSPrinter(uint64_t interval):
	interval(interval),
	frame_counter(0),
	running(true),
	print_thread([this] { mainloop(); }) {}

void FPSPrinter::mainloop() {
	while (({
		std::unique_lock<std::mutex> lock(mutex);
		!cv.wait_for(lock, std::chrono::milliseconds(interval), [this]() { return !running; });
	})) {
		const int cnt = frame_counter;
		frame_counter = 0;
		fmt::print(stderr, "{}fps\n", (size_t) std::round((double) cnt / interval * 1000));
	}
}

void FPSPrinter::stop() {
	{
		std::unique_lock<std::mutex> lock(mutex);
		running = false;
		cv.notify_one();
	}
	print_thread.join();
}

} // namespace abeat
