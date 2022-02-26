#include "Shape.h"
#include "Screen.h"
#include "Rectangle.h"
#include "Line.h"
#include "Trapezium.h"
#include "h_circle.h"
#include "myshape.h"


void up(shape& p, const shape& q) // поместить p над q
{	
	point n = q.north();
	point s = p.south();
	p.move(n.x - s.x, n.y - s.y + 1);
}

void down(shape& p, const shape& q)
{
	point n = q.south();
	point s = p.north();
	p.move(n.x - s.x, n.y - s.y - 1);
}

void center(shape& p, const shape& q) {
	point p_center(p.north().x , p.east().y);
	point q_center(q.north().x, q.east().y);
	p.move(- p_center.x + q_center.x, - p_center.y + q_center.y);
}

void left(shape& p, const shape& q) {
	p.move(-p.east().x + q.west().x, -p.east().y + q.west().y + (q.neast().y - q.east().y) / 3);
}

void right(shape& p, const shape& q) {
	p.move(q.east().x - p.west().x, q.east().y - p.west().y + (q.neast().y - q.east().y) / 3);
}

int main() {
	screen_init();

	//== 1.Объявление набора фигур ==
	rectangle hat(point(15, 34), point(27, 40));
	line brim(point(15, 5), 17);
	myshape face(point(15, 12), point(27, 20));
	h_circle beard(point(80, 10), point(92, 20));
	trapezium nose(point(60, 18), point(85, 26));
	trapezium l_ear(point(60, 29), point(85, 37));
	trapezium r_ear(point(60, 40), point(85, 48));
	shape_refresh();
	std::cout << "=== Generated... ===\n";
	std::cin.get(); //Смотреть исходный набор

//== 2.Подготовка к сборке ==
	hat.rotate_right();
	double d = 2.6;
	brim.resize(d);
	face.resize(d);
	beard.resize(d);
	hat.resize(d);
	nose.resize(0.5);
	l_ear.resize(0.5);
	r_ear.resize(0.5);
	beard.flip_vertically();
	nose.flip_horisontally();
	l_ear.rotate_left(); // размер имеет значение
	r_ear.rotate_right();
	shape_refresh();
	std::cout << "=== Prepared... ===\n";
	std::cin.get(); //Смотреть результат поворотов/отражений

//== 3.Сборка изображения ==
	//face.move(0, -10); // Лицо - в исходное положение
	up(brim, face);
	up(hat, brim);
	down(beard, face);
	center(nose, face);
	left(l_ear, face);
	right(r_ear, face);
	shape_refresh();
	std::cout << "=== Ready! ===\n";
	std::cin.get(); //Смотреть результат
	screen_destroy();
	return 0;
}
