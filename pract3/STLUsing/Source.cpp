#include <iostream>
#include <vector>

#include "Tree.h"

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

	//Тест заполнения пустого дерева
	Tree empty;
	for (int i = 0; i < 30; ++i) {
		std::cout << empty;
		empty.insert(i);
	}
	std::cout << empty;

	//Тест вставки по значению

	return 0;
}