#include <iostream>
#include "Screen.h"

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

void screen_refresh() // Обновление экрана
{
    for (int y = YMAX - 1; 0 <= y; --y) { // с верхней строки до нижней
        for (auto x : screen[y])    // от левого столбца до правого
            std::cout << x;
        std::cout << '\n';
    }
}

void screen_clear() { screen_init(); } //Очистка экрана

