#include "Shape.h"
#include "Screen.h"
#include "Rectangle.h"
#include "Line.h"

void up(shape& p, const shape& q) // поместить p над q
{	
	point n = q.north();
	point s = p.south();
	p.move(n.x - s.x, n.y - s.y + 1);
}


int main() {
	screen_init();

	auto* fig = new rectangle(point(1, 1), point(40, 40));
	fig->draw();
	screen_refresh();
	delete fig;

	return 0;
}