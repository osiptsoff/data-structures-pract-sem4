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
public:
    trapezium(point a, point b) : rectangle::rectangle(a, b) {}
    point north() const { return rectangle::north(); }
    point south() const { return rectangle::south(); }
    point east() const { return point(rectangle::east().x + 8, rectangle::east().y); }
    point west() const { return point(rectangle::west().x + 8, rectangle::west().y); }
    point neast() const { return point(ne.x + 16, ne.y); }
    point seast() const { return point(rectangle::seast().x + 16, rectangle::seast().y); }
    point nwest() const { return point(rectangle::nwest().x + 16, rectangle::nwest().y); }
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
   
    void draw()
    {
        rectangle::draw();
        put_line(sw.x, sw.y, seast().x, ne.y);
        put_line(sw.x, ne.y, seast().x, seast().y);
    }
};