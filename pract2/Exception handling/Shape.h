#pragma once

#include "Screen.h"
#include "OutOfScreen.h"
#include <list>
#include <iostream>

char screen[YMAX][XMAX];
enum color { black = '*', white = '.' };

void screen_init()
{
	for (auto y = 0; y < YMAX; ++y)
		for (auto& x : screen[y])  x = white;
}

void screen_destroy()
{
	for (auto y = 0; y < YMAX; ++y)
		for (auto& x : screen[y])  x = black;
}

bool on_screen(int a, int b) // проверка попадания точки на экран
{
	return 0 <= a && a < XMAX && 0 <= b && b < YMAX;
}

void put_point(int a, int b)
{
	if (on_screen(a, b)) screen[b][a] = black;
}

void put_line(int x0, int y0, int x1, int y1)
/* Алгоритм Брезенхэма для прямой:
рисование отрезка прямой от (x0,y0) до (x1,y1).
Уравнение прямой: b(x-x0) + a(y-y0) = 0.
Минимизируется величина abs(eps), где eps = 2*(b(x-x0)) + a(y-y0).  */
{
	if (!(on_screen(x0, y0) && on_screen(x1, y1)))
		throw OutOfScreen("\nCannot draw the line: one of points is out of screen.\n");

	int dx = 1;
	int a = x1 - x0;   if (a < 0) dx = -1, a = -a;
	int dy = 1;
	int b = y1 - y0;   if (b < 0) dy = -1, b = -b;
	int two_a = 2 * a;
	int two_b = 2 * b;
	int xcrit = -b + two_a;
	int eps = 0;
	for (;;) { //Формирование прямой линии по точкам
		put_point(x0, y0);
		if (x0 == x1 && y0 == y1) break;
		if (eps <= xcrit) x0 += dx, eps += two_b;
		if (eps >= a || a < b) y0 += dy, eps -= two_a;
	}
}

void screen_clear() { screen_init(); } //Очистка экрана

void screen_refresh() // Обновление экрана
{
	for (int y = YMAX - 1; 0 <= y; --y) { // с верхней строки до нижней
		for (auto x : screen[y])    // от левого столбца до правого
			std::cout << x;
		std::cout << '\n';
	}
}

/// <summary>
/// Объявление абстрактных классов
/// </summary>

struct shape { // Виртуальный базовый класс "фигура"
	static std::list<shape*> shapes;// Список фигур (один на все фигуры!)
	shape() { shapes.push_back(this); } //Фигура присоединяется к списку
	virtual point north() const = 0;	//Точки для привязки
	virtual point south() const = 0;
	virtual point east() const = 0;
	virtual point west() const = 0;
	virtual point neast() const = 0;
	virtual point seast() const = 0;
	virtual point nwest() const = 0;
	virtual point swest() const = 0;
	virtual void draw() = 0;		//Рисование
	virtual void move(int, int) = 0;	//Перемещение
	virtual void resize(double) = 0;    	//Изменение размера
	virtual ~shape() { shapes.remove(this); } //Деструктор
};

std::list<shape*> shape::shapes;   // Размещение списка фигур

void shape_refresh() // Перерисовка всех фигур на экране
{
	screen_clear();
	for (auto p : shape::shapes) p->draw(); //Динамическое связывание!!!
	screen_refresh();
}

class rotatable : virtual public shape { //Фигуры, пригодные к повороту 
public:
	virtual void rotate_left() = 0;	//Повернуть влево
	virtual void rotate_right() = 0;	//Повернуть вправо
};

class reflectable : virtual public shape { // Фигуры, пригодные
public:					     // к зеркальному отражению
	virtual void flip_horisontally() = 0;	// Отразить горизонтально
	virtual void flip_vertically() = 0;	          // Отразить вертикально
};

