#include "Shape.h"
#include "Screen.h"
#include "Rectangle.h"
#include "Line.h"
#include "Trapezium.h"

void up(shape& p, const shape& q) // поместить p над q
{	
	point n = q.north();
	point s = p.south();
	p.move(n.x - s.x, n.y - s.y + 1);
}


int main() {
	screen_init();

	auto* fig = new trapezium(point(20, 10), point(100, 30));
	fig->draw();
	screen_refresh();
	delete fig;

	return 0;
}