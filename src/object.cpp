#include <object.hpp>

/*      *       *       *       *       RACTANGLE       *       *       *       *       */

rectangle::rectangle(int x, int y, int h, int w) {
    this->x = x;
    this->y = y;
    this->h = h;
    this->w = w;
}

void rectangle::clear(bool includeBorder, bool isRev) {
    if (height() == 1) includeBorder = true;
    if (isRev) attron(A_REVERSE);
    for (int i = top() + !includeBorder; i <= bottom() - !includeBorder; ++i)
        for (int j = left() + !includeBorder; j <= right() - !includeBorder; ++j)
            mvaddch(i, j, ' ');
    if (isRev) attroff(A_REVERSE);
}

int rectangle::top() { return x; }
int rectangle::bottom() { return x + h - 1; }
int rectangle::left() { return y; }
int rectangle::right() { return y + w - 1; }

int rectangle::width() { return w; }
int rectangle::height() { return h; }

void rectangle::drawEdges() {
    attron(A_BOLD);
    for (int i = top(); i <= bottom(); ++i) {
        int x = i - top();
        mvaddch(i, left(), ACS_VLINE);
        mvaddch(i, right(), ACS_VLINE);
    }
    for (int i = left(); i <= right(); ++i) {
        int x = i - left();
        mvaddch(top(), i, ACS_HLINE);
        mvaddch(bottom(), i, ACS_HLINE);
    }
    mvaddch(top(), left(), ACS_ULCORNER);
    mvaddch(top(), right(), ACS_URCORNER);
    mvaddch(bottom(), left(), ACS_LLCORNER);
    mvaddch(bottom(), right(), ACS_LRCORNER);
}
void rectangle::drawTable() {
    attron(A_BOLD);
    for (int x = top(); x <= bottom(); ++x)
        for (int y = left(); y <= right(); ++y) {
            int h = x - top(), w = y - left();
            if (h % 2 && w % 2) continue;
            if (h % 2) {
                mvaddch(x, y, ACS_VLINE);
            } else {
                mvaddch(x, y, (w % 2) ? ACS_HLINE : ACS_DIAMOND);
            }
        }
    attroff(A_BOLD);
    refresh();
}

/*      *       *       *       *       WINDOW       *       *       *       *       */

void Windows::setButtons(vector <string> bb) {
    this->buttons = bb;
}

void Windows::setComponents(vector <string> cc) {
    this->comps = cc;
}

void Windows::setBFrame(vector <rectangle> bf) {
    this->bwin = bf;
}

void Windows::setCFrame(vector <rectangle> cf) {
    this->cwin = cf;
}

void Windows::clear(int x, int y, int h, int w) {
    for (int i = 0; i < h; ++i) for (int j = 0; j < w; ++j)
        mvaddch(x + i, y + j, ' ');
}

void Windows::updatePtr(int cur, int mask) {
    for (int i = 0; i < (int) cwin.size(); ++i) {
        if (i < cwin.size()) cwin[i].drawEdges();
        if (i == cur) attron(A_REVERSE);
        cwin[i].clear();
        if (mask >> i & 1) {
            mvaddstr(cwin[i].top() + 1, cwin[i].left() + 2, comps[i].c_str());
        } else {
            mvaddstr(cwin[i].top() + 1, cwin[i].left() + cwin[i].width() / 2 - comps[i].size() / 2, comps[i].c_str());
        }
        if (i == cur) attroff(A_REVERSE);
    }

    cur -= comps.size();

    for (int i = 0; i < (int) bwin.size(); ++i) {
        bwin[i].drawEdges();
        if (i == cur) attron(A_REVERSE);
        bwin[i].clear();
        mvaddstr(bwin[i].top() + 1, bwin[i].left() + bwin[i].width() / 2 - buttons[i].size() / 2, buttons[i].c_str());
        if (i == cur) attroff(A_REVERSE);
    }
    refresh();
}

void Windows::updatePtr(int cur, rectangle frame, vector <string> comps, vector <string> buttons) {
    frame.drawEdges();
    int x = frame.top() + frame.height() / 2 - comps.size() / 2 - 3;
    for (int i = 0; i < (int) comps.size(); ++i) {
        if (i == cur) attron(A_REVERSE);
        for (int k = frame.left() + 2; k < frame.right() - 1; ++k) mvaddch(x + i, k, ' ');
        mvaddstr(x + i, frame.left() + frame.width() / 2 - comps[i].size() / 2, comps[i].c_str());
        if (i == cur) attroff(A_REVERSE);
    }

    cur -= comps.size();

    for (int i = 0; i < (int) buttons.size(); ++i) {
        if (i == cur) attron(A_REVERSE);
        mvaddstr(frame.bottom() - 2, frame.left() + frame.width() / 2 - buttons[i].size() / ((int) buttons.size() + 1) * (i + 1), buttons[i].c_str());
        if (i == cur) attroff(A_REVERSE);
    }
    refresh();
}
