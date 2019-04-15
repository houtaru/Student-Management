//
//  student_node.cpp
//  Student Management
//
//  Created by Khoa Nguyen on 2/25/19.
//  Copyright © 2019 Khoa Nguyen. All rights reserved.
//

#include "student_node.hpp"

StudentNode::StudentNode() {}


StudentNode::StudentNode(string ID, string firstname, string lastname, string gender, string DoB, string classtmp) {
    this -> ID = ID;
    this -> firstname = firstname;
    this -> lastname = lastname;
    this -> gender = gender;
    this -> DoB = DoB;
    this -> classtmp = classtmp;
}

StudentNode::~StudentNode() {}

string StudentNode::getNewOne(int a, int b, int length, int checkType) {
    //Clear the space which user input new string for ID, First name, Last name, Gender, DoB, Class
    int x = a, y = b+1;
    
    attron(A_REVERSE);
    for (int i = 0; i <= length; ++i)
        mvaddch(x, y + i, ' ');
    
    
    //Get the new string
    string newOne = "";
    while (true) {
        int input = getch();
        
        if (input == '\n')
            break;
        
        if ((input == 127 || input == 263) && y > b+1) { //backspace
            mvaddch(x, --y, ' ');
            if (!newOne.empty())
                newOne.pop_back();
        }
        
        //Divide check type to cases, use checkInput to return the boolean value of condition
        bool checkInput = false;
        switch (checkType) {
            case ID_INFO:
                //ID should contain digits only
                if (isdigit(char(input)))
                    checkInput = true;
                break;
            case FIRSTNAME:
                //First name should contain letters and spaces only
                if (isalpha(char(input)) || input == ' ')
                    checkInput = true;
                break;
            case LASTNAME: case GENDER:
                //Last name, Gender should contain letters only
                if (isalpha(char(input)))
                    checkInput = true;
                break;
            case DOB:
                //DoB should contain digits and '-' characters only
                if (isdigit(char(input)) || input == '-')
                    checkInput = true;
                break;
            case CLASS:
                //Class should contain digits and letters only
                if (isdigit(char(input)) || isalpha(char(input)))
                    checkInput = true;
                break;
        };
        
        if (checkInput) {
            newOne += char(input);
            mvaddch(x, y++, char(input));
            checkInput = false;
        }
    }
    
    
    attroff(A_REVERSE);
    refresh();
    
    return newOne;
}


//After editing infor of a student, write down it into that or a new txt whose name is "ID"
void StudentNode::editSave() {
    //Get course link before write to that student file
    ifstream fileInput("data/classes/" + classtmp +  "/" + ID);
    string stuff;
    vector <string> courseLink;
    for (int i = 0; i < 4; ++i)
        getline(fileInput, stuff);
    while (getline(fileInput,stuff))
        courseLink.push_back(stuff);
    fileInput.close();
    int size = courseLink.size();
    
    ofstream fileOutput;
    fileOutput.open("data/classes/" + classtmp +  "/" + ID);
    
    fileOutput << firstname << endl;
    fileOutput << lastname << endl;
    fileOutput << gender << endl;
    fileOutput << DoB << endl;
    
    for (int i = 0; i < size; ++i)
        fileOutput << courseLink[i] << endl;
    
    fileOutput.close();
}


void StudentNode::editID(string newID) {
    remove(("data/classes/" + classtmp + "/" + ID).c_str());
    ID = newID;
    editSave();
}


void StudentNode::editFirstname(string newFirstname) {
    firstname = newFirstname;
    editSave();
}


void StudentNode::editLastname(string newLastname) {
    lastname = newLastname;
    editSave();
}


void StudentNode::editGender(string newGender) {
    gender = newGender;
    editSave();
}


void StudentNode::editDoB(string newDoB) {
    DoB = newDoB;
    editSave();
}


void StudentNode::editClass(string newClass, bool &isRemove) {
    isRemove = true;
    remove(("data/classes/" + classtmp + "/" + ID).c_str());
    classtmp = newClass;
    editSave();
}


void StudentNode::addClass(string newClass) {
    classtmp = newClass;
    editSave();
}


