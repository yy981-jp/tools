#include <vector>
#include <algorithm>

namespace vct {
	template<typename T>
	bool contains(const std::vector<T>& vec, const T& value) {
		return std::find(vec.begin(), vec.end(), value) != vec.end();
	}
}