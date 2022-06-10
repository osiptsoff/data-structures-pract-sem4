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
	result.insert(second.begin());
	return std::move(result);
}

Tree generateRandomTree(int size) {
	auto vect = std::vector<int>(size);

	vect[0] = rand();
	for (int i = 1; i < size; ++i)
		vect[i] = vect[i - 1] + rand();

	return std::move(Tree(vect));
}

int main() {
	const int minSize = 10;
	const int maxSize = 200;
	const int sizeStep = 10;
	const bool debugMode = false;

	Tree a, b, c, d, e;
	Tree dMinusE;
	Tree cXorUpper;
	Tree aOrB;
	Tree result;

	std::ofstream stream("in.txt");

	//srand(time(nullptr));

	for (int size = minSize; size <= maxSize; size += sizeStep) {
		a = generateRandomTree(size);
		b = generateRandomTree(size);
		c = generateRandomTree(size);
		d = generateRandomTree(size);
		e = generateRandomTree(size);

		if (debugMode) std::cout << "\na\n" << a << "\nb\n" << b << "\nc\n" << c << "\nd\n" << d << "\ne\n" << e << "\n\n";
		
		auto start = std::chrono::high_resolution_clock::now();

		dMinusE = treesMinus(d, e);
		if (debugMode) std::cout << "d minus e" << dMinusE << "\n\n";
		cXorUpper = treesXor(c, dMinusE);
		if (debugMode) std::cout << "c xor (d minus e)" << cXorUpper << "\n\n";
		aOrB = treesOr(a, b);
		if (debugMode) std::cout << "a or b" << aOrB << "\n\n";
		result = treesOr(aOrB, cXorUpper);
		if (debugMode) std::cout << "result" << result << "\n\n";

		auto end = std::chrono::high_resolution_clock::now();
		auto dt = duration_cast<duration<double>>(end - start);
		stream << size << " " << dt.count() << "\n";
	}

	stream.close();

	return 0;
}