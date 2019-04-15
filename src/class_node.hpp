#ifndef CLASS_NODE_H_DEFINED
#define CLASS_NODE_H_DEFINED

#include <algorithm>
#include <ncurses.h>

#include <staff.hpp>
#include <object.hpp>
#include <student_node.hpp>

class ClassNode {
public:
    ClassNode();
    
    ~ClassNode();

    void load();

    void display(string classX);

private:
    Windows *win;
    StudentNode *ss;
    StaffClass *staffClass;

    int lim;
    rectangle frame;
    vector <string> comps;
};

#endif