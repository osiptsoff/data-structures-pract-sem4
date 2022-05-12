#pragma once

#include <iterator>
#include <stack>
#include <deque>

#include "Node.h"

class AccessIterator : public std::iterator<std::forward_iterator_tag, int> {
protected:
	Node* treeRoot;
	Node* currentValue;
	std::deque<Node*> after;
	std::stack<Node*> before;

	AccessIterator(Node*);
	AccessIterator(Node*, Node*);
public:
	friend class Tree;

	AccessIterator() = delete;

	bool operator==(const AccessIterator& other) { return this->currentValue == other.currentValue; }
	bool operator!=(const AccessIterator& other) { return this->currentValue != other.currentValue; }

	pointer operator-> () { return &currentValue->value; }
	reference operator* () { return currentValue->value; }

	AccessIterator operator++ ();
	AccessIterator operator++ (int);
	AccessIterator operator+ (int);
	AccessIterator operator-- ();
	AccessIterator operator-- (int);
	AccessIterator operator- (int);
};

// Укажет на начало
AccessIterator::AccessIterator(Node* _root) : treeRoot(_root), currentValue(nullptr) {
	if (_root == nullptr) return;

	after.push_back(treeRoot);

	// пока у последней ноды в очереди есть нижний сын, будем выполнять по дереву поиск в ширину
	// в конце концов, в очереди останутся только самые левые ноды троек и двоек самого нижнего уровня
	while (after.front()->down != nullptr) {
		after.push_back(after.front()->down);
		after.push_back(after.front()->right->down);
		if (after.front()->right->right != nullptr)
			after.push_back(after.front()->right->right->down);
		after.pop_front();
	}
	currentValue = after.front();
}

// Укажет на переданную ноду
AccessIterator::AccessIterator(Node* _root, Node* _currentValue) : AccessIterator(_root) {
	for (; currentValue != _currentValue; ++(*this));
}

AccessIterator AccessIterator::operator++ () {
	if (currentValue == nullptr)
		throw std::out_of_range("Out of container");

	// after используется, как очередь
	// если у текущей ноды есть правый сын, следующим будет он
	if (currentValue->right != nullptr) {
		currentValue = currentValue->right;
	}
	// в другом случае, убираем её из очереди. Следующей будет нода из начала очереди
	else {
		before.push(after.front());
		after.pop_front();

		if (after.empty())
			currentValue = nullptr;
		else
			currentValue = after.front();
	}
	return *this;
}

AccessIterator AccessIterator::operator++ (int) {
	auto temp(*this);
	++(*this);
	return temp;
}

AccessIterator AccessIterator::operator+ (int offset) {
	if (offset < 0)
		throw std::invalid_argument("Use substraction");

	auto result(*this);
	try { 
		for (int i = 0; i < offset; ++i) ++result; 
	}
	catch (std::out_of_range e) {
		throw e;
	}

	return result;
}

AccessIterator AccessIterator::operator-- () {	
	// если дерево пустое, или итератор указывает на корень, или итератор указывает на самый левый лист
	if (treeRoot == nullptr || currentValue != nullptr && (currentValue->parent == nullptr 
		|| currentValue->parent->value == currentValue->value && before.empty()))
		throw std::out_of_range("Out of container");

	// after используется, как стек
	// если итератор указывает на конец дерева или на самое левое число в узле, 
	// то перейти на самое правое число предыдущей ноды
	if (currentValue == nullptr && treeRoot != nullptr || currentValue->parent->value == currentValue->value) {
		
		for (currentValue = before.top(); currentValue->right != nullptr; currentValue = currentValue->right);

		after.push_front(before.top());
		before.pop();
	}
	// в другом случае, перейти на число левее того, куда указывает итератор
	else currentValue = currentValue->parent;

	return *this;
}

AccessIterator AccessIterator::operator-- (int) {
	auto temp(*this);
	--(*this);
	return temp;
}

AccessIterator AccessIterator::operator- (int offset) {
	if (offset < 0)
		throw std::invalid_argument("Use addition");

	auto result(*this);
	try {
		for (int i = 0; i < offset; ++i) --result;
	}
	catch (std::out_of_range e) {
		throw e;
	}

	return result;
}