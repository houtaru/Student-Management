#ifndef COURSE_NODE_H_DEFINED
#define COURSE_NODE_H_DEFINED

#include <utils.hpp>
#include <string> 
#include <fstream> 
class Lector {
public:
    Lector();
    
    ~Lector();

    vector <string> getCurrentCourse(string name);

private:
    Tools *tool;
};

#endif