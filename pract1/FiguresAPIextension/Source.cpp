#include "Shape.h"
#include "Screen.h"
#include "Rectangle.h"
#include "Line.h"
#include "Trapezium.h"

void up(shape& p, const shape& q) // ��������� p ��� q
{	
	point n = q.north();
	point s = p.south();
	p.move(n.x - s.x, n.y - s.y + 1);
}


int main() {
	setlocale(LC_ALL, "Rus");
	screen_init();

	auto* fig = new trapezium(point(20, 10), point(80, 30));
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
	screen_refresh();

	/* 
	//== 1.���������� ������ ����� ==
	rectangle hat(point(0, 0), point(14, 5));
	line brim(point(0,15),17);
	myshape face(point(15,10), point(27,18));
	h_circle beard(point(40,10), point(50,20));
	shape_refresh( );
	std::cout << "=== Generated... ===\n";
	std::cin.get(); //�������� �������� �����

	//== 2.���������� � ������ ==
	hat.rotate_right( );
	brim.resize(2.0);
	face.resize(2.0);
	beard.flip_vertically();
    shape_refresh( );
	std::cout << "=== Prepared... ===\n";
	std::cin.get(); //�������� ��������� ���������/���������

	//== 3.������ ����������� ==
	//face.move(0, -10); // ���� - � �������� ���������
	up(brim, face);
	up(hat, brim);
	down(beard, face);
	shape_refresh( );
	std::cout << "=== Ready! ===\n";
	std::cin.get(); //�������� ���������
	screen_destroy( );
	*/

	delete fig;
	return 0;
}