void StudentNode::editInfo(bool &isRemove, bool isEdit, bool flag, bool isLect) {
    rectangle(LINES/2 - 1, COLS/2 - 20, 16, 39).clear(1);
    rectangle winEdit(LINES/2 - 1, COLS/2 - 20, 16, 39);
    
    attron(COLOR_PAIR(2));
    if (isEdit)
        mvprintw(winEdit.top() + 2, COLS/2 - 8, "Edit Information");
    else
        mvprintw(winEdit.top() + 2, COLS/2 - 8, "Add Information");
    attroff(COLOR_PAIR(2));
    mvprintw(winEdit.top() + 5, winEdit.left() + 3, "ID");
    mvprintw(winEdit.top() + 6, winEdit.left() + 3, "First name");
    mvprintw(winEdit.top() + 7, winEdit.left() + 3, "Last name");
    mvprintw(winEdit.top() + 8, winEdit.left() + 3, "Gender");
    mvprintw(winEdit.top() + 9, winEdit.left() + 3, "DoB");
    mvprintw(winEdit.top() + 10, winEdit.left() + 3, "Class");
    
    
    //Display student info lines onto screen and control it
    //0 = ID, 1 = First name, 2 = Last name, 3 = Gender, 4 = DoB, 5 = Class, 6 = [REMOVE], 7 = [BACK]
    int curPtr = ID_INFO;
    
    while (true) {
        int input = getch();
        bool exitWhile = false;
        
        switch (input) {
            case KEY_DOWN:
                curPtr = (curPtr + 1) % (BACK+1);
                break;
            case KEY_UP:
                curPtr = (curPtr - 1 + BACK+1) % (BACK+1);
                break;
            case '\n': {
                if (curPtr == BACK) {
                    exitWhile = true;
                    break;
                }
                if (!isLect) {
                    switch (curPtr) {
                        case ID_INFO:
                            editID(getNewOne(winEdit.top() + 5, COLS/2 - 4, 17, ID_INFO));
                            break;
                        case FIRSTNAME:
                            editFirstname(getNewOne(winEdit.top() + 6, COLS/2 - 4, 17, FIRSTNAME));
                            break;
                        case LASTNAME:
                            editLastname(getNewOne(winEdit.top() + 7, COLS/2 - 4, 17, LASTNAME));
                            break;
                        case GENDER:
                            editGender(getNewOne(winEdit.top() + 8, COLS/2 - 4, 17, GENDER));
                            break;
                        case DOB:
                            editDoB(getNewOne(winEdit.top() + 9, COLS/2 - 4, 17, DOB));
                            break;
                        case CLASS:
                            if (isEdit)
                                editClass(getNewOne(winEdit.top() + 10, COLS/2 - 4, 17, CLASS), isRemove);
                            else
                                addClass(getNewOne(winEdit.top() + 10, COLS/2 - 4, 17, CLASS));
                            break;
                        case SELECT: {
                            if (isEdit && !flag) {
                                isRemove = true;
                                remove(("data/classes/" + classtmp + "/" + ID).c_str());
                            } else {
                                isRemove = true;
                            }
                                
                            exitWhile = true;
                            break;
                        }
                    }
                }
                
                break;
            }
        }
        
        
        //If user choose [BACK], then finish this function
        if (exitWhile)
            break;
        
        
        //Create student info lines for GUI
        vector <string> studentInfoLine;
        string studentInfoLineSub;
        int sizeSub;
        
        studentInfoLineSub = " " + ID;
        sizeSub = studentInfoLineSub.size();
        for (int i = 1; i <= 11+8-sizeSub; ++i)
            studentInfoLineSub += " ";
        studentInfoLine.push_back(studentInfoLineSub);
        
        studentInfoLineSub = " " + firstname;
        sizeSub = studentInfoLineSub.size();
        for (int i = 1; i <= 11+8-sizeSub; ++i)
            studentInfoLineSub += " ";
        studentInfoLine.push_back(studentInfoLineSub);
        
        studentInfoLineSub = " " + lastname;
        sizeSub = studentInfoLineSub.size();
        for (int i = 1; i <= 11+8-sizeSub; ++i)
            studentInfoLineSub += " ";
        studentInfoLine.push_back(studentInfoLineSub);
        
        studentInfoLineSub = " " + gender;
        sizeSub = studentInfoLineSub.size();
        for (int i = 1; i <= 11+8-sizeSub; ++i)
            studentInfoLineSub += " ";
        studentInfoLine.push_back(studentInfoLineSub);
        studentInfoLineSub = " " + DoB;
        
        sizeSub = studentInfoLineSub.size();
        for (int i = 1; i <= 11+8-sizeSub; ++i)
            studentInfoLineSub += " ";
        studentInfoLine.push_back(studentInfoLineSub);
        
        studentInfoLineSub = " " + classtmp;
        sizeSub = studentInfoLineSub.size();
        for (int i = 1; i <= 11+8-sizeSub; ++i)
            studentInfoLineSub += " ";
        studentInfoLine.push_back(studentInfoLineSub);
        
        if (isEdit)
            studentInfoLine.push_back("  [REMOVE]  ");
        else
            studentInfoLine.push_back("  [ADD]  ");
        studentInfoLine.push_back("  [BACK]  ");
        
        
        //Update pointer
        winEdit.drawEdges();
        for (int i = ID_INFO; i < BACK+1; ++i) {
            if (i == curPtr)
                attron(A_REVERSE);
            
            if (i == BACK)
                mvprintw(winEdit.bottom() - 2, COLS/2 + 4, studentInfoLine[i].c_str());
            else {
                if (i == SELECT)
                    mvprintw(winEdit.bottom() - 2, COLS/2 - 16, studentInfoLine[i].c_str());
                else
                    mvprintw(winEdit.top() + 5+i, COLS/2 - 4, studentInfoLine[i].c_str());
            }
            
            if (i == curPtr)
                attroff(A_REVERSE);
        }
    }
    
    
    refresh();
    
    rectangle(LINES/2 - 1, COLS/2 - 20, 16, 39).clear(1);
}

