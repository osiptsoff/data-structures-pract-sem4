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
	void insert(AccessIterator); // subst

	void remove(AccessIterator);
	void remove(int);
	void remove(AccessIterator, AccessIterator); // excl
	void remove(int, int); // erase

	bool contains(int);

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

// ѕринима€ на вход остортированный(!) вектор, эта функци€ строит по нему 2-3 дерево
// этот алгоритм подробно проиллюстрирован в лекции
Tree::Tree(const vector<int>& sequence) : root(nullptr) {
	if (sequence.empty())
		return;

	root = new Node(nullptr, sequence[0]);

	Node* runner = root;
	Node* upperLayerRunnerPrev = nullptr;
	Node* upperLayerRunner;

	// —оздадим первый уровень: создадим дл€ каждого элемента вектора ноду и соединим все ноды слева направо,
	// корень - сама€ лева€ нода
	for (int size = sequence.size(), i = 1; i < size; ++i) {
		runner->right = new Node(runner, sequence[i]);
		runner = runner->right;
	}

	// пока справа от корн€ есть больше двух нод
	while (root->right != nullptr && root->right->right != nullptr && root->right->right->right != nullptr) {
		runner = root;
		// создадим очередной уновень выше текущего, его началом сделаем корень
		root = new Node(nullptr, root->value);
		root->down = runner;
		root->down->parent = root;

		upperLayerRunner = root;

		int nodesWatched = 1;

		//создав новый уровень, проходим по предыдущему: берЄм все тройки нод, отдел€ем их от оставшихс€, создаЄм дл€
		// них управл€юзую ноду и добавл€ем еЄ слева к последей ноде верхнего уровн€ 
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

		// если последн€€ нода предыдущего уровн€ осталась одна, то выберем ей пару, отсоеденив 
		// третью ноду из предыдущей тройки
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

	// ≈сли where указывает на nullptr (т.е. дерево пустое или where указывает на конец данных)
	if (where.currentValue == nullptr)
		// ≈сли дерево пустое
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

	// ƒобавить вставл€емый на место where, старое содержимое будет сразу после
	runner = where.currentValue;
	Node* oldRight = runner->right;

	runner->right = new Node(runner, runner->value);
	runner->right->right = oldRight;
	if (oldRight != nullptr) oldRight->parent = runner->right;

	for (; runner->parent != nullptr && runner->value == runner->parent->value; runner = runner->parent) runner->value = value;
	runner->value = value;

	// ѕосчитаем, сколько чисел в узле
	while (nodeStart->parent != nullptr && nodeStart->value != nodeStart->parent->value) nodeStart = nodeStart->parent;
	for (runner = nodeStart; runner->right != nullptr; runner = runner->right) ++n;

	// ≈сли изначально в узле хранилось 3 числа, то сейчас их 4. Ќужно перестроить дерево
	while (n > 3) {
		// если мы вставл€ем число в корневой узел, дерево нужно углубить
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

		// подсчитаем количество чисел в очередном узле, чтобы пон€ть, нужна ли дальнейша€ перестройка
		while (nodeStart->parent != nullptr && nodeStart->value != nodeStart->parent->value) nodeStart = nodeStart->parent;
		for (runner = nodeStart, n = 1; runner->right != nullptr; runner = runner->right) n++;
	}

	// ¬ернЄм новый итератор, указывающий на ноду из where, так как дл€ навигации итератор хранит в себе
	// структуру листьев, а она после вставки может быть изменена
	return where;
}

// ѕри переписывании дерева в дженерик потребуетс€ передавать компаратор
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

void Tree::remove(AccessIterator where) {

}

void Tree::remove(int what) {

}

void Tree::remove(AccessIterator otherStart, AccessIterator otherEnd) {
	for (auto i = *otherStart; i != *otherEnd; ++i)
		if (contains(i)) return;

	for (auto i = otherStart; i != otherEnd; ++i)
		remove(i);
}

void Tree::remove(int from, int to) {
	if (from < 0 || to - from < 0)
		return;

	try {
		for (auto i = begin() + from, end = begin() + to; i != end; ++i)
			remove(i);
	}
	catch (std::out_of_range e) {
		throw e;
	}
}

bool Tree::contains(int what) {
	auto runner = root;
	while (runner->down != nullptr) {
		for (; runner->value != what && runner->right != nullptr && runner->right->value <= what; runner = runner->right);
		runner = runner->down;
	}
	for (; runner->value != what && runner->right != nullptr && runner->value < what; runner = runner->right);

	return runner->value == what;
}