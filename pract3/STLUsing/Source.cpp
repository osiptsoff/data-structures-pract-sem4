#include <iostream>
#include <vector>

#include "Tree.h"

int main() {
	auto vect = std::vector<int>({5, 7, 12, 19, 24, 31, 40, 53, 68, 75, 84, 91, 102, 105, 112, 116});
	auto vect1 = std::vector<int>({ 1, 2});
	auto tree = Tree(vect);

	for (int i = -10; i < 0; ++i) {
		tree.insert(i, ++++++tree.begin());
		std::cout << "\n" << tree << '\n';
	}
	std::cout << "\n" << tree << '\n';
	return 0;
}