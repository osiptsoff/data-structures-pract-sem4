#pragma once

#include "Rectangle.h"

class trapezium : public rectangle/*public rotatable, public reflectable*/ {
    /*      nw ---- n ---- ne
           |                 |
          |                   |
         w          c          e
        |                       |
       |                         |
       sw --------- s --------- se */
protected:
    point sw, ne;
public:
    trapezium(point a, point b) : sw(a), ne(b) { }
    point north() const { return rectangle::north(); }
    point south() const { return rectangle::south(); }
    point east() const { return point(sw.x + (ne.x - sw.x) * 7 / 8, (sw.y + ne.y) / 2); }
    point west() const { return point(sw.x + (ne.x - sw.x) / 8, (sw.y + ne.y) / 2); }
    point neast() const { return point(sw.x + (ne.x - sw.x) * 3 / 4, ne.y); }
    point seast() const { return point(ne.x, sw.y); }
    point nwest() const { return point(sw.x + (ne.x - sw.x) / 4, ne.y); }
    point swest() const { return sw; }

   /*void rotate_right() // Поворот вправо относительно se
    {
        int w = ne.x - sw.x, h = ne.y - sw.y; //(учитывается масштаб по осям)
        sw.x = ne.x - h * 2; ne.y = sw.y + w / 2;
    }

    void rotate_left() // Поворот влево относительно sw
    {
        int w = ne.x - sw.x, h = ne.y - sw.y;
        ne.x = sw.x + h * 2; ne.y = sw.y + w / 2;
    }
    */
    void move(int a, int b)
    {
        sw.x += a; sw.y += b; ne.x += a; ne.y += b;
    }
    /*
    void resize(double d)
    {
        ne.x = sw.x + (ne.x - sw.x) * d; ne.y = sw.y + (ne.y - sw.y) * d;
    }
    */

    void draw()
    {
        
        /*point nw(sw.x, ne.y);
        put_line(nwest(), neast());
        put_line(neast(), seast());
        put_line(seast(), sw);   
        put_line(sw, nwest());
        put_line(sw, ne);
        put_line(nw, seast()); */
        rectangle::draw();
    }
};