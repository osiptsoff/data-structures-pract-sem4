#include "Shape.h"
#include "Screen.h"
#include "Rectangle.h"
#include "Line.h"
#include "Trapezium.h"
#include "h_circle.h"
#include "myshape.h"


void up(shape& p, const shape& q) // ��������� p ��� q
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
	/*
	auto* fig = new h_circle(point(20, 10), point(80, 30));
	fig->draw();
	screen_refresh();
	
	fig->rotate_right();
	screen_clear();
	fig->draw();
	screen_refresh();

	fig->rotate_right();
	screen_clear();
	fig->draw();
	screen_refresh();

	fig->rotate_right();
	screen_clear();
	fig->draw();
	screen_refresh();

	fig->rotate_right();
	screen_clear();
	fig->draw();
	screen_refresh();*/

	//== 1.���������� ������ ����� ==
	rectangle hat(point(0, 0), point(10, 6));
	line brim(point(0, 25), 17);
	myshape face(point(15, 10), point(27, 18));
	h_circle beard(point(40, 10), point(50, 20));
	trapezium nose(point(40, 20), point(65, 28));
	trapezium l_ear(point(50, 30), point(75, 38));
	trapezium r_ear(point(60, 40), point(85, 48));
	shape_refresh();
	std::cout << "=== Generated... ===\n";
	std::cin.get(); //�������� �������� �����
//== 2.���������� � ������ ==
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
	l_ear.rotate_left(); // ������ ����� ��������
	r_ear.rotate_right();
	shape_refresh();
	std::cout << "=== Prepared... ===\n";
	std::cin.get(); //�������� ��������� ���������/���������
//== 3.������ ����������� ==
	//face.move(0, -10); // ���� - � �������� ���������
	up(brim, face);
	up(hat, brim);
	down(beard, face);
	center(nose, face);
	left(l_ear, face);
	right(r_ear, face);
	shape_refresh();
	std::cout << "=== Ready! ===\n";
	std::cin.get(); //�������� ���������
	screen_destroy();
	return 0;
}
