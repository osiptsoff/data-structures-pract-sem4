#pragma once

#include "Screen.h"
#include "Shape.h"

class line : public shape {
	/* ������� ������ ["w", "e"].
	   north( ) ���������� ����� "���� ������ ������� � ��� ������
	   �� �����, ��� ����� ��� �������� �����", � �. �. */
protected:
	point w, e;
public:
	line(point a, point b) : w(a), e(b) { }; //������������ ����� (�� ���� ������)
	line(point a, int L) : w(point(a.x + L - 1, a.y)), e(a) {  }; //�������������� �����
	point north() const { return point((w.x + e.x) / 2, e.y < w.y ? w.y : e.y); }
	point south() const { return point((w.x + e.x) / 2, e.y < w.y ? e.y : w.y); }
	point east() const { return point(e.x < w.x ? w.x : e.x, (w.y + e.y) / 2); }
	point west() const { return point(e.x < w.x ? e.x : w.x, (w.y + e.y) / 2); }
	point neast() const { return point(w.x < e.x ? e.x : w.x, e.y < w.y ? w.y : e.y); }
	point seast() const { return point(w.x < e.x ? e.x : w.x, e.y < w.y ? e.y : w.y); }
	point nwest() const { return point(w.x < e.x ? w.x : e.x, e.y < w.y ? w.y : e.y); }
	point swest() const { return point(w.x < e.x ? w.x : e.x, e.y < w.y ? e.y : w.y); }
	void move(int a, int b) { w.x += a; w.y += b; e.x += a; e.y += b; }
	void draw() { put_line(w, e); }
	void resize(double d) // ��������� ����� ����� � (d) ���
	{ e.x = w.x + (e.x - w.x) * d; e.y = w.y + (e.y - w.y) * d; }
};
