#include <iostream>
#include <vector>

#include "Tree.h"

int main() {
	//auto vect = std::vector<int>({ 1, 2, 4, 5, 7, 12, 14, 17, 19, 21, 23, 24, 29, 31, 35, 37, 40, 43, 45, 48, 53, 68, 70, 73, 75, 79, 84, 87, 89, 91, 95, 98, 99});
	auto vect = std::vector<int>({ 1, 2});
	auto vect1 = std::vector<int>({ 5, 12 });

	for (int i = 3; i < 150; i++) vect.push_back(i);
	
	auto tree = Tree(vect);

	//Тест erase
	std::cout << tree << "\n\n";
	//tree.erase(tree.begin());
	//std::cout << tree << "\n";

	/*for (; tree.begin() != tree.end(); tree.erase(tree.begin())) {
		std::cout << tree << "\n";
	}*/

	//tree.erase(tree.begin());
	//std::cout << tree << "\n";

	for (int i = 0; i < 100; i++) {
		tree.erase(tree.end() - 4);  //  tree.end() - 2    tree.begin() + 7
		std::cout << tree << "\n";
	}
	
	//for (auto i : tree) std::cout << i << ' ';

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
	/*Tree empty;
	for (int i = 0; i < 30; ++i) {
		std::cout << empty;
		empty.insert(i);
	}
	std::cout << empty;*/

	//Тест вставки по значению

	return 0;
}