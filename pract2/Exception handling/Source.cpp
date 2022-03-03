#include "Shape.h"
#include "Screen.h"
#include "Rectangle.h"
#include "Line.h"
#include "Trapezium.h"
#include "OutOfScreen.h"

using std::to_string;

template<class Figure>
Figure* SafeMakeFigure(point sw, point ne) { // or "w" and "e" since this func is appliable to lines too
	if (sw.x >= ne.x || sw.y >= ne.y)
		throw OutOfScreen("Unsuitable coordinates.");
	if (sw.x < 0 || sw.x >= XMAX || sw.y < 0 || sw.y >= YMAX)
		throw OutOfScreen("First point argument (" + to_string(sw.x) + "; " + to_string(sw.y) + ") is out of screen.");
	if (ne.x < 0 || ne.x >= XMAX || ne.y < 0 || ne.y >= YMAX)
		throw OutOfScreen("Second point argument (" + to_string(ne.x) + "; " + to_string(ne.y) + ") is out of screen.");
	
	return new Figure(sw, ne);
}

void SafeMove(shape& fig, int a, int b) {
	point sw = fig.swest();
	point ne = fig.neast();

	if (sw.x + a < 0 || sw.y + b < 0 || ne.x + a >= XMAX || ne.y + b >= YMAX)
		throw OutOfScreen("One of points would have been out of screen after moving.");

	fig.move(a, b);
}

int main() {
	trapezium* fig = nullptr;

	screen_init();

	try {
		fig = SafeMakeFigure<trapezium>(point(0, 0), point(40, 40));
	}
	catch (OutOfScreen& e) {
		std::cout << e.what() << std::endl;
	}
	shape_refresh();
	std::cout << "\n\nI reached this line!\n\n";

	fig->resize(8);
	shape_refresh();
	std::cout << "\n\n";

	fig->resize(1.2);
	shape_refresh();
	std::cout << "\n\n";

	fig->rotate_right();
	shape_refresh();
	std::cout << "\n\n";

	try {
		SafeMove(*fig, 5, 10);
	}
	catch (OutOfScreen& e) {
		std::cout << e.what() << std::endl;
	}
	shape_refresh();
	std::cout << "\n\nI reached this line too!\n\n";

	delete fig;
	return 0;
}
