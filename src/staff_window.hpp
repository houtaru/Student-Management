#ifndef STAFF_WINDOW_H_DEFINED
#define STAFF_WINDOW_H_DEFINED

#include <ncurses.h>

#include <staff.hpp>
#include <object.hpp>
#include <class_node.hpp>
#include <course_node.hpp>
#include <lector_node.hpp>

class StaffWindow {
public:
    StaffWindow();
    
    ~StaffWindow();    

    void load();

private:
    Staff *staff;
    Windows *win;
    ClassNode *classNode;
    CoursesNode *courseNode;
    LectorNode *lectorNode;

};

#endif