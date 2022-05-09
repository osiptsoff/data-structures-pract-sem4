#pragma once

#include <iterator>
#include <queue>

#include "Node.h"

// ƒќ–јЅќ“ј“№: добавить возможность дл€ каждого листа получить доступ ко всему дереву
class AccessIterator : public std::iterator<std::forward_iterator_tag, int> {
protected:
	Node* treeRoot;
	Node* currentValue;
	std::queue<Node*> que;
	AccessIterator(Node* current, int) : currentValue(current) {};

public:
	friend class Tree;

	AccessIterator(Node* _root);

	bool operator==(const AccessIterator& other) { return this->currentValue == other.currentValue; }
	bool operator!=(const AccessIterator& other) { return this->currentValue != other.currentValue; }

	pointer operator-> () { return &currentValue->value; }
	reference operator* () { return currentValue->value; }

	AccessIterator operator++ ();
	AccessIterator operator++ (int);
};

AccessIterator::AccessIterator(Node* _root) : treeRoot(_root), currentValue(nullptr), que() {
	if (_root == nullptr) return;

	que.push(treeRoot);

	// пока у последней ноды в очереди есть нижний сын, будем выполн€ть по дереву поиск в ширину
	// в конце концов, в очереди останутс€ только самые левые ноды троек и двоек самого нижнего уровн€
	while (que.front()->down != nullptr) {
		que.push(que.front()->down);
		que.push(que.front()->right->down);
		if (que.front()->right->right != nullptr)
			que.push(que.front()->right->right->down);
		que.pop();
	}

	currentValue = que.front();
}

AccessIterator AccessIterator::operator++ () {
	if (currentValue == nullptr)
		throw std::out_of_range("Out of container");

	// если у текущей ноды есть правый сын, следующим будет он
	if (currentValue->right != nullptr) {
		currentValue = currentValue->right;
	}
	// в другом случае, убираем еЄ из очереди. —ледующей будет нода из начала очереди
	else {
		que.pop();

		if (que.empty())
			currentValue = nullptr;
		else
			currentValue = que.front();
	}
	return *this;
}

AccessIterator AccessIterator::operator++ (int) {
	auto temp(*this);
	++(*this);
	return temp;
}