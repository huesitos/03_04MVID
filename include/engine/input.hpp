#ifndef __INPUT_H__
#define __INPUT_H__

#include <vector>

class Input {
public:
	static Input* instance() {
		static Input i;
		return &i;
	}

	void addKey(int key, int action) {
		keys_.emplace_back(key, action);
	}

	void clear() {
		keys_.clear();
	}

	const std::vector<std::pair<int, int>>& getKeys() const {
		return keys_;
	}

private:
	Input() = default;
	~Input() = default;

	Input(const Input&) = default;
	Input(Input&&) = default;
	Input& operator=(const Input&) = default;
	Input& operator=(Input&&) = default;

	std::vector<std::pair<int, int>> keys_;
};

#endif