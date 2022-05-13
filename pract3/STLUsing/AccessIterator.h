#pragma once

#include <iterator>

#include "Node.h"

class AccessIterator : public std::iterator<std::forward_iterator_tag, int> {
protected:
	Node* treeRoot;
	Node* currentValue;

	AccessIterator(Node* root, Node* cValue) : treeRoot(root), currentValue(cValue) {};
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

AccessIterator AccessIterator::operator++ () {
	if (currentValue == nullptr)
		throw std::out_of_range("Out of container");

	if (currentValue->right != nullptr) currentValue = currentValue->right;
	else {
		do {
			for (; currentValue->parent != nullptr && currentValue->parent->value != currentValue->value; currentValue = currentValue->parent);
			currentValue = currentValue->parent;
		} while (currentValue != nullptr && currentValue->right == nullptr);

		if (currentValue != nullptr) {
			currentValue = currentValue->right;
			for (; currentValue->down != nullptr; currentValue = currentValue->down);
		}
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
	if (treeRoot == nullptr || currentValue != nullptr && treeRoot->value == currentValue->value)
		throw std::out_of_range("Out of container");

	// если указывает на конец
	if (currentValue == nullptr) {
		currentValue = treeRoot;
		while (currentValue->down != nullptr) {
			for (; currentValue->right != nullptr; currentValue = currentValue->right);
			currentValue = currentValue->down;
		}
		for (; currentValue->right != nullptr; currentValue = currentValue->right);
	}
	else
		if (currentValue->parent->value != currentValue->value)
			currentValue = currentValue->parent;
		else {
			for (; currentValue->parent->value == currentValue->value; currentValue = currentValue->parent);
			currentValue = currentValue->parent;
			while (currentValue->down != nullptr) {
				currentValue = currentValue->down;
				for (; currentValue->right != nullptr; currentValue = currentValue->right);
			}
		}

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