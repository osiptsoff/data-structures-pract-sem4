#pragma once
#include "Shape.h"
#include "Trapezium.h"
#include "Screen.h"
#include "Rectangle.h"
#include "Line.h"


// Cборна€ пользовательска€ фигура - физиономи€
class myshape : public rectangle { // ћо€ фигура я¬Ћя≈“—я
	int w, h;			             //        пр€моугольником
	line l_eye; // левый глаз Ц мо€ фигура —ќƒ≈–∆»“ линию
	line r_eye; // правый глаз
	line mouth; // рот
public:
	myshape(point, point);
	void draw();
	void move(int, int);
	void resize(double);
};

myshape::myshape(point a, point b)
	: rectangle(a, b),	//»нициализаци€ базового класса
	w(neast().x - swest().x + 1), // »нициализаци€ данных
	h(neast().y - swest().y + 1), // - строго в пор€дке объ€влени€!
	l_eye(point(sw.x + (ne.x - sw.x) / 8, sw.y + (ne.y - sw.y) * 3 / 4), point(sw.x + (ne.x - sw.x) * 3 / 8, sw.y + (ne.y - sw.y) * 3 / 4)),
	r_eye(point(sw.x + (ne.x - sw.x) * 5 / 8, sw.y + (ne.y - sw.y) * 3 / 4), point(sw.x + (ne.x - sw.x) * 7 / 8, sw.y + (ne.y - sw.y) * 3 / 4)),
	mouth(point(sw.x + (ne.x - sw.x) / 4, sw.y + (ne.y - sw.y) / 4), point(sw.x + (ne.x - sw.x) * 3 / 4, sw.y + (ne.y - sw.y) / 4))
{ }

void myshape::draw()
{
	rectangle::draw(); // онтур лица (глаза и нос рисуютс€ сами!) 
	int a = (swest().x + neast().x) / 2;
	int b = (swest().y + neast().y) / 2;
	put_point(point(a, b)); // Ќос Ц существует только на рисунке!
}

void myshape::move(int a, int b)
{
	rectangle::move(a, b);
	l_eye.move(a, b);  r_eye.move(a, b);
	mouth.move(a, b);
}

void myshape::resize(double d) {
	//l_eye.resize(d);
	//r_eye.resize(d);
	//mouth.resize(d);
	rectangle::resize(d);
	l_eye = line(point(sw.x + (ne.x - sw.x) / 8, sw.y + (ne.y - sw.y) * 3 / 4), point(sw.x + (ne.x - sw.x) * 3 / 8, sw.y + (ne.y - sw.y)*3 / 4));
	r_eye = line(point(sw.x + (ne.x - sw.x)*5 / 8, sw.y + (ne.y - sw.y) * 3 / 4), point(sw.x + (ne.x - sw.x) * 7 / 8, sw.y + (ne.y - sw.y) * 3 / 4));
	mouth = line(point(sw.x + (ne.x - sw.x)/4, sw.y + (ne.y - sw.y)/4), point(sw.x + (ne.x - sw.x)*3 / 4, sw.y + (ne.y - sw.y) / 4));
	//mouth.move(d, 0)
}