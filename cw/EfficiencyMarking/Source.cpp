#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <chrono>

#include "Tree.h"

using namespace std::chrono;

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

Tree generateRandomTree(int size, int dElem) {
	auto vect = std::vector<int>(size);

	vect[0] = rand() % dElem;
	for (int i = 1; i < size; ++i)
		vect[i] = vect[i - 1] + rand() % dElem + 1;

	return std::move(Tree(vect));
}

int main() {
	const int minSize = 10;
	const int maxSize = 1005;
	const int sizeStep = 5;
	const int maxDel = 15;
	const bool debugMode = 0;

	Tree A, B, C, D, E;
	Tree dMinusE;
	Tree cXorUpper;
	Tree aOrB;
	Tree result;

	std::ofstream stream("in.txt");

	//srand(time(nullptr));

	for (int size = minSize, i = 1; size <= maxSize; size += sizeStep, i++) {
		A = generateRandomTree(size, maxDel);
		B = generateRandomTree(size, maxDel);
		C = generateRandomTree(size, maxDel);
		D = generateRandomTree(size, maxDel);
		E = generateRandomTree(size, maxDel);

		std::cout << "Test " << i << "\n";
		if (debugMode) std::cout << "\n\nA" << A << "\n\nB" << B << "\n\nC" << C << "\n\nD" << D << "\n\nE" << E << "\n\n";
		
		auto start = std::chrono::high_resolution_clock::now();

		dMinusE = treesMinus(D, E);
		if (debugMode) std::cout << "D\\E" << dMinusE << "\n\n";
		cXorUpper = treesXor(C, dMinusE);
		if (debugMode) std::cout << "C XOR D\\E" << cXorUpper << "\n\n";
		aOrB = treesOr(A, B);
		if (debugMode) std::cout << "A or B" << aOrB << "\n\n";
		result = treesOr(aOrB, cXorUpper);
		if (debugMode) std::cout << "Result = A or B or C XOR D\\E" << result << "\n\n";

		auto end = std::chrono::high_resolution_clock::now();
		auto dt = duration_cast<duration<double>>(end - start);
		stream << size << " " << dt.count() << "\n";
	}

	stream.close();
	return 0;
}