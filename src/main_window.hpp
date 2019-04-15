#ifndef MAIN_WINDOW_H_DEFINED
#define MAIN_WINDOW_H_DEFINED

#include <utils.hpp>
#include <object.hpp>
#include <staff_window.hpp>
#include <lector_window.hpp>
#include <student_window.hpp>

class MainWindow {
public:
    MainWindow();
    
    ~MainWindow();

    void load();

private:
    System *sys;
    Windows *win;
    StaffWindow *staffWindow;
    LectureWindow *lectorWindow;
    StudentWindow *studentWindow;

    rectangle frame;
    vector <string> comps;
    vector <string> title;
    vector <rectangle> cwin;
};

#endif