#pragma once

#include <vector>
#include <queue>

#include "Node.h"
#include "AccessIterator.h"

using std::queue;
using std::vector;

class Tree {
protected:
	Node* root;

public:
	Tree(const vector<int>&);
	~Tree() { delete root; }

	AccessIterator begin() { return AccessIterator(root); }
	AccessIterator end() { return AccessIterator(nullptr); }

	AccessIterator Insert(int, AccessIterator);

	friend std::ostream& operator<<(std::ostream& stream, const Tree& tree) {
		queue <Node*> que;
		que.push(tree.root);
		Node* elem;
		int rootValue = tree.root->value;

		while (!que.empty()) {
			elem = que.front();
			if(elem->value == rootValue) std::cout << std::endl;
			
			std::cout << elem->value << " ";
			std::cout << elem->right->value << " ";
			if (elem->right->right != nullptr) std::cout << elem->right->right->value << " ";
			else std::cout << "@";
			std::cout << " | ";
			que.pop();

			if (elem->down != nullptr) 
				que.push(elem->down);
			if (elem->right->down != nullptr)
				que.push(elem->right->down);
			if (elem->right->right != nullptr &&elem->right->right->down != nullptr)
				que.push(elem->right->right->down);
		}
		return stream;
	}
};

// �������� �� ���� ���������������(!) ������, ��� ������� ������ �� ���� 2-3 ������
// ���� �������� �������� ���������������� � ������
Tree::Tree(const vector<int>& sequence) {
	root = new Node(nullptr, sequence[0]);

	Node* runner = root;
	Node* upperLayerRunnerPrev = nullptr;
	Node* upperLayerRunner;

	// �������� ������ �������: �������� ��� ������� �������� ������� ���� � �������� ��� ���� ����� �������,
	// ������ - ����� ����� ����
	for (int size = sequence.size(), i = 1; i < size; ++i) {
		runner->right = new Node(runner, sequence[i]);
		runner = runner->right;
	}

	// ���� ������ �� ����� ���� ������ ���� ���
	while (root->right != nullptr && root->right->right != nullptr && root->right->right->right != nullptr) {
		runner = root;
		// �������� ��������� ������� ���� ��������, ��� ������� ������� ������
		root = new Node(nullptr, root->value);
		root->down = runner;
		root->down->parent = root;

		upperLayerRunner = root;

		int nodesWatched = 1;

		//������ ����� �������, �������� �� �����������: ���� ��� ������ ���, �������� �� �� ����������, ������ ���
		// ��� ����������� ���� � ��������� � ����� � �������� ���� �������� ������ 
		while (runner->right != nullptr) {
			runner = runner->right;
			++nodesWatched;
			if (nodesWatched == 3 && runner->right != nullptr) {
				upperLayerRunner->right = new Node(upperLayerRunner, runner->right->value);
				upperLayerRunner->right->down = runner->right;
				runner->right->parent = upperLayerRunner->right;
				runner->right = nullptr;
				runner = upperLayerRunner->right->down;
				upperLayerRunnerPrev = upperLayerRunner;
				upperLayerRunner = upperLayerRunner->right;
				nodesWatched = 1;
			}
		}

		// ���� ��������� ���� ����������� ������ �������� ����, �� ������� �� ����, ���������� 
		// ������ ���� �� ���������� ������
		if (nodesWatched == 1) {
			upperLayerRunner->down = upperLayerRunnerPrev->down->right->right;
			upperLayerRunnerPrev->down->right->right->parent = upperLayerRunner;
			upperLayerRunner->value = upperLayerRunnerPrev->down->right->right->value;
			upperLayerRunner->down->right = runner;
			runner->parent = upperLayerRunner->down;
			upperLayerRunnerPrev->down->right->right = nullptr;
		}
	}
}

AccessIterator Tree::Insert(int value, AccessIterator where) {
	Node* runner = where.currentValue;
	int n = 0;

	while (runner->value != runner->parent->value) runner = runner->parent;
	for(; runner->right != nullptr; runner = runner->right) n++;

	if (n < 3) {
		runner = where.currentValue;
		Node* oldRight = runner->right;
		runner->right = new Node(runner, runner->value);
		runner->value = value;

	}

	return this->begin();
}
