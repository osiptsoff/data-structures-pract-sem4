#include <iostream>
#include <vector>

#include "Tree.h"

Tree generateRandomTree(int size) {
	auto vect = std::vector<int>(size);

	vect[0] = rand();
	for (int i = 1; i < size; ++i)
		vect[i] = vect[i-1] + rand();
	//std::sort(vect.begin(), vect.end());

	return std::move(Tree(vect));
}

Tree treesXor(Tree& first, Tree& second) { // result = first xor second
	auto result = Tree();

	for (auto i : first)
		if (!second.contains(i))
			result.insert(i);

	for (auto i : second)
		if (!first.contains(i))
			result.insert(i);

	return std::move(result);
}

Tree treesExclude(const Tree& first, const Tree& second) { // result = first / second
	auto result = Tree(first);
	for (auto i : second)
		result.remove(i);

	return std::move(result);
}

Tree treesOr(const Tree& first, const Tree& second) { // result = first or second
	auto result = Tree(first);
	result.insert(second.begin());
	return std::move(result);
}

int main() {
	auto vect = std::vector<int>({ 5, 7, 12, 19, 24, 31, 40, 53, 68, 75, 84, 91 });
	auto vect1 = std::vector<int>({5, 12, 21, 657, 11224});
	auto tree = Tree(vect);

	//Тест subst
	/*std::cout << "\n" << tree << "\n\n";
	auto ntree = Tree(vect);
	auto treesubst = tree;
	treesubst.insert(ntree.begin() + 3);
	std::cout << "\n" << treesubst << "\n\n";*/

	//Тест вставки
	/*for (int i = -10; i <= 100; i += 10) {
		tree.insert(i);
		std::cout << "\n" << tree << '\n';
	}
	std::cout << "\n" << tree << "\n\n";*/

	// Тест инкремента и декремена итератора
	/*auto iter = tree.begin();
	for (; iter != tree.end(); ++iter)
		std::cout << *iter << " ";
	std::cout << "\n";
	for (; iter != tree.begin(); --iter)
		std::cout << *iter << " ";
	std::cout << *iter << " \n";
	for (; iter != tree.end(); ++iter)
		std::cout << *iter << " ";*/

	return 0;
}