#pragma once

#include <memory>

using std::unique_ptr;

class Node {
private:
	int value;
	unique_ptr<Node> left;
	unique_ptr<Node> middle;
	unique_ptr<Node> right;

	Node() {
		left = nullptr;
		middle = nullptr;
		right = nullptr;
	}
};