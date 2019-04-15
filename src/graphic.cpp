#ifndef GRAPHIC_H_DEFIEND
#define GRAPHIC_H_DEFIEND

#include <graphic.hpp>

//#include <staff_window.hpp>

/*      *       *       *       *       OVERALL       *       *       *       *       */

Graphic::Graphic() {
    initscr(); //Initialises all implementation data structure and enviroment variable specifics.
    cbreak(); //Disable line buffering
    noecho(); //switches off echoing
    clear(); //clear terminal
    refresh();
    
    curs_set(0); //set appearance of the cursor to invisible
    keypad(stdscr, true); //turn on keypad translation
    nodelay(stdscr, true); //set stdscr to No Delay Mode

    start_color(); //start color functionalitsy
    if (!has_colors()) {
        printf("ERROR: Terminal does not support color!\n");
        exit(0);
    }
    attron(A_BOLD);
    window = new MainWindow;
    
}

Graphic::~Graphic() {
    attroff(A_BOLD);
    delete window;
    endwin();
}

void Graphic::load() {
    rectangle(0, 0, LINES, COLS).drawEdges();

    window->load();
}

/*      *       *       *       *       STAFF       *       *       *       *       */

/*      *       *       *       *       LECTORS       *       *       *       *       */

/*      *       *       *       *       STUDENTS       *       *       *       *       */

#endif
