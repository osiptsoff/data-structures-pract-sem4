#pragma once

#include <vector>
#include <queue>
#include <algorithm>

#include "Node.h"
#include "AccessIterator.h"

using std::queue;
using std::vector;

class Tree {
protected:
	Node* root;
	std::vector<int> sequence;

	AccessIterator insert(AccessIterator, int);
public:
	Tree() : root(nullptr) { sequence = vector<int>(); };
	Tree(const vector<int>);

	Tree(const Tree&);
	Tree(Tree&& other) noexcept : root(other.root) { other.root = nullptr; sequence = other.sequence; };

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
	Tree subst(Tree&, int);

	void remove(AccessIterator);
	void remove(int);

	Tree excl(Tree&);
	Tree erase(int, int);

	bool contains(const std::vector<int>&);
	bool contains(int what);

	std::ostream& printSequence(std::ostream& stream) {
		for (auto elem : sequence)
			stream << elem << " ";

		return stream;
	}

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
			if (elem->value == rootValue) std::cout << '\n';

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
			if (elem->right->right != nullptr && elem->right->right->down != nullptr)
				que.push(elem->right->right->down);
		}
		return stream;
	}
};

// Принимая на вход остортированный(!) вектор, эта функция строит по нему 2-3 дерево
// этот алгоритм подробно проиллюстрирован в лекции
Tree::Tree(vector<int> sequence) : root(nullptr) {
	if (sequence.empty())
		return;

	this->sequence = sequence;
	std::sort(sequence.begin(), sequence.end());

	root = new Node(nullptr, sequence[0]);

	Node* runner = root;
	Node* upperLayerRunnerPrev = nullptr;
	Node* upperLayerRunner;

	// Создадим первый уровень: создадим для каждого элемента вектора ноду и соединим все ноды слева направо,
	// корень - самая левая нода
	for (int size = sequence.size(), i = 1; i < size; ++i) {
		runner->right = new Node(runner, sequence[i]);
		runner = runner->right;
	}

	// пока справа от корня есть больше двух нод
	while (root->right != nullptr && root->right->right != nullptr && root->right->right->right != nullptr) {
		runner = root;
		// создадим очередной уновень выше текущего, его началом сделаем корень
		root = new Node(nullptr, root->value);
		root->down = runner;
		root->down->parent = root;

		upperLayerRunner = root;

		int nodesWatched = 1;

		//создав новый уровень, проходим по предыдущему: берём все тройки нод, отделяем их от оставшихся, создаём для
		// них управляюзую ноду и добавляем её слева к последей ноде верхнего уровня 
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

		// если последняя нода предыдущего уровня осталась одна, то выберем ей пару, отсоеденив 
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
	std::vector<int> vector = other.sequence;

	Tree tmp = Tree(vector);
	root = tmp.root;
	sequence = tmp.sequence;
	tmp.root = nullptr;
}

Tree& Tree::operator = (const Tree& other) {
	if (this != &other) {
		std::vector<int> vector = other.sequence;
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
		sequence = other.sequence;
		other.root = nullptr;
	}
	return *this;
}

AccessIterator Tree::insert(AccessIterator where, int value) {
	Node* nodeStart = where.currentValue;
	Node* runner;
	int n = 1;

	// Если where указывает на nullptr (т.е. дерево пустое или where указывает на конец данных)
	if (where.currentValue == nullptr)
		// Если дерево пустое
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

	// Добавить вставляемый на место where, старое содержимое будет сразу после
	runner = where.currentValue;
	Node* oldRight = runner->right;

	runner->right = new Node(runner, runner->value);
	runner->right->right = oldRight;
	if (oldRight != nullptr) oldRight->parent = runner->right;

	for (; runner->parent != nullptr && runner->value == runner->parent->value; runner = runner->parent) runner->value = value;
	runner->value = value;

	// Посчитаем, сколько чисел в узле
	while (nodeStart->parent != nullptr && nodeStart->value != nodeStart->parent->value) nodeStart = nodeStart->parent;
	for (runner = nodeStart; runner->right != nullptr; runner = runner->right) ++n;

	// Если изначально в узле хранилось 3 числа, то сейчас их 4. Нужно перестроить дерево
	while (n > 3) {
		// если мы вставляем число в корневой узел, дерево нужно углубить
		if (nodeStart->parent == nullptr) {
			Node* underRoot = new Node(nodeStart, nodeStart->value);
			underRoot->down = nodeStart->down;
			if (nodeStart->down != nullptr) nodeStart->down->parent = underRoot;
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

		// подсчитаем количество чисел в очередном узле, чтобы понять, нужна ли дальнейшая перестройка
		while (nodeStart->parent != nullptr && nodeStart->value != nodeStart->parent->value) nodeStart = nodeStart->parent;
		for (runner = nodeStart, n = 1; runner->right != nullptr; runner = runner->right) n++;
	}

	// Вернём новый итератор, указывающий на ноду из where, так как для навигации итератор хранит в себе
	// структуру листьев, а она после вставки может быть изменена
	sequence.push_back(value);
	return where;
}

// При переписывании дерева в дженерик потребуется передавать компаратор
AccessIterator Tree::insert(int value) {
	if (root == nullptr)
		return insert(end(), value);

	auto runner = root;
	while (runner->down != nullptr) {
		for (; runner->right != nullptr && runner->right->value <= value; runner = runner->right);
		runner = runner->down;
	}
	for (; runner->right != nullptr && runner->value < value; runner = runner->right);

	if (runner->value < value)
		return insert(++AccessIterator(root, runner), value);
	else if (runner->value != value)
		return insert(AccessIterator(root, runner), value);
}

void Tree::remove(AccessIterator where) {
	Node* removable = where.currentValue;
	if (removable == nullptr) return;

	int i = 0;
	int size = sequence.size();
	auto iter = sequence.begin();
	for (; sequence[i] != removable->value && i < size; ++i, ++iter);
	if (i != size) sequence.erase(iter);

	if (removable->parent == nullptr) {
		if (root->right) {
			root = removable->right;
			root->parent = nullptr;
			removable->right = nullptr;
			delete removable;
		}
		else root = nullptr;
	}
	else if (removable->parent == root && root->down == nullptr) {
		root->value = removable->value;
		root->right = nullptr;
		delete removable;
	}
	else {
		Node* runner = removable;
		int n = 1;
		bool flag;

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
		if (removable->right) removable->right = nullptr;
		delete removable;

		// Перестройка дерева, если необходимо
		if (runner->down && runner->value == runner->down->value) runner = runner->down;
		else if (runner->parent != nullptr && runner->parent->down == nullptr && runner->parent->parent && runner->parent->value == runner->parent->parent->value) runner = runner->parent;
		else if (runner->parent == root && root->down == nullptr) runner = runner->parent;

		n = 1;
		for (; runner->right != nullptr; runner = runner->right) ++n; // Считаем, сколько чисел в узле
		if (n == 1) { // остался один лист
			removable = runner;
			std::string str = "1"; // Переменная для перестройки родителей по необходимости
			if (removable->parent && removable->parent->right) { // Go on right nodes
				flag = true;
				runner = removable->parent->right->down; // Переход к рассмотрению правых 2/3 листов
				for (; runner->right != nullptr; runner = runner->right) ++n;
				if (n == 2) { // For one son next two nodes
					Node* deleted = removable->parent->right;
					runner = runner->parent;
					removable->right = runner;
					runner->parent = removable;

					if (deleted->right != nullptr) {
						deleted->right->parent = deleted->parent;
						deleted->parent->right = deleted->right;
						deleted->right = nullptr;
					}
					else removable->parent->right = nullptr;
					deleted->down = nullptr;
					delete deleted;

					if (!runner->parent->parent->right) { // В связке остался один родитель
						n = 1;
						if (!runner->parent->parent->parent) { // В связке остался корень и нижние узлы
							runner->parent = root;
							root->right = runner;
							removable->right = nullptr;
							removable->parent->down = nullptr;
							delete removable;
							n = -1;
						}
						else if (runner->parent->parent->value == runner->parent->parent->parent->value) {
							if (removable->parent->parent->right) { // Переход к рассмотрению правых 2/3 родителей
								runner = removable->parent->parent->right->down;
								for (; runner->right != nullptr; runner = runner->right) ++n;
								while (runner->value != runner->parent->value) runner = runner->parent;
								str = (n == 2) ? "r2" : "r3";
							}
							else { // Переход к рассмотрению левых 2/3 родителям
								runner = removable->parent->parent->parent->down;
								for (; runner->right != nullptr; runner = runner->right) ++n;
								str = (n == 2) ? "l2" : "l3";
							}
						}
						else n = -1;
					}
					else n = -1;
				}
				else { // Следующая связка из трех листьев
					runner = runner->parent->parent;
					runner->parent->value = runner->right->value;
					runner->parent->down = runner->right;
					runner->right->parent = runner->parent;
					removable->right = runner;
					runner->parent = removable;
					runner->right = nullptr;
					n = -1;
				}
			}
			else { //Go on left nodes
				flag = false;
				runner = removable->parent->parent->down; // Переход к рассмотрению левых 2/3 сыновей
				for (; runner->right != nullptr; runner = runner->right) ++n;
				if (n == 2) { //For one son before two nodes
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

					if (!runner->parent->parent) { // Root and sons
						root->right = runner->right;
						runner->right->parent = root;
						root->down = nullptr;
						runner->right = nullptr;
						delete runner;
						n = -1;
					}
					else if (!runner->parent->right && runner->parent->value == runner->parent->parent->value) { // В связке остался один родитель
						n = 1;
						removable = runner;
						if (runner->parent->parent->right) { // Переход к рассмотрению правых 2/3 родителей
							runner = runner->parent->parent->right->down;
							for (; runner->right != nullptr; runner = runner->right) ++n;
							while (runner->value != runner->parent->value) runner = runner->parent;
							str = (n == 2) ? "r2" : "r3";
						}
						else { // Переход к рассмотрению левых 2/3 родителям
							runner = runner->parent->parent->parent->down;
							for (; runner->right != nullptr; runner = runner->right) ++n;
							str = (n == 2) ? "l2" : "l3";
						}
					}
					else n = -1;
				}
				else { // For one son before three nodes
					removable->parent->value = runner->value;
					runner->parent->right = nullptr;
					runner->parent = removable->parent;
					removable->parent->down = runner;
					runner->right = removable;
					removable->parent = runner;
					n = -1;
				}
			}

			do {
				Node* up_node = nullptr;
				// Перестройка по родителям
				if (str == "r2") { //For one parent next two parent
					Node* deleted = runner->parent;
					removable->parent->parent->right = runner->parent->right;
					if (runner->parent->right) runner->parent->right->parent = removable->parent->parent;
					runner->parent->down = nullptr;
					removable->parent->right = runner;
					runner->parent = removable->parent;
					deleted->right = nullptr;
					deleted->down = nullptr;
					delete deleted;
					n = 0;

					if (removable->parent->parent == root) {
						Node* delNode = removable->parent;
						removable->parent = root;
						runner->parent = root;
						removable->parent->down = removable;
						root->right = runner;
						delNode->right = nullptr;
						delNode->down = nullptr;
						delete delNode;
						n = -1;
					}
					if (runner->parent->parent && !runner->parent->parent->right) flag = true;
				}
				else if (str == "r3") { // For one parent next three parent
					runner->parent->value = runner->right->value;
					runner->parent->down = runner->right;
					runner->right->parent = runner->parent;
					runner->right = nullptr;
					runner->parent = removable->parent;
					removable->parent->right = runner;
					n = -1;
				}
				else if (str == "l2") { // For one parent before two parent
					Node* deleted = runner->parent->parent->right;
					removable->parent->parent = runner;
					runner->right = removable->parent;
					runner->parent->parent->right = nullptr;
					if (deleted) {
						deleted->right = nullptr;
						deleted->down = nullptr;
					}
					delete deleted;
					n = 0;

					if (runner->parent->parent == root) {
						Node* delNode = runner->parent;
						runner->parent = delNode->parent;
						delNode->down->parent = runner->parent;
						runner->parent->down = delNode->down;
						delNode->parent->right = runner;
						delNode->right = nullptr;
						delNode->down = nullptr;
						delete delNode;
						n = -1;
					}
					if (runner->parent->parent && !runner->parent->parent->right) flag = true;
				}
				else if (str == "l3") { // For one parent before three parent
					removable->parent->parent->value = runner->value;
					runner->parent->right = nullptr;
					removable->parent->parent->down = runner;
					runner->parent = removable->parent->parent;
					runner->right = removable->parent;
					removable->parent->parent = runner;
					n = -1;
				}

				int lvl = 1;
				for (Node* new_node = root; new_node && new_node->down; new_node = new_node->down) lvl++;
				if (n != -1 && lvl > 3) {
					if (flag) {
						if (str == "r2") up_node = removable->parent->parent;
						else if (str == "l2") up_node = runner->parent->parent;
					}
					else {
						if (str == "r2") up_node = runner->parent->parent;
						else if (str == "l2") up_node = removable->parent->parent;
					}
					n = 1;
					while (up_node && up_node->parent && up_node->parent->parent && up_node->value != up_node->parent->value) up_node = up_node->parent;
					for (; up_node && up_node->right; up_node = up_node->right) ++n;

					if (n == 1) {
						if (flag) removable = up_node->parent->down;
						else removable = (str == "r2") ? up_node->parent->down : up_node->parent->parent;

						if (removable->parent && removable->parent->right) {
							if (!runner->parent->parent->right) { // В связке остался один родитель
								n = 1;
								runner = removable->parent->right->down;
								if (removable->right) for (Node* new_node = removable; new_node->right != nullptr; new_node = new_node->right) ++n;
								else for (Node* new_node = runner; new_node->right != nullptr; new_node = new_node->right) ++n;

								if (!removable->parent->parent) { // В связке остался корень и нижние узлы
									if (n == 2) {
										Node* deleted = runner->parent;
										if (removable->right) {
											removable->right->right = runner;
											runner->parent = removable->right;
										}
										else {
											removable->right = runner;
											runner->parent = removable;
										}

										if (deleted->right != nullptr) {
											removable->parent->right = deleted->right;
											deleted->right->parent = removable->parent;
											deleted->right = nullptr;
										}
										else removable->parent->right = nullptr;
										deleted->down = nullptr;
										delete deleted;
									}
									else {
										runner->parent->value = runner->right->value;
										removable->right = runner;
										runner->parent->down = runner->right;
										runner->right->parent = runner->parent;
										runner->parent = removable;
										runner->right = nullptr;
										n = -1;
									}

									if (n != -1) {
										if (removable->right->value == runner->value) {
											runner->parent = root;
											root->right = runner;
										}
										else {
											runner->parent->parent = root;
											root->right = runner->parent;
										}

										if (root->down->down) {
											root->down = removable->down;
											removable->down->parent = root;
										}
										else root->down = nullptr;
										removable->right = nullptr;
										removable->down = nullptr;
										delete removable;
										n = -1;
									}

								}
								else if (runner->parent->parent->value == runner->parent->parent->parent->value) {
									removable = removable->down;
									n = 1;

									if (removable->parent->parent && removable->parent->parent->right) { // Переход к рассмотрению правых 2/3 родителей
										runner = removable->parent->parent->right->down;
										for (; runner->right != nullptr; runner = runner->right) ++n;
										while (runner->value != runner->parent->value) runner = runner->parent;
										str = (n == 2) ? "r2" : "r3";
									}
									else { // Переход к рассмотрению левых 2/3 родителям
										runner = removable->parent->parent->parent->down;
										for (; runner->right != nullptr; runner = runner->right) ++n;
										str = (n == 2) ? "l2" : "l3";
									}
									n = 1;
								}
								else n = -1;
							}
							else n = -1;
							flag = true;
						}
						else {
							if (flag) {
								if (removable->down->right->value == runner->value) {
									runner = removable->parent->parent->down;
									removable = removable->down;
								}
								else {
									removable = removable->parent;
									runner = removable->parent->parent->right->down;
								}
							}
							else {
								if (removable->parent->parent->parent) {
									if (removable->parent->parent->parent->value == removable->value) {
										runner = removable->parent;
										removable = runner->parent->right->down->down;
									}
									else runner = removable->parent->parent->parent->down;
								}
								else {
									runner = removable->parent;
									removable = runner->parent->right->down->down;
								}
							}

							if (!runner->parent->parent) {
								n = 1;
								if (runner->right) for (Node* new_node = runner; new_node->right != nullptr; new_node = new_node->right) ++n;
								else for (Node* new_node = removable->parent; new_node->right != nullptr; new_node = new_node->right) ++n;

								if (!runner->parent->parent) {
									if (n == 2) {
										Node* deleted = removable->parent->parent;
										if (runner->right) {
											runner->right->right = removable->parent;
											removable->parent->parent = runner->right;
										}
										else {
											runner->right = removable->parent;
											removable->parent->parent = runner;
										}

										if (deleted->right != nullptr) {
											runner->parent->right = deleted->right;
											deleted->right->parent = runner->parent;
											deleted->right = nullptr;
										}
										else runner->parent->right = nullptr;
										deleted->down = nullptr;
										delete deleted;
									}
									else {
										if (runner->right) {
											removable->parent->parent->value = runner->right->right->value;
											runner->right->right->parent = removable->parent->parent;
											removable->parent->parent->down = runner->right->right;
											runner->right->right->right = removable->parent;
											removable->parent->parent = runner->right->right;
											runner->right->right = nullptr;
										}
										n = -1;
									}

									if (n != -1) {
										root->right = runner->right;
										runner->right->parent = root;

										if (root->down->down) {
											root->down = runner->down;
											runner->down->parent = root;
										}
										else root->down = nullptr;
										runner->right = nullptr;
										runner->down = nullptr;
										delete runner;
										n = -1;
									}
								}
								else if (!runner->parent->right && runner->parent->value == runner->parent->parent->value) { // В связке остался один родитель
									n = 1;
									removable = runner;
									if (runner->parent->parent->right) { // Переход к рассмотрению правых 2/3 родителей
										runner = runner->parent->parent->right->down;
										for (; runner->right != nullptr; runner = runner->right) ++n;
										while (runner->value != runner->parent->value) runner = runner->parent;
										str = (n == 2) ? "r2" : "r3";
									}
									else { // Переход к рассмотрению левых 2/3 родителям
										runner = runner->parent->parent->parent->down;
										for (; runner->right != nullptr; runner = runner->right) ++n;
										str = (n == 2) ? "l2" : "l3";
									}
									n = 1;
								}
								else n = -1;

							}
							else if (runner->right && runner->right->right) {
								if (removable->parent->down->value == runner->value) removable = removable->down->down;
								runner = runner->right->right;
								str = "l3";
							}
							else if (runner->right) {
								if (removable->parent->down->value == runner->value) removable = removable->down->down;
								runner = runner->right;
								if (removable->parent->parent->value == removable->parent->value) str = "l2";
								else n = -1;
							}
							else if (!runner->right) {
								n = 1;
								for (Node* up_node = removable->parent; up_node && up_node->right; up_node = up_node->right) ++n;
								str = (n == 2) ? "r2" : "r3";
								removable = runner->down;
								runner = runner->parent->right->down;
								n = 1;
							}
							flag = false;
						}
					}
				}
			} while (n == 1);
		}
	}
}

void Tree::remove(int value) {
	if (root != nullptr) {
		auto runner = root;
		while (runner->down != nullptr) {
			for (; runner->right != nullptr && runner->right->value <= value; runner = runner->right);
			runner = runner->down;
		}
		for (; runner->right != nullptr && runner->value < value; runner = runner->right);

		if (runner->value == value) remove(AccessIterator(root, runner));
	}
}

Tree Tree::excl(Tree& other) {
	if (contains(other.sequence)) {
		Tree result = Tree(*this);
		for (auto i : other.sequence)
			result.remove(i);
		return std::move(result);
	}
	else return std::move(*this);

}

Tree Tree::erase(int from, int to) {
	if (from < 0 || to < from || sequence.size() < to)
		throw std::out_of_range("Incorrect bounds");

	Tree result = Tree(*this);

	for (; from != to;)
		result.remove(sequence[from++]);


	return result;
}

Tree Tree::subst(Tree& other, int fromPos) {
	std::vector<int> resSequence = std::vector<int>();

	try {
		begin() + fromPos;
	}
	catch (std::out_of_range e) {
		throw e;
	}

	int i = 0;
	for (; i < fromPos; ++i)
		resSequence.push_back(sequence[i]);

	for (auto i : other.sequence)
		resSequence.push_back(i);

	for (auto size = sequence.size(); i < size; ++i)
		resSequence.push_back(sequence[i]);

	return std::move(Tree(resSequence));
}

bool Tree::contains(const std::vector<int>& subseq) {
	int thisSize = sequence.size();
	int otherSize = subseq.size();
	int ctr = 1;

	if (subseq.empty()) return true;
	if (sequence.empty()) return false;

	int runner = 0;

	for (; runner < thisSize && sequence[runner] != subseq[0];) {
		if (sequence[runner] == subseq[0])
			--runner;
		++runner;
	}

	for (int i = 1; i < otherSize && runner < thisSize; ++i)
		if (sequence[++runner] != subseq[i])
			return false;
		else ctr++;

	if (ctr != otherSize)
		return false;

	return true;

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