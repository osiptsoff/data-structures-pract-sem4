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

Tree treesMinus(const Tree& first, const Tree& second) { // result = first / second
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
	const int maxSize = 15;

	Tree a, b, c, d, e;
	Tree dMinusE;
	Tree cXorUpper;
	Tree aOrB;
	Tree result;

	srand(time(nullptr));

	a = generateRandomTree(rand() % maxSize + 1);
	b = generateRandomTree(rand() % maxSize + 1);
	c = generateRandomTree(rand() % maxSize + 1);
	d = generateRandomTree(rand() % maxSize + 1);
	e = generateRandomTree(rand() % maxSize + 1);

	std::cout << "\na\n" << a << "\nb\n" << b << "\nc\n" << c << "\nd\n" << d << "\ne\n" << e << "\n\n";

	dMinusE = treesMinus(d, e);
	std::cout << "d minus e" << dMinusE << "\n\n";
	cXorUpper = treesXor(c, dMinusE);
	std::cout << "c xor (d minus e)" << cXorUpper << "\n\n";
	aOrB = treesOr(a, b);
	std::cout << "a or b" << aOrB << "\n\n";
	result = treesOr(aOrB, cXorUpper);
	std::cout << "result" << result << "\n\n";

	return 0;
}