#ifndef LECTURER_H_DEFINED
#define LECTURER_H_DEFINED

#include <object.hpp>
#include <utils.hpp>
#include <course_node.hpp>
#include <lector.hpp>

class LectureWindow {
public:
    LectureWindow();
    
    ~LectureWindow();

    void init(string lecture_account);

    void load(string lector_account);

    void info_window(string lecture_account, int cur1);

private:
    Tools *tool = new Tools;
    Windows *win = new Windows;
    Lector *lector = new Lector;

    rectangle frame;
    vector <string> buttons;
    vector <string> comps;
};

#endif