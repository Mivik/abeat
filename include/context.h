
#pragma once

#include <unordered_set>
#include <vector>

#include "common.h"

namespace abeat {

class Object;

class Animation;

class Stream;

class Widget;

class Context {
public:
	Context() = default;
	Context(const Context &) = delete;
	Context(Context &&t) noexcept;
	Context &operator=(Context &&) noexcept = default;

	~Context();

	bool insert_object(Object *obj);
	bool release_object(Object *obj);

	std::vector<Object*> get_objects() const;

	template<class T, class...Args>
	inline T *new_object(Args &&...args) {
		T *ptr = new T(std::forward<Args>(args)...);
		insert_object(ptr);
		return ptr;
	}

private:
	std::unordered_set<Object *> objects;
};

} // namespace abeat
