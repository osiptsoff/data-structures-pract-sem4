#pragma once

#include <queue>

using std::vector;

//����������: �������� ������ �� �������� ��� ��������� ��������� ������
class Node {
private:
	int value;
	Node* down;
	Node* right;

public:
	friend class Tree;
	friend class AccessIterator;

	Node(int _value) {
		value = _value;
		down = nullptr;
		right = nullptr;
	}

	~Node() {
		delete down;
		delete right;
	}
};