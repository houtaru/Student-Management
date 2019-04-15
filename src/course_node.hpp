#ifndef COURSE_NODE_HPP_DEFINED
#define COURSE_NODE_HPP_DEFINED

#include <numeric>
#include <ncurses.h>

#include <staff.hpp>
#include <object.hpp>
#include <student_node.hpp>

class CourseNode {
public:
    CourseNode(string path);
    
    ~CourseNode();

    void getInfo(); // add new course
    
    void displayInfo();

    void displayStudent();
    
    void displayAttend();
    
    void displayScoreboard();

    void display();
private:
    string path;
    Tools *tool;
    System *sys;
    StaffClass * staffClass;
};

class LectCourseNode {
public:
    LectCourseNode(string path);
    
    ~LectCourseNode();

    void getInfo(); // add new course
    
    void displayInfo();

    void displayStudent();
    
    void displayAttend();
    
    void displayScoreboard(string acc = "");

    void display();

    void editScoreboard();
    
    void editAttendance(int pos, vector <int> &att);
private:
    string path;
    Tools *tool;
    System *sys;
    StaffClass * staffClass;
};

class CoursesNode {
public:
    CoursesNode();
    
    ~CoursesNode();

    void load();
    
    void display();
    
    void add();

    void remove();

private:
    System *sys;
    Tools *tool;
    StaffCourse *staffCourse;
};

#endif
