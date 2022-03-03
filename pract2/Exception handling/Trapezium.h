#pragma once

#include "Rectangle.h"

class trapezium : public rectangle, public reflectable {
    /*      --------------
           |              |
          |                |
         |                  |
        |                    |
       |                      |
       ------------------------*/
protected:
    char state;

public:
    trapezium(point a, point b, char st = 0) : rectangle::rectangle(a, b), state(st % 4) {}

    void rotate_right() {
        state = ++state % 4;
        rectangle::rotate_right();
    }

    void rotate_left() {
        state = --state % 4;
        rectangle::rotate_left();
    }
    
    void flip_horisontally() { if( !(state % 2) ) state = (state + 2) % 4; }
    void flip_vertically() { if (state % 2) state = (state + 2) % 4; };

    void draw() {
        point sideStartFir, sideStartSec, sideEndFir, sideEndSec;

        if (state % 2) {
            sideStartFir = state == 1 ? point(ne.x, ne.y - (ne.y - sw.y) / 4) : point(nwest().x, ne.y - (ne.y - sw.y) / 4);
            sideStartSec = state == 1 ? point(seast().x, sw.y + (ne.y - sw.y) / 4) : point(sw.x, sw.y + (ne.y - sw.y) / 4);
            sideEndFir = state == 1 ? nwest() : ne;
            sideEndSec = state == 1 ? sw : seast();
        }
        else {
            sideStartFir = state == 0 ? point(nwest().x + (ne.x - sw.x) / 4, ne.y) : point(sw.x + (ne.x - sw.x) / 4, sw.y);
            sideStartSec = state == 0 ? point(ne.x - (ne.x - sw.x) / 4, ne.y) : point(seast().x - (ne.x - sw.x) / 4, sw.y);
            sideEndFir = state == 0 ? sw : nwest();
            sideEndSec = state == 0 ? seast() : ne;
        }
        try {
            put_line(sw, ne);
            put_line(seast(), nwest());
            put_line(sideStartFir, sideStartSec);
            put_line(sideEndFir, sideEndSec);
            put_line(sideStartFir, sideEndFir);
            put_line(sideStartSec, sideEndSec);
        }
        catch (OutOfScreen& e) {
            this->move(-sw.x, -sw.y);
            this->draw();
            std::cout << e.what();
            std::cout << "Figure was moved for sw to be (0; 0).\n";
        }
    }
};