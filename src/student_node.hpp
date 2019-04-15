//
//  student_node.hpp
//  Student Management
//
//  Created by Khoa Nguyen on 2/25/19.
//  Copyright © 2019 Khoa Nguyen. All rights reserved.
//

#ifndef student_node_hpp
#define student_node_hpp

#include <stdio.h>
#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <iomanip>
#include <ncurses.h>

#include <object.hpp>

using namespace std;

enum Info {
    ID_INFO,
    FIRSTNAME,
    LASTNAME,
    GENDER,
    DOB,
    CLASS,
    SELECT,
    BACK,
};

class StudentNode {
public :
    StudentNode();
    StudentNode(string ID, string firstname, string lastname, string gender, string DoB, string classtmp);
    ~StudentNode();
    
    string getID() {return ID;};
    string getFirstname() {return firstname;};
    string getLastname() {return lastname;};
    string getGender() {return gender;};
    string getDoB() {return DoB;};
    string getClass() {return classtmp;};
    
    void setID(string newID) {ID = newID;};
    void setFirstname(string newFirstname) {firstname = newFirstname;};
    void setLastname(string newLastname) {lastname = newLastname;};
    void setGender(string newGender) {gender = newGender;};
    void setDoB(string newDoB) {DoB = newDoB;};
    void setClass(string newClass) {classtmp = newClass;};
    
    string getNewOne(int a, int b, int length, int checkType);
    
    void editSave();
    
    void editID(string newID);
    void editFirstname(string newFirstname);
    void editLastname(string newLastname);
    void editGender(string newGender);
    void editDoB(string newDoB);
    void editClass(string newClass, bool &isRemove);
    void addClass(string newClass);
    
    void editInfo(bool &isRemove, bool isEdit, bool flag = false, bool isLect = false);
private :
    string ID;
    string firstname;
    string lastname;
    string gender;
    string DoB;
    string classtmp;
    
};

#endif /* student_node_hpp */