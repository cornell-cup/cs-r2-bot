#pragma once

#include <string>
#include <exception>

namespace R2D2 {

	// Returns milliseconds since the Unix epoch.
	uint64_t timeSinceEpoch();

	template<typename T, typename... Args>
	std::unique_ptr<T> make(bool shouldMake, Args&&... args) {
		if (shouldMake) {
			return std::make_unique<T>(std::forward<Args>(args)...);
		} else {
			return std::unique_ptr<T>();
		}
	}
}