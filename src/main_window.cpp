#include <main_window.hpp>

MainWindow::MainWindow() {
    title = {
        "                            ______   _______   __   __   _____     _______   __    __   _______                  ",
        "                           / ____/  /__  __/  / /  / /  / ___ \\   / _____/  /  |  / /  /__  __/                  ",
        "                          / /         / /    / /  / /  / /  / /  / /____   / | | / /     / /                     ",
        "                          \\ \\        / /    / /  / /  / /  / /  / _____/  / /| |/ /     / /                      ",
        "                        ___\\ \\      / /    / /__/ /  / /__/ /  / /____   / / | | /     / /                       ",
        "                       /_____/     /_/    /______/  /______/  /______/  /_/  |__/     /_/                        ",
        "",
        "     __   __       ____      __    __   ____      _______    _______   __   __       _______   __    __   _______",
        "    /  | /  |     /   |     /  |  / /  /   |     / _____/   / _____/  /  | /  |     / _____/  /  |  / /  /__  __/",
        "   / | |/ / |    / /| |    / | | / /  / /| |    / / __     / /____   / | |/ / |    / /____   / | | / /     / /   ",
        "  / /|___/| |   / __  |   / /| |/ /  / ___ |   / / /_ \\   / _____/  / /|___/| |   / _____/  / /| |/ /     / /    ",
        " / /      | |  / /  | |  / / | | /  / /  | |  / /___/ /  / /____   / /      | |  / /____   / / | | /     / /     ",
        "/_/       |_| /_/   |_| /_/  |__/  /_/   |_| /_______/  /______/  /_/       |_| /______/  /_/  |__/     /_/      "
    };
    
    comps = {
        "",
        ""
    };
    
    vector <string> buttons = {
        "SUBMIT",
        "EXIT"
    };

    vector <rectangle> bwin(2);
    cwin.resize(2);
    
    enum COMPONENTS {
        USERNAME,
        PASSWORD
    };

    enum BUTTONS {
        SUBMIT,
        CANCEL
    };
    
    frame = rectangle(LINES / 3 + 5, COLS / 2 - 25, 15, 50);
    cwin[USERNAME] = rectangle(frame.top() + 1, frame.left() + 17, 3, 30);
    cwin[PASSWORD] = rectangle(cwin[USERNAME].bottom() + 1, cwin[USERNAME].left(), 3, 30);
    
    bwin[SUBMIT] = rectangle(cwin[PASSWORD].bottom() + 4, frame.left() + frame.width() / 4 - 5, 3, 10);
    bwin[CANCEL] = rectangle(bwin[SUBMIT].top(), frame.left() + frame.width() / 4 * 3 - 5, 3, 10);

    win = new Windows;
    sys = new System;
    staffWindow = new StaffWindow;
    lectorWindow = new LectureWindow;
    studentWindow = new StudentWindow;
    
    win->setBFrame(bwin);
    win->setCFrame(cwin);
    win->setComponents(comps);
    win->setButtons(buttons);
};

MainWindow::~MainWindow() {
    delete sys;
    delete win;
    delete staffWindow;
    delete lectorWindow;
    delete studentWindow;
}

void MainWindow::load() {
    int bottom = LINES / 3;
    init_pair(1, COLOR_MAGENTA, COLOR_BLACK);
    attron(COLOR_PAIR(1));
    for (int i = 0; i < title.size(); ++i) 
        mvprintw(LINES / 3 - title.size() + i, COLS / 2 - title[0].size() / 2 - 2, "%s", title[i].c_str());
    attroff(COLOR_PAIR(1));
    int cur = 0;
    while (true) {
        int input = getch();
        if (input == KEY_DOWN || input == KEY_RIGHT) {
            cur = (cur + 1) % 4;
        } if (input == KEY_UP || input == KEY_LEFT) {
            cur = (cur + 3) % 4;
        } else if (input == ' ' || input == '\n') {
            if (cur == 0) {
                cwin[0].clear(false, true);
                comps[0] = sys->getID(cwin[0].top() + 1, cwin[0].left() + 2);
                win->setComponents(comps);

                win->updatePtr(++cur, (1 << 0) + (1 << 1));

                cwin[1].clear(false , true);
                comps[1] = sys->getPassword(cwin[1].top() + 1, cwin[1].left() + 2);
                win->setComponents(comps);
                cur++;
            } else if (cur == 1) {
                cwin[1].clear(false , true);
                comps[1] = sys->getPassword(cwin[1].top() + 1, cwin[1].left() + 2);
                win->setComponents(comps);
                cur++;
            } else if (cur == 2) {
                int k = sys->checkPass();
                switch (k) {
                    case 0: // Staff
                        staffWindow->load();
                        //mvaddstr(1, 1, "staff");
                        break;
                    case 1: // Lector
                        lectorWindow->load(comps[0]);
                        rectangle(LINES / 3 + 1, 1, LINES - LINES / 3 - 3, COLS - 2).clear(1);
                        //mvaddstr(1, 1, "lector");
                        break;
                    case 2: // Student
                        studentWindow->load(comps[0], sys->getClass());
                        //mvaddstr(1, 1, "student");
                        break;
                    default:
                        attron(A_BLINK);
                        init_pair(3, COLOR_RED, COLOR_BLACK);
                        attron(COLOR_PAIR(3));
                        mvaddstr(cwin[1].bottom() + 2, frame.left() + frame.width() / 2 - 12, "Invalid username/password");
                        attron(COLOR_PAIR(3));
                        attroff(A_BLINK);
                        break;
                }
            } else {
                break;
            }
        }
        frame.drawEdges();
        init_pair(2, COLOR_YELLOW, COLOR_BLACK);
        attron(COLOR_PAIR(2));
        mvaddstr(frame.top() + 2, frame.left() + 5, "USERNAME");
        mvaddstr(frame.top() + 5, frame.left() + 5, "PASSWORD");
        attroff(COLOR_PAIR(2));
        win->updatePtr(cur, (1 << 0) + (1 << 1));
    }
}
