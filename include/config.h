
#pragma once

#include <cassert>
#include <cstdint>

#include "context.h"
#include "mgl.h"

#define BUILDER_BEGIN(name) \
public: \
    using OutputType = name; \
    using ConfigType = name::Config; \
    \
    explicit Builder(Context &ctx): ctx(ctx) {}

#define BUILDER_BEGIN_S(name) \
public: \
    using ConfigType = name; \
    \
    explicit Builder(Context &ctx): ctx(ctx) {}

#define BUILDER_ARG(name) inline Builder& name(decltype(ConfigType::name) name) { config.name = std::move(name); f_##name = true; return *this; }
#define BUILDER_OPT(name) inline Builder& name(decltype(ConfigType::name) name) { config.name = std::move(name); return *this; }

#define BUILDER_END(flag) \
public: \
    inline OutputType* build() const { \
        assert((flag) && "Config incomplete"); \
        return ctx.new_object<OutputType>(config); \
    } \
private: \
    Context &ctx; \
    ConfigType config;

#define BUILDER_END_S(flag) \
public: \
    inline ConfigType build() const { \
        assert((flag) && "Config incomplete"); \
        return config; \
    } \
private: \
    Context &ctx; \
    ConfigType config;

namespace abeat {

template<class T>
struct Range {
	T min, max;

	inline T length() const { return max - min; }
};

namespace config {

struct Window {
	bool decorated = true, draggable = true;
	size_t width = 1000, height = 350;
	mgl::Color background = { 1.f, 1.f, 1.f, 1.f };

	class Builder;
};

class Window::Builder {
BUILDER_BEGIN_S(Window)
	BUILDER_OPT(decorated)
	BUILDER_OPT(draggable)
	BUILDER_OPT(width)
	BUILDER_OPT(height)
	BUILDER_OPT(background)
BUILDER_END_S(true)
};

struct Render {
	bool MSAA = true;
	uint32_t samples = 4, max_fps = 60;

	class Builder;
};

class Render::Builder {
BUILDER_BEGIN_S(Render)
	BUILDER_OPT(MSAA)
	BUILDER_OPT(samples)
	BUILDER_OPT(max_fps)
BUILDER_END_S(true)
};

} // namespace config

} // namespace abeat
