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

	AccessIterator insert(AccessIterator, int);
public:
	Tree() : root(nullptr) {} ;
	Tree(const vector<int>&);

	Tree(const Tree&);
	Tree(Tree&& other) noexcept : root(other.root) { other.root = nullptr; };

	Tree& operator = (const Tree&);
	Tree& operator = (Tree&&) noexcept;

	~Tree() { delete root; };

	AccessIterator begin() {
		Node* first;
		for (first = root; first != nullptr && first->down != nullptr; first = first->down);
		return AccessIterator(root, first); 
	}
	AccessIterator end() { return AccessIterator(root, nullptr); }

	AccessIterator begin() const {
		Node* first;
		for (first = root; first != nullptr && first->down != nullptr; first = first->down);
		return AccessIterator(root, first);
	}
	AccessIterator end() const { return AccessIterator(root, nullptr); }

	AccessIterator insert(int);
	void insert(AccessIterator); // ������� subst

	void erase(AccessIterator);

	friend std::ostream& operator<<(std::ostream& stream, const Tree& tree) {
		if (tree.root == nullptr)
			return stream;

		if (tree.root->right == nullptr) {
			std::cout << tree.root->value << " @ @ |\n";
			return stream;
		}

		queue <Node*> que;
		que.push(tree.root);
		Node* elem;
		int rootValue = tree.root->value;

		while (!que.empty()) {
			elem = que.front();
			if(elem->value == rootValue) std::cout << '\n';
			
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
Tree::Tree(const vector<int>& sequence) : root(nullptr) {
	if (sequence.empty())
		return;

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

Tree::Tree(const Tree& other) {
	std::vector<int> vector;
	for (auto obj : other)
		vector.push_back(obj);

	Tree tmp = Tree(vector);
	root = tmp.root;
	tmp.root = nullptr;
}

Tree& Tree::operator = (const Tree& other) {
	if (this != &other) {
		std::vector<int> vector;
		for (auto obj : other)
			vector.push_back(obj);

		Tree tmp = Tree(vector);
		delete root;
		root = tmp.root;
		tmp.root = nullptr;
	}
	return *this;
}

Tree& Tree::operator = (Tree&& other) noexcept {
	if (this != &other) {
		root = other.root;
		other.root = nullptr;
	}
	return *this;
}

AccessIterator Tree::insert(AccessIterator where, int value) {
	Node* nodeStart = where.currentValue;
	Node* runner;
	int n = 1;

	// ���� where ��������� �� nullptr (�.�. ������ ������ ��� where ��������� �� ����� ������)
	if (where.currentValue == nullptr)
		// ���� ������ ������
		if (where.treeRoot == nullptr) {
			root = new Node(nullptr, value);

			return AccessIterator(root, root);
		}
		else {
			auto newWhere = ++insert(--where, value);
			auto runner = newWhere.currentValue->parent;
			for (; runner->parent != nullptr && runner->parent->value == runner->value; runner = runner->parent)
				runner->value = newWhere.currentValue->value;
			runner->value = newWhere.currentValue->value;
			newWhere.currentValue->value = value;

			return newWhere;
		}

	// �������� ����������� �� ����� where, ������ ���������� ����� ����� �����
	runner = where.currentValue;
	Node* oldRight = runner->right;

	runner->right = new Node(runner, runner->value);
	runner->right->right = oldRight;
	if (oldRight != nullptr) oldRight->parent = runner->right;

	for (; runner->parent != nullptr && runner->value == runner->parent->value; runner = runner->parent) runner->value = value;
	runner->value = value;

	// ���������, ������� ����� � ����
	while (nodeStart->parent != nullptr && nodeStart->value != nodeStart->parent->value) nodeStart = nodeStart->parent;
	for (runner = nodeStart; runner->right != nullptr; runner = runner->right) ++n;

	// ���� ���������� � ���� ��������� 3 �����, �� ������ �� 4. ����� ����������� ������
	while (n > 3) {
		// ���� �� ��������� ����� � �������� ����, ������ ����� ��������
		if (nodeStart->parent == nullptr) {
			Node* underRoot = new Node(nodeStart, nodeStart->value);
			underRoot->down = nodeStart->down;
			if(nodeStart->down != nullptr) nodeStart->down->parent = underRoot;
			nodeStart->down = underRoot;

			underRoot->right = new Node(underRoot, nodeStart->right->value);
			underRoot->right->down = nodeStart->right->down;
			if (nodeStart->right->down != nullptr) nodeStart->right->down->parent = underRoot->right;
			nodeStart->right->down = nodeStart->right->right;
			nodeStart->right->value = nodeStart->right->right->value;
			nodeStart->right->right->parent = nodeStart->right;
			nodeStart->right->right = nullptr;
		}
		else {
			nodeStart->parent->value = nodeStart->value;

			runner = nodeStart->parent;
			oldRight = runner->right;
			runner->right = new Node(runner, nodeStart->right->right->value);
			runner->right->right = oldRight;
			if (oldRight != nullptr) oldRight->parent = runner->right;

			runner->right->down = nodeStart->right->right;
			nodeStart->right->right->parent = runner->right;
			nodeStart->right->right = nullptr;

			nodeStart = nodeStart->parent;
		}

		// ���������� ���������� ����� � ��������� ����, ����� ������, ����� �� ���������� �����������
		while (nodeStart->parent != nullptr && nodeStart->value != nodeStart->parent->value) nodeStart = nodeStart->parent;
		for (runner = nodeStart, n = 1; runner->right != nullptr; runner = runner->right) n++;
	}

	// ����� ����� ��������, ����������� �� ���� �� where, ��� ��� ��� ��������� �������� ������ � ����
	// ��������� �������, � ��� ����� ������� ����� ���� ��������
	return where;
}

// ��� ������������� ������ � �������� ����������� ���������� ����������
AccessIterator Tree::insert(int value) {
	if (root == nullptr)
		return insert(end(), value);

	auto runner = root;
	while (runner->down != nullptr) {
		for (; runner->right != nullptr && runner->right->value <= value; runner = runner->right);
		runner = runner->down;
	}
	for (; runner->right != nullptr && runner->value < value; runner = runner->right);

	if(runner->value < value)
		return insert(++AccessIterator(root, runner), value);
	else if(runner->value != value)
		return insert(AccessIterator(root, runner), value);
}

void Tree::insert(AccessIterator otherStart) { 
	for (; otherStart.currentValue != nullptr; ++otherStart) insert(*otherStart);
}

void Tree::erase(AccessIterator where) {
	Node* removable = where.currentValue;
	if (removable->parent == nullptr) {
		std::cout << "Work with root\n";
		root = removable->right;
		root->parent = nullptr;
		removable->right = nullptr;
		delete removable;
	}
	else {
		Node* runner = removable;
		int n = 1;
		if (removable->parent->value == removable->value) {
			removable->parent->down = removable->right;
			for (; runner->parent != nullptr && runner->parent->value == removable->value; runner = runner->parent)
				runner->parent->value = removable->right->value;
			removable->right->parent = removable->parent;
		}
		else {
			removable->parent->right = removable->right;
			if (removable->right) removable->right->parent = removable->parent;
		}
		runner = removable->parent;
		std::cout << std::endl << "Deleted " << removable->value;
		if (removable->right) removable->right = nullptr;
		delete removable;

		std::cout << std::endl << "Runner = " << runner->value; // ��� ������� �������� ������� ������ ��������?
		// ����������� ������, ���� ����������
		if (runner->down != nullptr && runner->value == runner->down->value) runner = runner->down;
		else if (runner->parent->down == nullptr && runner->parent->value == runner->parent->parent->value) runner = runner->parent;
		n = 1;
		for (; runner->right != nullptr; runner = runner->right) ++n; // �������, ������� ����� � ����
		std::cout << std::endl << "Runner = " << runner->value;
		std::cout << std::endl << "Now nodes " << n << std::endl;
		if (n == 1) { // ������� ���� ����
			std::cout << "One son\n";
			removable = runner;
			std::string str = "1"; // ���������� ��� ����������� ��������� �� �������������
			if (removable->parent->right) {
				std::cout << "Go on right nodes\n";
				runner = removable->parent->right->down; // ������� � ������������ ������ 2/3 ������
				for (; runner->right != nullptr; runner = runner->right) ++n;
				if (n == 2) { // ��������� ������ �� ���� �������
					std::cout << "For one son next two nodes\n";
					Node* deleted = removable->parent->right;
					runner = runner->parent;
					removable->right = runner;
					runner->parent = removable;
					if (deleted->right != nullptr) {
						deleted->right->parent = deleted->parent;
						deleted->parent = deleted->right;
						deleted->right = nullptr;
					}
					else removable->parent->right = nullptr;
					deleted->down = nullptr;
					delete deleted;

					if (!runner->parent->parent->right && runner->parent->parent->value == runner->parent->parent->parent->value) { // � ������ ������� ���� ��������
						std::cout << "One parent\n";
						n = 1;
						if (removable->parent->parent->right) { // ������� � ������������ ������ 2/3 ���������
							runner = removable->parent->parent->right->down;
							for (; runner->right != nullptr; runner = runner->right) ++n;
							if (n == 2) str = "r2";
							else str = "r3";
						}
						else { // ������� � ������������ ����� 2/3 ���������
							runner = removable->parent->parent->parent->down;
							for (; runner->right != nullptr; runner = runner->right) ++n;
							if (n == 2)	str = "l2";
							else str = "l3";
						}
					}
				}
				else { // ��������� ������ �� ���� �������
					std::cout << "For one son next tree nodes\n";
					runner = runner->parent->parent;
					runner->parent->value = runner->right->value;
					runner->parent->down = runner->right;
					runner->right->parent = runner->parent;
					removable->right = runner;
					runner->parent = removable;
					runner->right = nullptr;
				}
			}
			else {
				std::cout << "Go on left nodes\n";
				runner = removable->parent->parent->down; // ������� � ������������ ����� 2/3 �������
				for (; runner->right != nullptr; runner = runner->right) ++n;
				if (n == 2) { // ���������� ������ �� ���� �������
					std::cout << "For one son before two nodes\n";
					runner = runner->parent;
					Node* deleted = removable->parent;
					runner->right->right = removable;
					removable->parent = runner->right;
					if (deleted->right != nullptr) {
						runner->parent->right = deleted->right;
						deleted->right->parent = runner->parent;
						deleted->right = nullptr;
					}
					else runner->parent->right = nullptr;
					deleted->down = nullptr;
					delete deleted;

					if (!runner->parent->right && runner->parent->value == runner->parent->parent->value) { // � ������ ������� ���� ��������
						std::cout << "One parent\n";
						n = 1;
						removable = runner;
						if (runner->parent->parent->right) { // ������� � ������������ ������ 2/3 ���������
							runner = runner->parent->parent->right->down;
							for (; runner->right != nullptr; runner = runner->right) ++n;
							while (runner->value != runner->parent->value) runner = runner->parent;
							if (n == 2) str = "r2";
							else str = "r3";
						}
						else { // ������� � ������������ ����� 2/3 ���������
							runner = runner->parent->parent->parent->down;
							for (; runner->right != nullptr; runner = runner->right) ++n;
							if (n == 2)	str = "l2";
							else str = "l3";
						}
					}
				}
				else { // ���������� ������ �� ���� �������
					std::cout << "For one son before tree nodes\n";
					removable->parent->value = runner->right->right->value;
					runner->right->right->parent = removable->parent;
					removable->parent->down = runner->right->right;
					removable->parent = runner->right->right;
					runner->right->right = nullptr;
				}
			}

			// ����������� �� ���������
			if (str == "r2") {
				std::cout << "For one parent next two parent\n";
				Node* deleted = runner->parent;
				removable->parent->parent->right = runner->parent->right;
				if (runner->parent->right) runner->parent->right->parent = removable->parent->parent;
				removable->parent->right = runner;
				runner->parent = removable->parent;
				deleted->right = nullptr;
				deleted->down = nullptr;
				delete deleted;
			}
			else if (str == "r3") {
				std::cout << "For one parent next tree parent\n";
				runner->parent->value = runner->right->value; // ��������: ���� ������ ���� �������, �������� �������������� � ���������� ������
				runner->parent->down = runner->right;
				runner->right->parent = runner->parent;
				runner->right = nullptr;
				runner->parent = removable->parent;
				removable->parent->right = runner;
			}
			else if (str == "l2") {
				std::cout << "For one parent before two parent\n";
				Node* deleted = runner->parent->parent->right;
				removable->parent->parent = runner;
				runner->right = removable->parent;
				runner->parent->parent->right = nullptr;
				deleted->right = nullptr;
				deleted->down = nullptr;
				delete deleted;
			}
			else if (str == "l3") {
				std::cout << "For one parent before tree parent\n";
				removable->parent->parent->value = runner->value; // ��������: ���� ������ ���� �������, �������� �������������� � ���������� ������
				runner->parent->right = nullptr;
				removable->parent->parent->down = runner;
				runner->parent = removable->parent->parent;
				runner->right = removable->parent;
				removable->parent->parent = runner;
			}
		}
	}
}