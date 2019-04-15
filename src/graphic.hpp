#ifndef GRAPHIC_H_DEFINED
#define GRAPHIC_H_DEFINED

#include <ncurses.h>

#include <vector>
#include <string>
#include <utility>

#include <object.hpp>
#include <main_window.hpp>

using namespace std;

class Graphic {
public:
    Graphic();
    
    ~Graphic();

    void load();

private:
    MainWindow *window;
};

#endif