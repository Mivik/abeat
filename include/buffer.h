
#pragma once

#include <cstdint>
#include <mutex>
#include <vector>

namespace abeat {

template<class T>
class Buffer {
public:
	explicit Buffer(size_t size): data(nullptr), size(0) { resize(size); }
	~Buffer() {
		auto lock = this->lock();
		delete[] data;
	}
	inline std::unique_lock<std::mutex> lock() { return std::unique_lock(mutex); }
	inline void resize(size_t size) {
		auto lock = this->lock();
		if (this->size == size) return;
		delete[] data;
		data = new T[this->size = size];
		std::fill(data, data + size, 0);
		has_new_data = false;
	}
	inline void write(T *src, size_t n) {
		{
			auto lock = this->lock();
			has_new_data = true;
			$write(src, std::min(n, size));
		}
	}
	inline void write_offset(T *src, size_t n, size_t gap, size_t offset) {
		auto lock = this->lock();
		has_new_data = true;

		n = std::min((n - offset - 1) / gap + 1, size);
		if (inter_buf.size() < n) inter_buf.resize(n);
		for (size_t i = 0, j = offset; i < n; i++, j += gap)
			inter_buf[i] = src[j];
		$write(inter_buf.data(), n);
	}
	[[nodiscard]] inline size_t get_size() const { return size; }

	bool has_new_data;
	T *data;
private:
	inline void $write(T *src, size_t n) {
		std::copy(data + n, data + size, data);
		std::copy(src, src + n, data + size - n);
	}

	std::mutex mutex;
	std::vector<T> inter_buf;
	size_t size;
};

} // namespace abeat
