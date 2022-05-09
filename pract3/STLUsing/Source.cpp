#include <iostream>
#include <vector>

#include "Tree.h"

int main() {
	auto vect = std::vector<int>({5, 7, 12, 19, 24, 31, 40, 53, 68, 75, 84, 91, 102, 105, 112, 116});
	auto vect1 = std::vector<int>({ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10});
	auto tree = Tree(vect1);

	for (int i = 0; i < 5; ++i) {
		std::cout << tree << '\n';
		tree.insert(i, tree.begin());
	}
	return 0;
}