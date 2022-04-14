#pragma once

#include <memory>
#include <vector>
#include <iostream>

using std::unique_ptr;
using std::weak_ptr;
using std::vector;

class Node {
public:
	int value;
	Node* down;
	Node* right;

public:

	Node(int _value) {
		value = _value;
		down = nullptr;
		right = nullptr;
	}
	Node(const vector<int> &sequence) {
		Node* root = new Node(sequence[0]);
		Node* runner = root;
		Node* upperLayerRunnerPrev = nullptr;
		Node* upperLayerRunner;

		for (int size = sequence.size(), i = 1; i < size; ++i) {
			runner->right = new Node(sequence[i]);
			runner = runner->right;
		}

		while (root->rightsCount() > 2) {
			runner = root;

			dbout(root);
			std::cout << root->rightsCount() << std::endl;

			// Дебаг - вывод значений корня на каждом шаге
			root = new Node(root->value);
			root->down = runner;
			//

			upperLayerRunner = root;

			int nodesWatched = 1;

			while(runner->right != nullptr) {
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

			if (nodesWatched == 1) {
				upperLayerRunner->down = upperLayerRunnerPrev->down->right->right;
				upperLayerRunner->value = upperLayerRunnerPrev->down->right->right->value;
				upperLayerRunner->down->right = runner;
				upperLayerRunnerPrev->down->right->right = nullptr;
			}
		}

		value = root->value;;
		down = root->down;
		right = root->right;

		// Дебаг - вывод значений корня
		dbout(root);
		std::cout << root->rightsCount() << std::endl;
		//
	}

	~Node() {
		delete down;
		delete right; 
	}

	int rightsCount() {
		int number = 0;

		for (Node* runner = right; runner != nullptr; runner = runner->right, ++number);

		return number;
	}

	// Потом удалить - нужна только для дебага конструктора
	void dbout(Node* root) {
		for (Node* runner = root; runner != nullptr; runner = runner->right) std::cout << runner->value << " ";
		std::cout << std::endl;

	}

	std::ostream& operator<<(std::ostream &stream) {
		// РЕАЛИЗОВАТЬ
	}
};