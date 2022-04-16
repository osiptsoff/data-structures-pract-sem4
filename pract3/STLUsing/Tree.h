#pragma once

#include <vector>

#include "Node.h"
#include "AccessIterator.h"

class Tree {
protected:
	Node* root;

public:
	Tree(const vector<int>&);
	~Tree() { delete root; }

	AccessIterator begin() { return AccessIterator(root); }
	AccessIterator end() { return AccessIterator(nullptr); };

	friend std::ostream& operator<<(std::ostream& stream, const Tree& tree) {
		//–≈јЋ»«ќ¬ј“№

		return stream;
	}
};

// ѕринима€ на вход остортированный(!) вектор, эта функци€ строит по нему 2-3 дерево
// этот алгоритм подробно проиллюстрирован в лекции
Tree::Tree(const vector<int>& sequence) {
	root = new Node(sequence[0]);

	Node* runner = root;
	Node* upperLayerRunnerPrev = nullptr;
	Node* upperLayerRunner;

	// —оздадим первый уровень: создадим дл€ каждого элемента вектора ноду и соединим все ноды слева направо,
	// корень - сама€ лева€ нода
	for (int size = sequence.size(), i = 1; i < size; ++i) {
		runner->right = new Node(sequence[i]);
		runner = runner->right;
	}

	// пока справа от корн€ есть больше двух нод
	while (root->right != nullptr && root->right->right != nullptr && root->right->right->right != nullptr) {
		runner = root;
		// создадим очередной уновень выше текущего, его началом сделаем корень
		root = new Node(root->value);
		root->down = runner;

		upperLayerRunner = root;

		int nodesWatched = 1;

		//создав новый уровень, проходим по предыдущему: берЄм все тройки нод, отдел€ем их от оставшихс€, создаЄм дл€
		// них управл€юзую ноду и добавл€ем еЄ слева к последей ноде верхнего уровн€ 
		while (runner->right != nullptr) {
			runner = runner->right;
			++nodesWatched;
			if (nodesWatched == 3 && runner->right != nullptr) {
				upperLayerRunner->right = new Node(runner->right->value);
				upperLayerRunner->right->down = runner->right;
				runner->right = nullptr;
				runner = upperLayerRunner->right->down;
				upperLayerRunnerPrev = upperLayerRunner;
				upperLayerRunner = upperLayerRunner->right;
				nodesWatched = 1;
			}
		}

		// если последн€€ нода предыдущего уровн€ осталась одна, то выберем ей пару, отсоеденив 
		// третью ноду из предыдущей тройки
		if (nodesWatched == 1) {
			upperLayerRunner->down = upperLayerRunnerPrev->down->right->right;
			upperLayerRunner->value = upperLayerRunnerPrev->down->right->right->value;
			upperLayerRunner->down->right = runner;
			upperLayerRunnerPrev->down->right->right = nullptr;
		}
	}
}
