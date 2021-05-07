
#include "context.h"
#include "object.h"

namespace abeat {

Context::Context(Context &&t) noexcept:
	objects(std::move(t.objects)) {}

Context::~Context() {
	for (auto *obj : objects) delete obj;
}

bool Context::insert_object(Object *obj) {
	return objects.insert(obj).second;
}

bool Context::release_object(Object *obj) {
	if (objects.erase(obj)) {
		delete obj;
		return true;
	}
	return false;
}

std::vector<Object *> Context::get_objects() const {
	return std::vector<Object *>(objects.begin(), objects.end());
}

} // namespace abeat
