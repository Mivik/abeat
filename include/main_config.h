
#pragma once

#include "config.h"
#include "fps_printer.h"

namespace abeat {

struct MainConfig {
	config::Window window;
	FPSPrinter::Config fps_printer;
	config::Render render;

	class Builder;
};

class MainConfig::Builder {
BUILDER_BEGIN_S(MainConfig)
	BUILDER_OPT(window)
	BUILDER_OPT(fps_printer)
	BUILDER_OPT(render)
BUILDER_END_S(true)
};

} // namespace abeat
