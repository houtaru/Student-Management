#include <lector_node.hpp>

LectorNode::LectorNode() {
    tool = new Tools;
}

LectorNode::~LectorNode() {
    delete tool;
}

void LectorNode::load() {
    rectangle(LINES / 3 + 1, 1, LINES - LINES / 3 - 3, COLS - 2).clear(1);
    rectangle frame(LINES / 3 + 5, COLS / 2 - 20, 20, 40);
    
    vector <string> comps = tool->read_directory("data/lecturers");
    sort(comps.begin(), comps.end(), [&](string x, string y) {
        return strcmp(x.c_str(), y.c_str()) <= 0;
    });
    vector <string> buttons = {
        "[   BACK   ]"
    };

    auto updateptr = [&](int ptr) {
        frame.drawEdges();
        for (int i = 0; i < comps.size(); ++i) {
            if (i == ptr) attron(A_REVERSE);
            for (int k = frame.left() + 1; k < frame.right(); ++k) {
                mvaddch(frame.top() + 3 + i, k, ' ');
            }
            mvprintw(frame.top() + 3 + i, frame.left() + frame.width() / 2 - comps[i].size() / 2 - 1, comps[i].c_str());

            if (i == ptr) attroff(A_REVERSE);
        }
        ptr -= comps.size();
        for (int i = 0; i < buttons.size(); ++i) {
            if (i == ptr) attron(A_REVERSE);
            mvaddstr(frame.bottom(), frame.left() + frame.width() / (buttons.size() + 1) * (i + 1) - buttons[i].size() / 2, buttons[i].c_str());
            if (i == ptr) attroff(A_REVERSE);
        }
        refresh();
    };

    int cur = 0, lim = comps.size() + buttons.size();
    while (true) {
        int input = getch();
        if (input == KEY_UP || input == KEY_LEFT) {
            cur = (cur - 1 + lim) % lim;
        } else if (input == KEY_DOWN || input == KEY_RIGHT) {
            cur = (cur + 1) % lim;
        } else if (input == ' ' || input == '\n') {
            if (cur == lim - 1) break;
            rectangle xwin(frame.bottom() - 6, COLS / 2 - 25, 3, 50);
            xwin.clear(1);
            attron(A_BLINK);
            mvaddstr(xwin.top() + 1, xwin.left() + 5, " Press R to remove or press C to cancel!    ");
            attroff(A_BLINK);
            refresh();
            while (true) {
                int xx = getch();
                if (xx == 'C' || xx == 'c') {
                    break;
                } else if (xx == 'r' || xx == 'R') {
                    // remove lecturers out of storage
                    system(("rm -r data/lecturers/" + comps[cur]).c_str()); 

                    // remove from current temporaty memory
                    lim--;
                    comps.erase(comps.begin() + cur);
                    break;
                }
            }
            frame.clear();
        }
        rectangle(1, 1, 10, 5).clear(1);
        updateptr(cur);
    }

    rectangle(LINES / 3 + 1, 1, LINES - LINES / 3 - 3, COLS - 2).clear(1);
}