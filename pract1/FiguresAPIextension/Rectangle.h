#pragma once

#include "Screen.h"
#include "Shape.h"

class rectangle : public rotatable {
    /* nw ---------- n -------- ne
       |                         |
       |                         |
       w             c           e
       |                         |
       |                         |
       sw ---------- s -------- se */
protected:
    point sw, ne;
public:
    rectangle(point a, point b) : sw(a), ne(b) { }
    rectangle() { shapes.push_back(this); }
    point north() const { return point((sw.x + ne.x) / 2, ne.y); }
    point south() const { return point((sw.x + ne.x) / 2, sw.y); }
    point east() const { return point(ne.x, (sw.y + ne.y) / 2); }
    point west() const { return point(sw.x, (sw.y + ne.y) / 2); }
    point neast() const { return ne; }
    point seast() const { return point(ne.x, sw.y); }
    point nwest() const { return point(sw.x, ne.y); }
    point swest() const { return sw; }

    point getSwest () const { return sw; }
    point getNeast() const { return ne; }

    void rotate_right() // ������� ������ ������������ se
    {
        int w = ne.x - sw.x, h = ne.y - sw.y; //(����������� ������� �� ����)
        sw.x = ne.x - h * 2; ne.y = sw.y + w / 2;
    }

    void rotate_left() // ������� ����� ������������ sw
    {
        int w = ne.x - sw.x, h = ne.y - sw.y;
        ne.x = sw.x + h * 2; ne.y = sw.y + w / 2;
    }

    void move(int a, int b)
    {
        sw.x += a; sw.y += b; ne.x += a; ne.y += b;
    }

    void resize(double d) 
    {
        ne.x = sw.x + (ne.x - sw.x) * d; ne.y = sw.y + (ne.y - sw.y) * d;
    }

    void draw()
    {
        put_line(nwest(), ne);   
        put_line(ne, seast());
        put_line(seast(), sw);   
        put_line(sw, nwest());
    }
};