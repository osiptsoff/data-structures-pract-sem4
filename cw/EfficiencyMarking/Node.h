#pragma once

class Node {
public: //private:
	int value;
	Node* down;
	Node* right;
	Node* parent;

public:
	friend class Tree;
	friend class AccessIterator;
	friend std::ostream& operator<<(std::ostream&, const Tree&);

	Node() = delete;
	Node(const Node&) = delete;
	Node(Node* _parent, int _value) {
		value = _value;
		down = nullptr;
		right = nullptr;
		parent = _parent;
	}

	~Node() {
		delete down;
		delete right;
	}

	Node& operator=(const Node&) = delete;
};