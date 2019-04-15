#ifndef OBJECT_H_DEFINED
#define OBJECT_H_DEFINED

#include <vector>
#include <string>

#include <ncurses.h>

using namespace std;

class rectangle {
public:
    rectangle() {}
    
    rectangle(int x, int y, int h, int w);
    
    void clear(bool includeBorder = false, bool isRev = false);
    
    int top();

    int bottom();

    int left();

    int right();

    int width();

    int height();

    void drawEdges();

    void drawTable();

private:
    int x, y, w, h;
};


class Windows {
public:
    void setButtons(vector <string> bb);
    
    void setComponents(vector <string> cc);

    void setBFrame(vector <rectangle> bf);

    void setCFrame(vector <rectangle> cf);

    void clear(int x, int y, int h, int w);

    void updatePtr(int cur, int mask = 0);

    void updatePtr(int cur, rectangle frame, vector <string> comps, vector <string> buttons);

    void load();

private:
    vector <rectangle> cwin;
    vector <rectangle> bwin;
    vector <string> comps;
    vector <string> buttons;
};


#endif