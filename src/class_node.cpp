#include <class_node.hpp>

ClassNode::ClassNode() {
    ss = new StudentNode;
    win = new Windows;
    staffClass = new StaffClass;
    
    frame = rectangle(LINES / 3 + 5, COLS / 2 - 20, 20, 40);

    comps = staffClass->getClassList();

    vector <rectangle> cwin;
    for (int i = 0, x = frame.top() + frame.height() / 2 - comps.size() / 2; i < comps.size(); ++i) {
        cwin.push_back(rectangle(x + i, frame.left() + 2, 1, frame.width() - 4));
    }
    
    lim = comps.size() + 1;
}

ClassNode::~ClassNode() {
    delete ss;
    delete win;
    delete staffClass;
}

void ClassNode::load() {
    rectangle(LINES / 3 + 1, 1, LINES - LINES / 3 - 3, COLS - 2).clear(1);

    sort(comps.begin(), comps.end(), [&] (string x, string y) {
        if (x.size() != y.size()) return x.size() < y.size();
        for (int i = 0; i < x.size(); ++i) if (x[i] != y[i])
            return x[i] < y[i];
        return true;
    });

    int cur = 0;
    while (true) {
        int input = getch();
        if (input == KEY_UP || input == KEY_LEFT) {
            cur = (cur - 1 + lim) % lim;
        } else if (input == KEY_DOWN || input == KEY_RIGHT) {
            cur = (cur + 1) % lim;
        } else if (input == ' ' || input == '\n') {
            if (cur < lim - 1) {
                display(comps[cur]);
            } else break;
        }
        win->updatePtr(cur, frame, comps, vector <string> (1, "   [ BACK ]   "));
    }
    rectangle(LINES / 3 + 1, 1, LINES - LINES / 3 - 3, COLS - 2).clear(1);
}

void ClassNode::display(string classX) {
    rectangle(LINES/3 + 1, 1, LINES - LINES/3 - 3, COLS - 2).clear(1);

    rectangle cwin(LINES/2 - 7, COLS/2 - 36, 24, 74);
    cwin.drawEdges();

    init_pair(4, COLOR_CYAN, COLOR_BLACK);
    attron(COLOR_PAIR(4));
    mvprintw(LINES/2 - 5, COLS/2 - 3, classX.c_str());
    attroff(COLOR_PAIR(4));
    attron(COLOR_PAIR(2));
    mvprintw(LINES/2 - 2, COLS/2 - 27 , "ID      First name    Last name     Gender         DoB");
    attroff(COLOR_PAIR(2));
    
    //Get information of students in class X
    vector <string> studentList = staffClass->getStudentList(classX);
    vector <StudentNode> student;
    
    sort(studentList.begin(), studentList.end(), [&] (string x, string y) {
        if (x.size() != y.size()) return x.size() < y.size();
        for (int i = 0; i < x.size(); ++i) if (x[i] != y[i])
            return x[i] < y[i];
        return true;
    });
    
    for (int i = 0; i < studentList.size(); ++i) {
        ifstream fin("data/classes/" + classX + "/" + studentList[i]);
        vector <string> studentInfo;
        string studentInfoSub;
        
        studentInfo.push_back(studentList[i]);
        for (int i = 0; i < 4; ++i) {
            getline(fin,studentInfoSub);
            studentInfo.push_back(studentInfoSub);
        }
        studentInfo.push_back(classX);
        
        StudentNode studentSub(studentInfo[ID_INFO],
                               studentInfo[FIRSTNAME],
                               studentInfo[LASTNAME],
                               studentInfo[GENDER],
                               studentInfo[DOB],
                               studentInfo[CLASS]);
        
        student.push_back(studentSub);
        
        fin.close();
    }
    
    
    //Display student lines on to screen and control the editing
    int curPtr = 0;
    int size = student.size() + 2;
    while (true) {
        int input = getch();
        bool exitWhile = false;
        
        switch (input) {
            case KEY_DOWN:
                curPtr = (curPtr + 1) % size;
                break;
            case KEY_UP:
                curPtr = (curPtr - 1 + size) % size;
                break;
            case '\n':
                if (curPtr == size - 1) {
                    exitWhile = true;
                    break;
                }
                
                bool isEdit;
                bool isRemove = false;
                
                //User click on [ADD]
                if (curPtr == size - 2) {
                    isEdit = false;
                    
                    StudentNode studentSub("", "", "", "", "", classX);
                    studentSub.editInfo(isRemove, isEdit);
                    student.push_back(studentSub);
                    ++size;
                }
                else {
                    isEdit = true;
                    student[curPtr].editInfo(isRemove,isEdit);
                    if (isRemove) {
                        student.erase(student.begin() + curPtr);
                        rectangle(LINES/2 + size-3, COLS/2 - 35, 1, 70).clear(1);
                        --size;
                    }
                    isRemove = false;
                }
                
                break;
        }
        
        
        //If click on [BACK], then finish this function
        if (exitWhile)
            break;
        
        
        //Create student lines for GUI and editing
        vector <string> studentLine;
        for (int i = 0; i < student.size(); ++i) {
            string studentLineSub;
            int sizeSub;
            studentLineSub = " " + student[i].getID();
            sizeSub = student[i].getID().size();
            for (int i = 1; i <= 31-20-sizeSub; ++i)
                studentLineSub += " ";
            studentLineSub += student[i].getFirstname();
            sizeSub = student[i].getFirstname().size();
            for (int i = 1; i <= 20-1-1-sizeSub; ++i)
                studentLineSub += " ";
            studentLineSub += student[i].getLastname();
            sizeSub = student[i].getLastname().size();
            for (int i = 1; i <= 10-sizeSub; ++i)
                studentLineSub += " ";
            studentLineSub += student[i].getGender();
            sizeSub = student[i].getGender().size();
            for (int i = 1; i <= 22-9-1-sizeSub; ++i)
                studentLineSub += " ";
            studentLineSub += student[i].getDoB() + " ";
            
            
            studentLine.push_back(studentLineSub);
        }
        studentLine.push_back("  [   ADD   ]  ");
        studentLine.push_back("  [   BACK   ]  ");
        
        cwin.drawEdges();
        //Update pointer
        for (int i = 0; i < size; ++i) {
            if (i == curPtr)
                attron(A_REVERSE);
            
            if (i == size - 1)
                mvprintw(cwin.bottom() - 2, COLS/2 + 10, studentLine[i].c_str());
            else {
                if (i == size - 2)
                    mvprintw(cwin.bottom() - 2, COLS/2 - 19, studentLine[i].c_str());
                else
                    mvprintw(LINES/2 + i, COLS/2 - 31, studentLine[i].c_str());
            }
            
            if (i == curPtr)
                attroff(A_REVERSE);
        }
        refresh();
        
    }
    
    
    refresh();
    
    rectangle(LINES/3 + 1, 1, LINES - LINES/3 - 3, COLS - 2).clear(1);
}
