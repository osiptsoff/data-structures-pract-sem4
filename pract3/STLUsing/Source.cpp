#include <iostream>
#include <vector>

#include "Tree.h"

int main() {
	auto vect = std::vector<int>({5, 7, 12, 19, 24, 31, 40, 53, 68, 75, 84, 91, 102, 105, 112, 116});
	auto vect1 = std::vector<int>({ 1, 35, 53, 216});
	auto tree = Tree(vect1);

	//���� subst
	std::cout << "\n" << tree << "\n\n";
	auto ntree = Tree(vect);
	auto treesubst = tree;
	treesubst.insert(ntree.begin() + 3);
	std::cout << "\n" << treesubst << "\n\n";

	//���� �������
	/*for (int i = -10; i < 100; i += 10) {
		tree.insert(i);
		std::cout << "\n" << tree << '\n';
	}
	std::cout << "\n" << tree << "\n\n";*/

	// ���� ���������� � ��������� ���������
	/*auto iter = tree.begin();
	for (; iter != tree.end(); ++iter)
		std::cout << *iter << " ";
	std::cout << "\n";
	for (--iter; iter != tree.begin(); --iter)
		std::cout << *iter << " ";
	std::cout << *iter << " \n";
	for (;iter != tree.end(); ++iter)
		std::cout << *iter << " ";*/

	//���� ���������� ������� ������
	/*Tree empty;
	for (int i = 0; i < 30; ++i) {
		std::cout << empty;
		empty.insert(i, empty.end());
	}
	std::cout << empty;*/

	//���� ������� �� ��������

	return 0;
}