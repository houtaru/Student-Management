#include <staff_window.hpp>

StaffWindow::StaffWindow() {

    enum BUTTONS {
        CLASS,
        COURSE,
        LECTOR,
        BACK
    };

    vector <string> buttons {
        "CLASS MANAGING",
        "COURSE MANAGING",
        "LECTOR MANAGING",
        "BACK"
    };

    vector <rectangle> bwin(4);

    bwin[CLASS] = rectangle(LINES / 3 + 6, COLS / 2 - 16, 3, 27);
    bwin[COURSE] = rectangle(bwin[CLASS].bottom() + 1, bwin[CLASS].left(), 3, 27);
    bwin[LECTOR] = rectangle(bwin[COURSE].bottom() + 1, bwin[CLASS].left(), 3, 27);
    bwin[BACK] = rectangle(bwin[LECTOR].bottom() + 1, bwin[CLASS].left(), 3, 27);

    win = new Windows;
    staff = new Staff;
    staff->import();

    classNode = new ClassNode;
    
    courseNode = new CoursesNode;
    lectorNode = new LectorNode;
    
    win->setCFrame(vector <rectangle>(0));
    win->setComponents(vector <string>(0));
    win->setBFrame(bwin);
    win->setButtons(buttons);

}

StaffWindow::~StaffWindow() {
    delete win;
    delete staff;
    delete classNode;
    delete courseNode;
    delete lectorNode;
}

void StaffWindow::load() {
    rectangle(LINES / 3 + 1, 1, LINES - LINES / 3 - 3, COLS - 2).clear(1);
    refresh();
    int cur = 0, lim = 4;
    while (true) {
        int input = getch();
        if (input == KEY_DOWN) {
            cur = (cur + 1) % lim;
        } else if (input == KEY_UP) {
            cur = (cur - 1 + lim) % lim;
        } else if (input == ' ' || input == '\n') {
            if (cur == 0) {
                classNode->load();
                //mvprintw(1, 1, "exit class manager");
            } else if (cur == 1) {
                courseNode->load();
                //mvprintw(1, 1, "exit course manager");
            } else if (cur == 2) {
                lectorNode->load();
                //mvprintw(1, 1, "exit lector manager");
            } else break;
        }
        win->updatePtr(cur);
    }
    rectangle(LINES / 3 + 1, 1, LINES - LINES / 3 - 3, COLS - 2).clear(true);
}
