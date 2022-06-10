#include <iostream>
#include <vector>

#include "Tree.h"

Tree generateRandomTree(int size, int dElem) {
	auto vect = std::vector<int>(size);

	vect[0] = rand() % dElem;
	for (int i = 1; i < size; ++i)
		vect[i] = vect[i - 1] + rand() % dElem + 1;

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
	result.subst(second.begin());
	return std::move(result);
}

int main() {
	const int maxSize = 15;
	const int maxDel = 15;

	Tree À, B, C, D, E;
	Tree dMinusE;
	Tree cXorUpper;
	Tree aOrB;
	Tree result;

	srand(time(nullptr));

	À = generateRandomTree(rand() % maxSize + 1, maxDel);
	B = generateRandomTree(rand() % maxSize + 1, maxDel);
	C = generateRandomTree(rand() % maxSize + 1, maxDel);
	D = generateRandomTree(rand() % maxSize + 1, maxDel);
	E = generateRandomTree(rand() % maxSize + 1, maxDel);

	std::cout << "\nA" << À << "\n\nB" << B << "\n\nC" << C << "\n\nD" << D << "\n\nE" << E << "\n\n";

	dMinusE = treesMinus(D, E);
	std::cout << "D\\E" << dMinusE << "\n\n";
	cXorUpper = treesXor(C, dMinusE);
	std::cout << "C XOR D\\E" << cXorUpper << "\n\n";
	aOrB = treesOr(À, B);
	std::cout << "A or B" << aOrB << "\n\n";
	result = treesOr(aOrB, cXorUpper);
	std::cout << "Result = A or B or C XOR D\\E" << result << "\n\n";
	return 0;
}