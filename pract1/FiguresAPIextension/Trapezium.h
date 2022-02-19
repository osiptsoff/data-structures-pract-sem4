#pragma once

#include "Rectangle.h"

class trapezium : public rectangle, public reflectable /*public rotatable*/ {
    /*      nw ---- n ---- ne
           |                 |
          |                   |
         w          c          e
        |                       |
       |                         |
       sw --------- s --------- se */
protected:
    bool reflected;
public:
    trapezium(point a, point b, bool r = true) : rectangle::rectangle(a, b), reflected(r) {}
    inline point north() const { return rectangle::north(); }
    inline point south() const { return rectangle::south(); }
    inline point east() const { return point(rectangle::east().x + (ne.x - sw.x) / 8, rectangle::east().y); }
    inline point west() const { return point(rectangle::west().x + (ne.x - sw.x) / 8, rectangle::west().y); }
    inline point neast() const { return ne; }
    inline point seast() const { return point(rectangle::seast().x + (ne.x - sw.x) / 4, rectangle::seast().y); }
    inline point nwest() const { return point(rectangle::nwest().x + (ne.x - sw.x) / 4, rectangle::nwest().y); }
    inline point swest() const { return sw; }

   /*void rotate_right() // ѕоворот вправо относительно se
    {
        int w = ne.x - sw.x, h = ne.y - sw.y; //(учитываетс€ масштаб по ос€м)
        point c(sw.x + w/2, sw.y + h/2);
        sw.x = ne.x - h * 2;
        ne.y = sw.y + w / 2;
    }

    void rotate_left() // ѕоворот влево относительно sw
    {
        int w = ne.x - sw.x, h = ne.y - sw.y;
        ne.x = sw.x + h * 2;
        ne.y = sw.y + w / 2;
    }
    */
   
    void flip_horisontally() { reflected = !reflected; } //отражение по горизонтали

    void flip_vertically() { }; //отражение по вертикали ничего не мен€ет

    void draw()
    {
        if (reflected) { rectangle::draw(); }
        else
        {
            int h = ne.y - sw.y;
            point _sw(sw.x, sw.y + h);
            point _nw(nwest().x, sw.y);
            point _ne(neast().x, sw.y);
            point _se(seast().x, sw.y + h);
            put_line(_sw, _nw);
            put_line(_nw, _ne);
            put_line(_ne, _se);
            put_line(_se, _sw);
        }
        put_line(sw.x, sw.y, seast().x, ne.y);
        put_line(sw.x, ne.y, seast().x, seast().y);
    }
};