#include <iostream>
#include <vector>

#include "Node.h"


int main() {
	auto vect = std::vector<int>({5, 7, 12, 19, 24, 31, 40, 53, 68, 75, 84, 91, 102, 105, 112, 116});
	auto vect1 = std::vector<int>({ 1, 2, 3, 4, 5, 6, 7, 8 });
	auto foo = Node(vect1);

	//std::cout << foo.value << " " << foo.right->value << " " << std::endl;
	//std::cout << foo.right->value << " " << foo.right->down->value << " " << foo.right->down->right->value << std::endl;

	return 0;
}