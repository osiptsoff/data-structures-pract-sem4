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
		//�����������

		return stream;
	}
};

// �������� �� ���� ���������������(!) ������, ��� ������� ������ �� ���� 2-3 ������
// ���� �������� �������� ���������������� � ������
Tree::Tree(const vector<int>& sequence) {
	root = new Node(sequence[0]);

	Node* runner = root;
	Node* upperLayerRunnerPrev = nullptr;
	Node* upperLayerRunner;

	// �������� ������ �������: �������� ��� ������� �������� ������� ���� � �������� ��� ���� ����� �������,
	// ������ - ����� ����� ����
	for (int size = sequence.size(), i = 1; i < size; ++i) {
		runner->right = new Node(sequence[i]);
		runner = runner->right;
	}

	// ���� ������ �� ����� ���� ������ ���� ���
	while (root->right != nullptr && root->right->right != nullptr && root->right->right->right != nullptr) {
		runner = root;
		// �������� ��������� ������� ���� ��������, ��� ������� ������� ������
		root = new Node(root->value);
		root->down = runner;

		upperLayerRunner = root;

		int nodesWatched = 1;

		//������ ����� �������, �������� �� �����������: ���� ��� ������ ���, �������� �� �� ����������, ������ ���
		// ��� ����������� ���� � ��������� � ����� � �������� ���� �������� ������ 
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

		// ���� ��������� ���� ����������� ������ �������� ����, �� ������� �� ����, ���������� 
		// ������ ���� �� ���������� ������
		if (nodesWatched == 1) {
			upperLayerRunner->down = upperLayerRunnerPrev->down->right->right;
			upperLayerRunner->value = upperLayerRunnerPrev->down->right->right->value;
			upperLayerRunner->down->right = runner;
			upperLayerRunnerPrev->down->right->right = nullptr;
		}
	}
}
