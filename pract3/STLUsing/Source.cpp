#include <iostream>
#include <vector>

#include "Tree.h"

int main() {
	auto vect = std::vector<int>({5, 7, 12, 19, 24, 31, 40, 53, 68, 75, 84, 91, 102, 105, 112, 116});
	auto vect1 = std::vector<int>({ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10});

	auto tree = Tree(vect);
	Tree copy_tree;
	copy_tree = tree;

	std::cout << tree << '\n' << copy_tree << '\n';
	return 0;
}