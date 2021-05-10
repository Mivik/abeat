
#pragma once

#include <map>
#include <memory>
#include <optional>
#include <stdexcept>
#include <type_traits>

template<class T>
using Ptr = std::unique_ptr<T>;

template<class T>
using SPtr = std::shared_ptr<T>;

template<class K, class V>
inline std::optional<V> try_get(const std::map<K, V> &mp, const K &key) {
	auto it = mp.find(key);
	if (it == mp.cend()) return std::nullopt;
	return it->second;
}

template<class V, class = std::enable_if_t<std::is_arithmetic_v<V>>>
inline V *alloc_filled(size_t size, V value) {
	V *ret = new V[size];
	std::fill(ret, ret + size, value);
	return ret;
}

template<class T, class F>
inline std::shared_ptr<T> force_ptr_cast(
	const std::shared_ptr<F> &ptr,
	const std::string &error_msg = "Failed to perform dynamic cast"
) {
	using result_t = std::shared_ptr<T>;
	if (auto *r = dynamic_cast<typename result_t::element_type *>(ptr.get()))
		return result_t(ptr, r);
	throw std::runtime_error(error_msg);
}
