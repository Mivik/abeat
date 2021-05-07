
#pragma once

#include <atomic>
#include <condition_variable>
#include <cstdint>
#include <mutex>
#include <thread>

#include "config.h"

namespace abeat {

class FPSPrinter {
public:
	struct Config {
		bool enabled = true;
		uint64_t interval = 1000;
	};

	explicit FPSPrinter(uint64_t interval);

	void stop();
	inline void count_frame() { ++frame_counter; }

private:
	void mainloop();

	uint64_t interval;
	std::atomic<uint32_t> frame_counter;
	std::condition_variable cv;
	std::mutex mutex;
	bool running;
	std::thread print_thread;
};

} // namespace abeat
