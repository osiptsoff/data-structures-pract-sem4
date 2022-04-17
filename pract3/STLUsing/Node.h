#pragma once

#include <iostream>



//����������: �������� ������ �� �������� ��� ��������� ��������� ������
class Node {
private:
	int value;
	Node* down;
	Node* right;

public:
	friend class Tree;
	friend class AccessIterator;
	friend std::ostream& operator<<(std::ostream&, const Tree&);

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