#include <iostream>
#include <vector>

#include "Tree.h"

int main() {
	auto vect = std::vector<int>({5, 7, 12, 19, 24, 31, 40, 53, 68, 75, 84, 91, 102, 105, 112, 116});
	auto vect1 = std::vector<int>({ 1, 2 });
	auto tree = Tree(vect);

	for (int i = -5; i < 0; ++i) {
		std::cout << tree << '\n';
		tree.insert(i, tree.begin());
	}
	std::cout << tree << '\n';
	return 0;
}