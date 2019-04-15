#ifndef STAFF_H_DEFINED
#define STAFF_H_DEFINED

#include <dirent.h>
#include <sys/types.h>

#include <map>
#include <string>
#include <vector>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <algorithm>

#include <md5.hpp>
#include <utils.hpp>

using namespace std;

class StaffClass {
public:
    StaffClass();

    ~StaffClass();
    
    void import(string);

    vector <string> getClassList();

    vector <string> getStudentList(string classtmp);

private:
    Tools *tool;
};

class StaffCourse {
public:
    StaffCourse();

    ~StaffCourse();
    
    void import(string);
    
    vector <string> getYearList(string path);
    
    vector<string> getSemesterList(string path); 
    
    vector<string> getCourseList(string path); 
    
    vector<string> getCoursesInfo(string path); 

    vector <string> getData(int id, string path);

private:
    Tools *tool;
    StaffClass *staffClass;
};

class Staff {
public:
    Staff();
    
    ~Staff();
    
    void import();

private:
    System *sys;
    Tools *tool;
    StaffClass *staffClass;
    StaffCourse *staffCourse;
};

#endif