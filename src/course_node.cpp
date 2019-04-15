#include <course_node.hpp>

/*      *       *       *       *       OVERALL     *       *       *       *       *       */

CourseNode::CourseNode(string path) { 
    this->path = path; 
    
    tool = new Tools;
    sys = new System;
    staffClass = new StaffClass;
}

CourseNode::~CourseNode() {
    delete sys;
    delete tool;
    delete staffClass;
}

void CourseNode::display() {
    rectangle(LINES / 3 + 1, 1, LINES - LINES / 3 - 3, COLS - 2).clear(1);

    enum COURSE {
        INFO,
        STUDENT,
        ATTENDANCE,
        SCOREBOARD,
        REMOVE,
        BACK
    };

    const vector <string> ss {
        "COURSE INFO",
        "STUDENT LIST",
        "ATTENDANCE LIST",
        "SCOREBOARD",
        "REMOVE",
        "BACK"
    };

    rectangle cwin[10];
    cwin[INFO] = rectangle(LINES / 3 + 6, COLS / 2 - 16, 3, 27);
    cwin[STUDENT] = rectangle(cwin[INFO].bottom() + 1, COLS / 2 - 16, 3, 27);
    cwin[ATTENDANCE] = rectangle(cwin[STUDENT].bottom() + 1, COLS / 2 - 16, 3, 27);
    cwin[SCOREBOARD] = rectangle(cwin[ATTENDANCE].bottom() + 1, COLS / 2 - 16, 3, 27);
    cwin[REMOVE] = rectangle(cwin[SCOREBOARD].bottom() + 1, cwin[INFO].left() - 10, 3, 15);
    cwin[BACK] = rectangle(cwin[REMOVE].top(), cwin[INFO].right() - 5, 3, 15);
    
    auto updateptr = [&](int ptr, int stat, int end) {
        for (int i = stat; i <= end; ++i) {
                cwin[i].drawEdges();
                if (i == ptr) attron(A_REVERSE);
                for (int j = cwin[i].left() + 1; j < cwin[i].right(); ++j)
                    mvaddch(cwin[i].top() + 1, j, ' ');
                int y = cwin[i].left() + cwin[i].width() / 2 - ss[i].size() / 2;
                mvaddstr(cwin[i].top() + 1, y, ss[i].c_str());
                if (i == ptr) attroff(A_REVERSE);
            }
            refresh();
    };

    int curPtr = 0, lim = ss.size();
    while (true) {
        int input = getch();
        if (input == KEY_DOWN) {
            curPtr = (curPtr + 1) % lim;
        } else if (input == KEY_UP) {
            curPtr = (curPtr - 1 + lim) % lim;
        } else if (input == '\n' || input == ' ') {
            if (curPtr == INFO) displayInfo();
            else if (curPtr == ATTENDANCE) displayAttend();
            else if (curPtr == SCOREBOARD) displayScoreboard();
            else if (curPtr == STUDENT) displayStudent();
            else {
                if (curPtr == REMOVE) {
                    //system("echo 1 >> error.log");
                    //system(("echo \"" + path + "\" >> error.log").c_str());
                    system(("rm -r \"" + path + "\"").c_str());
                    //mvprintw(1, 1, "%s", path.c_str());
                }
                break;
            }
        }
        updateptr(curPtr, 0, lim - 1);
    }
    rectangle(LINES / 3 + 1, 1, LINES - LINES / 3 - 3, COLS - 2).clear(1);
}

void CourseNode::displayInfo() {
    rectangle(LINES / 3 + 1, 1, LINES - LINES / 3 - 3, COLS - 2).clear(1);
    
    //lecturer account[0], name[1], start[2], end[3], day[4], shour[5], ehour[6], room[7]
    vector <string> text {
        "ID          :",
        "Name        :",
        "Start date: ",
        "End date    :",
        "Day of week :",
        "Start Hour  :",
        "End Hour    :",
        "Room        :"
    };
    vector <string> cur = tool->getContent(path + "/info");

    enum INFO {
        FRAME,
        SAVE_BUTTON,
        CANCEL_BUTTON
    };
    rectangle cwin[3];
    cwin[FRAME] = rectangle(LINES / 3 + 6, COLS / 2 - 25, 15, 50);
    cwin[SAVE_BUTTON] = rectangle(cwin[FRAME].bottom() - 1, cwin[FRAME].left() + cwin[FRAME].width() / 4 - 6, 3, 12);
    cwin[CANCEL_BUTTON] = rectangle(cwin[SAVE_BUTTON].top(), cwin[FRAME].left() + cwin[FRAME].width() * 3 / 4 - 6, 3, 12);
    
    cwin[FRAME].drawEdges();

    auto updateptr = [&] (int curPtr) {
        int tmp = (curPtr - text.size()) % 2;
        if (curPtr >= cur.size()) {
            if (!(tmp % 2)) {
                attron(A_REVERSE);
                mvaddstr(cwin[CANCEL_BUTTON].top() + 1, cwin[CANCEL_BUTTON].left() + 1, "[   CANCEL   ]");
                attroff(A_REVERSE);
                mvaddstr(cwin[SAVE_BUTTON].top() + 1, cwin[SAVE_BUTTON].left() + 1, "[    SAVE    ]");
            } else {
                mvaddstr(cwin[CANCEL_BUTTON].top() + 1, cwin[CANCEL_BUTTON].left() + 1, "[   CANCEL   ]");
                attron(A_REVERSE);
                mvaddstr(cwin[SAVE_BUTTON].top() + 1, cwin[SAVE_BUTTON].left() + 1, "[    SAVE    ]");
                attroff(A_REVERSE);
            }
        } else {
            mvaddstr(cwin[CANCEL_BUTTON].top() + 1, cwin[CANCEL_BUTTON].left() + 1, "[   CANCEL   ]");
            mvaddstr(cwin[SAVE_BUTTON].top() + 1, cwin[SAVE_BUTTON].left() + 1, "[    SAVE    ]");
        }
        
        for (int i = 1; i < cur.size(); ++i) {
            mvaddstr(cwin[FRAME].top() + 1 + i, cwin[FRAME].left() + 3, text[i - 1].c_str());
            if (curPtr == i) attron(A_REVERSE);
            for (int y = cwin[FRAME].left() + cwin[FRAME].width() / 2; y < cwin[FRAME].right() - 2; ++y)
                mvaddch(cwin[FRAME].top() + 1 + i, y, ' ');
            mvaddstr(cwin[FRAME].top() + 1 + i, cwin[FRAME].left() + cwin[FRAME].width() / 2 + 2, cur[i].c_str());
            if (curPtr == i) attroff(A_REVERSE);
        }
    };

    int curPtr = 0, lim = cur.size() + 1;
    while (true) {
        int input = getch();
        if (input == KEY_UP || input == KEY_LEFT) {
            curPtr = (curPtr - 1 + lim) % lim;
        } else if (input == KEY_DOWN || input == KEY_RIGHT) {
            curPtr = (curPtr + 1) % lim;
        } else if (input == '\n' || input == ' ') {
            if (curPtr < cur.size() - 1) {
                attron(A_REVERSE);
                for (int y = cwin[FRAME].left() + cwin[FRAME].width() / 2; y < cwin[FRAME].right() - 2; ++y)
                    mvaddch(cwin[FRAME].top() + 2 + curPtr, y, ' ');
                attroff(A_REVERSE);
                string tex = sys->getInput(cwin[FRAME].top() + 2 + curPtr, cwin[FRAME].left() + cwin[FRAME].width() / 2 + 2);
                cur[curPtr + 1] = tex;
            } else  {
                if (curPtr < cur.size()) sys->updateData(path + "/info", cur);
                break;
            }
        }
        updateptr(curPtr + 1);
    }
     
    rectangle(LINES / 3 + 1, 1, LINES - LINES / 3 - 3, COLS - 2).clear(1);
}

void CourseNode::displayStudent() {
    rectangle(LINES/3 + 1, 1, LINES - LINES/3 - 3, COLS - 2).clear(1);

    rectangle cwin;
    cwin = rectangle(LINES/2 - 6, COLS/2 - 36, 24, 80);
    cwin.drawEdges();

    //mvprintw(LINES/2 - 5, COLS/2 - 3, classX.c_str());
    mvprintw(LINES/2 - 2, COLS/2 - 27 , "ID      First name    Last name     Gender         DoB");
    
    //Get information of students in class X
    vector <string> studentList = tool->getContent(path + "/student_list");

    vector <StudentNode> student;
    
    for (int i = 0; i < studentList.size(); ++i) {
        string classX = tool->getClass(studentList[i]);
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
                    
                    StudentNode studentSub("", "", "", "", "", "");
                    studentSub.editInfo(isRemove, isEdit, 1);
                    student.push_back(studentSub);
                    ++size;
                }
                else {
                    isEdit = true;
                    student[curPtr].editInfo(isRemove,isEdit);
                    if (isRemove) {
                        studentList.erase(studentList.begin() + curPtr);
                        student.erase(student.begin() + curPtr);
                        sys->updateData(path + "/student_list", studentList);
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

void CourseNode::displayAttend() {
    rectangle(LINES / 3 + 1, 1, LINES - LINES / 3 - 3, COLS - 2).clear(1);
    
    rectangle cwin[3];
    cwin[0] = rectangle(LINES / 3 + 6, COLS / 2 - 25, 20, 50);
    cwin[1] = rectangle(cwin[0].bottom() - 1, cwin[0].left() + cwin[0].width() / 4 - 8, 3, 16);
    cwin[2] = rectangle(cwin[0].bottom() - 1, cwin[0].left() + cwin[0].width() * 3 / 4 - 8, 3, 16);

    cwin[0].drawEdges();

    mvaddstr(cwin[0].top() + 1, cwin[0].left() + cwin[0].width() / 4 - 5, "Student ID");
    mvaddstr(cwin[0].top() + 1, cwin[0].left() + cwin[0].width() * 3 / 4 - 5, "Attendance");

    vector <string> studentList;
    vector < vector <int> > comps;

    ifstream fin[2];
    fin[0].open(path + "/student_list");
    fin[1].open(path + "/attendance_list");

    vector <string> csv; 
    string xxx = "No,Student ID";
    for (int i = 0; i < 10; ++i) xxx += ",Week " + to_string(i + 1);
    csv.push_back(xxx);

    string id, classname;
    int dx = cwin[0].top() + 3, cnt = 0;
    while (fin[0] >> id) {
        mvaddstr(dx, cwin[0].left() + cwin[0].width() / 4 - id.size() / 2, id.c_str());
        
        vector <int> cur(10);
        for (int i = 0; i < 10; ++i) fin[1] >> cur[i];
        
        int x = accumulate(cur.begin(), cur.end(), 0), y = 10;
        
        comps.push_back(cur);
        studentList.push_back(id);
        string tmp = to_string(++cnt) + "," + id;
        for (int i = 0; i < 10; ++i) tmp += "," + to_string(cur[i]);
        csv.push_back(tmp);
    }

    vector <string> buttons {
        "[  EXPORT  ]",
        "[  BACK  ]"
    };
    
    auto updateptr = [&](int ptr) {
        cwin[0].drawEdges();
        for (int i = 0; i < comps.size(); ++i) {
            if (i == ptr) attron(A_REVERSE);
            for (int k = cwin[0].left() + 1; k < cwin[0].right(); ++k) {
                mvaddch(cwin[0].top() + 3 + i, k, ' ');
            }
            mvprintw(cwin[0].top() + 3 + i, cwin[0].left() + cwin[0].width() / 4 - studentList[i].size() / 2, "%s", studentList[i].c_str());
            mvprintw(cwin[0].top() + 3 + i, cwin[0].left() + cwin[0].width() / 4 * 3 - 2, "%d / %d", accumulate(comps[i].begin(), comps[i].end(), 0), 10);

            if (i == ptr) attroff(A_REVERSE);
        }
        ptr -= comps.size();
        for (int i = 0; i < buttons.size(); ++i) {
            if (i == ptr) attron(A_REVERSE);
            mvaddstr(cwin[0].bottom(), cwin[0].left() + cwin[0].width() / (buttons.size() + 1) * (i + 1) - buttons[i].size() / 2, buttons[i].c_str());
            if (i == ptr) attroff(A_REVERSE);
        }
        refresh();
    };

    int cur = 0, lim = comps.size() + buttons.size();
    while (true) {
        int input = getch();
        if (input == KEY_UP || input == KEY_LEFT) {
            cur = (cur - 1 + lim) % lim;
        } else if (input == KEY_DOWN || input == KEY_RIGHT) {
            cur = (cur + 1) % lim;
        } else if (input == ' ' || input == '\n') {
            if (cur == comps.size()) {
                ofstream fout(path + "/attendance.csv");
                for (auto it : csv) fout << it << "\n";
                fout.close();

                attron(A_BLINK);
                mvaddstr(cwin[0].bottom() - 3, cwin[0].left() + cwin[0].width() / 2 - 1, "DONE!");
                attroff(A_BLINK);
            } if (cur == lim - 1 ) break;
        }
        updateptr(cur);
    }

    rectangle(LINES / 3 + 1, 1, LINES - LINES / 3 - 3, COLS - 2).clear(1);
}

void CourseNode::displayScoreboard() {
    rectangle(LINES / 3 + 1, 1, LINES - LINES / 3 - 3, COLS - 2).clear(1);
    rectangle cwin[3];
    cwin[0] = rectangle(LINES / 3 + 6, COLS / 2 - 35, 20, 72);
    cwin[1] = rectangle(cwin[0].bottom() - 1, cwin[0].left() + cwin[0].width() / 4 - 8, 3, 16);
    cwin[2] = rectangle(cwin[0].bottom() - 1, cwin[0].left() + cwin[0].width() * 3 / 4 - 8, 3, 16);

    cwin[0].drawEdges();

    mvaddstr(cwin[0].top() + 1, cwin[0].left() + cwin[0].width() / 6 - 7, "Student ID");
    mvaddstr(cwin[0].top() + 1, cwin[0].left() + cwin[0].width() * 2 / 6 - 3, "Midterm");
    mvaddstr(cwin[0].top() + 1, cwin[0].left() + cwin[0].width() * 3 / 6 - 1, "Final");
    mvaddstr(cwin[0].top() + 1, cwin[0].left() + cwin[0].width() * 4 / 6 - 1, "Lab");
    mvaddstr(cwin[0].top() + 1, cwin[0].left() + cwin[0].width() * 5 / 6 - 2, "Bonus");

    vector <string> csv; csv.push_back("No,Student ID,Midterm,Final,Lab,Bonus");

    ifstream fin[2];
    fin[0].open(path + "/student_list");
    fin[1].open(path + "/scoreboard");


    int dx = cwin[0].top() + 3, cnt = 0;
    string id, classname;
    while (fin[0] >> id) {
        mvaddstr(dx, cwin[0].left() + cwin[0].width() / 6 - 6, id.c_str());
        
        id = to_string(++cnt) + "," + id;
        double a[4]; fin[1] >> a[0] >> a[1] >> a[2] >> a[3];
        for (int i = 2; i <= 5; ++i) {
            mvprintw(dx, cwin[0].left() + cwin[0].width() * i / 6 - 1, "%.2f", a[i - 2]);
            id += ',' + to_string(a[i - 2]);
        }
        dx++;
        csv.push_back(id);
    }
    fin[0].close(); fin[1].close();

    vector <string> ss {
        "  [ EXPORT ]  ",
        "  [ BACK ]   "
    };
    
    auto updateptr = [&](int ptr, int stat, int end) {
        for (int i = stat; i <= end; ++i) {
            if (i == ptr) attron(A_REVERSE);
            for (int j = cwin[i].left() + 1; j < cwin[i].right(); ++j)
                mvaddch(cwin[i].top() + 1, j, ' ');
            int y = cwin[i].left() + cwin[i].width() / 2 - ss[i - 1].size() / 2;
            mvaddstr(cwin[i].top() + 1, y, ss[i - 1].c_str());
            if (i == ptr) attroff(A_REVERSE);
        }
        refresh();
    };

    int curPtr = 0;
    while (true) {
        int input = getch();
        if (input == KEY_DOWN || input == KEY_UP || input == KEY_LEFT || input == KEY_RIGHT) {
            curPtr = (curPtr + 1) % 2;
        } else if (input == '\n' || input == ' ') {
            if (curPtr == 0) {
                ofstream out(path + "scoreboard.csv");
                for (int i = 0; i < (int) csv.size(); ++i)
                    out << csv[i] << "\n";
                out.close();
                attron(A_BLINK);
                mvaddstr(cwin[1].top() - 2, cwin[0].left() + cwin[0].width() / 2 - 1, "DONE!");
                attroff(A_BLINK);
            } else break;
        }
        updateptr(curPtr + 1, 1, 2);
    }

    rectangle(LINES / 3 + 1, 1, LINES - LINES / 3 - 3, COLS - 2).clear(1);
}

/*          *           *           OVERALL         *           *           *           */

CoursesNode::CoursesNode() {
    sys = new System;
    tool = new Tools;
    staffCourse = new StaffCourse;
}

CoursesNode::~CoursesNode() {
    delete sys;
    delete tool;
    delete staffCourse;
}

void CoursesNode::load() {
    rectangle(LINES / 3 + 1, 1, LINES - LINES / 3 - 3, COLS - 2).clear(1);

    vector <string> text {
        "   VIEW COURSES   ",
        "    ADD COURSE    ",
        "       BACK       ",
    };
    rectangle cwin[5];
    cwin[0] = rectangle(LINES / 3 + 5, COLS / 2 - 23, 15, 45);
    cwin[1] = rectangle(cwin[0].top() + 3, cwin[0].left() + cwin[0].width() / 2 - text[0].size() / 2 - 1, 3, text[0].size() + 2);
    cwin[2] = rectangle(cwin[1].bottom() + 1, cwin[1].left(), 3, cwin[1].width());
    cwin[3] = rectangle(cwin[2].bottom() + 1, cwin[1].left(), 3, cwin[1].width());

    auto updateptr = [&](int ptr, int stat, int end) {
        cwin[0].drawEdges();
        for (int i = stat; i <= end; ++i) {
            cwin[i + 1].drawEdges();
            if (i == ptr) attron(A_REVERSE);
            mvaddstr(cwin[i + 1].top() + 1, cwin[i + 1].left() + 1, text[i].c_str());
            if (i == ptr) attroff(A_REVERSE);
        }
        refresh();
    };

    int curPtr = 0;
    while (true) {
        int input = getch();
        if (input == KEY_UP) {
            curPtr = (curPtr - 1 + 3) % 3;
        } else if (input == KEY_DOWN) {
            curPtr = (curPtr + 1) % 3;
        } else if (input == '\n' || input == ' ') {
            if (curPtr == 0) display();
            else if (curPtr == 1) add();
            else break;
        }
        updateptr(curPtr, 0, 2);
    }
    rectangle(LINES / 3 + 1, 1, LINES - LINES / 3 - 3, COLS - 2).clear(1);
}

void CoursesNode::display() {
    rectangle(LINES / 3 + 1, 1, LINES - LINES / 3 - 3, COLS - 2).clear(1);

    rectangle win[2];

    enum COURSE {
        COURSE_FRAME,
        COURSE_BACK_BUTTON
    };

    win[COURSE_FRAME] = rectangle(LINES / 3 + 5, COLS / 2 - 20, 20, 40);
    win[COURSE_BACK_BUTTON] = rectangle(win[COURSE_FRAME].bottom() - 3, 
                                win[COURSE_FRAME].left() + win[COURSE_FRAME].width() / 2 - 8, 3, 16);

    auto updatePtr = [&] (int curPtr, const vector <string> &comps) {
        win[COURSE_FRAME].drawEdges();

        int x = win[COURSE_FRAME].top() + max(1, (int) ((win[COURSE_FRAME].height() - 3) / 2 - comps.size() / 2));
        int y = min((int) (x + comps.size() - 2), (int) (win[COURSE_BACK_BUTTON].top() - 1));
        int stat = 0, end = y - x;
        while (end < comps.size() - 2 && !(stat <= curPtr && curPtr <= end)) stat++, end++;
        for (int i = stat; i <= end; ++i) {
            if (i == curPtr) attron(A_REVERSE);
            for (int k = win[COURSE_FRAME].left() + 3; k < win[COURSE_FRAME].right() - 2; ++k)
                mvaddch(x + i - stat, k, ' ');
            mvaddstr(x + i - stat, win[COURSE_FRAME].left() + win[COURSE_FRAME].width() / 2 - comps[i].size() / 2, comps[i].c_str());
            if (i == curPtr) attroff(A_REVERSE);
        }
        if (curPtr == comps.size() - 1) {
            attron(A_REVERSE);
            mvaddstr(win[COURSE_BACK_BUTTON].top() + 1, win[COURSE_BACK_BUTTON].left() + 1, comps.back().c_str());
            attroff(A_REVERSE);
        } else {
            mvaddstr(win[COURSE_BACK_BUTTON].top() + 1, win[COURSE_BACK_BUTTON].left() + 1, comps.back().c_str());
        }
        refresh();
    };

    int mainPtr = 0, curPtr = 0;
    string curPath = "data/courses/";
    
    vector <string> comps = staffCourse->getData(curPtr, curPath);
    sort(comps.begin(), comps.end(), [&] (string a, string b) {
        if (a.size() != b.size()) return a.size() < b.size();
        for (int i = 0; i < (int) a.size(); ++i) if (a[i] != b[i])
            return a[i] < b[i];
        return true;
    });

    comps.push_back("   [ BACK  ]  ");
    
    while (true) {
        int input = getch();
        if (input == KEY_DOWN) {
            curPtr = (curPtr + 1) % comps.size();
        } else if (input == KEY_UP) {
            curPtr = (curPtr - 1 + comps.size()) % comps.size();
        } else if (input == '\n') {
            if (curPtr == comps.size() - 1) {
                if (mainPtr == 0) break;
                mainPtr--;
                curPath.pop_back();
                while (curPath.back() != '/') curPath.pop_back();
            } else {
                if (mainPtr == 2) {
                    CourseNode nw(curPath + comps[curPtr] + "/");
                    //courseManager(curPath + comps[curPtr] + "/");
                    nw.display();
                } else {
                    curPath += comps[curPtr] + "/";
                    mainPtr++;
                }
            }
            curPtr = 0;
            comps = staffCourse->getData(mainPtr, curPath);
            sort(comps.begin(), comps.end(), [&] (string a, string b) {
                if (a.size() != b.size()) return a.size() < b.size();
                for (int i = 0; i < (int) a.size(); ++i) if (a[i] != b[i])
                    return a[i] < b[i];
                return true;
            });
            comps.push_back("   [ BACK  ]  ");
            rectangle(win[COURSE_FRAME].top() + 1, 
                                    win[COURSE_FRAME].left() + 1,
                                    win[COURSE_FRAME].height() - 2, 
                                    win[COURSE_FRAME].width() - 2).clear(1);
        }
        updatePtr(curPtr, comps);
    }

    rectangle(LINES / 3 + 1, 1, LINES - LINES / 3 - 3, COLS - 2).clear(1);
}

void CoursesNode::add() {
    rectangle(LINES / 3 + 1, 1, LINES - LINES / 3 - 3, COLS - 2).clear(1);
    
    rectangle win(LINES/2 - 3, COLS/2 - 30, 20, 50);

    win.drawEdges();

    vector <string> text {
        "ID", "Name", "LectorAcc", "Start Date", "End Date", "Start Hour", "End Hour", "Date of Week", "Room"
    };

    mvprintw(win.top() + 2, win.left() + win.width() / 2 - 9, "Course Information");
    mvprintw(win.top() + 5, win.left() + 3, "ID");
    mvprintw(win.top() + 6, win.left() + 3, "Name");
    mvprintw(win.top() + 7, win.left() + 3, "LectorAcc");
    mvprintw(win.top() + 8, win.left() + 3, "Start Date");
    mvprintw(win.top() + 9, win.left() + 3, "End Date");
    mvprintw(win.top() + 10, win.left() + 3, "Start Hour");
    mvprintw(win.top() + 11, win.left() + 3, "End Hour");
    mvprintw(win.top() + 12, win.left() + 3, "Date of Week");
    mvprintw(win.top() + 13, win.left() + 3, "Room");

    vector <string> comps(9);
    //for (auto &it : comps) it.clear();

    auto updateptr = [&](int cur) {
        for (int i = 0; i < 9; ++i) {
            if (cur == i) attron(A_REVERSE);
            for (int k = win.left() + 18; k < win.right() - 1; ++k) mvaddch(win.top() + 5 + i, k, ' ');
            if (!comps[i].empty()) 
                mvaddstr(win.top() + 5 + i, win.left() + 18, comps[i].c_str());
            if (cur == i) attroff(A_REVERSE);
        }
        mvaddstr(win.bottom() - 2, win.left() + (int) win.width() / 4 * 0.8 - 4, "   [  ADD  ]   ");
        mvaddstr(win.bottom() - 2, win.left() + (int) win.width() / 4 * 2.8 - 4, "   [ CANCEL ]   ");
        if (cur > 8) {
            cur %= 2;
            attron(A_REVERSE);
            mvaddstr(win.bottom() - 2, win.left() + (int) win.width() / 4 * (cur ? 0.8 : 2.8) - 4, (cur ? "   [  ADD  ]   " : "   [ CANCEL ]   "));
            attroff(A_REVERSE);
        }
        refresh();
    };

    int curptr = 0, lim = 11;

    while (true) {
        int input = getch();
        if (input == KEY_UP || input == KEY_LEFT) {
            curptr = (curptr - 1 + lim) % lim;
        } else if (input == KEY_DOWN || input == KEY_RIGHT) {
            curptr = (curptr + 1) % lim;
        } else if (input == '\n' || input == ' ') {
            if (curptr < 9) {
                attron(A_REVERSE);
                for (int k = win.left() + 18; k < win.right() - 1; ++k) 
                    mvaddch(win.top() + 5 + curptr, k, ' ');
                attroff(A_REVERSE);
                comps[curptr] = sys->getInput(win.top() + 5 + curptr, win.left() + 18);
            } else if (curptr == 9) { // Add course
                //for (int i = 0; i < comps.size(); ++i) mvprintw(1 + i, 4, "%s %d", comps[i].c_str(), comps[i].empty());
                bool ok = true;
                mvaddstr(win.bottom() - 4, win.left() + 1, string(win.width() - 2,  ' ').c_str());
                for (int i = 0; i < comps.size(); ++i) if (comps[i].empty()) {
                    attron(A_BLINK);
                    mvaddstr(win.bottom() - 4, win.left() + win.width() / 2 - 4, ("Invalid " + text[i] + "!").c_str());
                    attroff(A_BLINK);
                    refresh();
                    
                    ok = false;
                    break;
                }
                if (!ok) continue;
                string path = "data/courses/2019/Fall/" + comps[0];
                tool->createFolder(path);
                system(("touch " + path + "/info").c_str());
                system(("touch " + path + "/student_list").c_str());
                system(("touch " + path + "/scoreboard").c_str());
                system(("touch " + path + "/attendance_list").c_str());
                
                ofstream fout(path + "/info");
                fout << comps[2] << "\n";
                fout << comps[3] << "\n";
                fout << comps[0] << "\n";
                fout << comps[1] << "\n";
                fout << comps[4] << "\n";
                fout << comps[8] << "\n";
                fout << comps[5] << "\n";
                fout << comps[6] << "\n";
                fout << comps[7] << "\n";
                fout.close();

                break;
            } else break;
        }
        updateptr(curptr);
    }
    rectangle(LINES / 3 + 1, 1, LINES - LINES / 3 - 3, COLS - 2).clear(1);
}


/*      *       *       *       *       LECTURER COURSE NODE     *       *       *       *       *       */

LectCourseNode::LectCourseNode(string path) { 
    this->path = path; 
   
    tool = new Tools;
    sys = new System;
    staffClass = new StaffClass;
}

LectCourseNode::~LectCourseNode() {
    delete sys;
    delete tool;
    delete staffClass;
}

void LectCourseNode::display() {
    rectangle(LINES / 3 + 1, 1, LINES - LINES / 3 - 3, COLS - 2).clear(1);

    enum COURSE {
        INFO,
        STUDENT,
        ATTENDANCE,
        SCOREBOARD,
        BACK
    };

    const vector <string> ss {
        "COURSE INFO",
        "STUDENT LIST",
        "ATTENDANCE LIST",
        "SCOREBOARD",
        "BACK"
    };

    rectangle cwin[10];
    cwin[INFO] = rectangle(LINES / 3 + 6, COLS / 2 - 16, 3, 27);
    cwin[STUDENT] = rectangle(cwin[INFO].bottom() + 1, COLS / 2 - 16, 3, 27);
    cwin[ATTENDANCE] = rectangle(cwin[STUDENT].bottom() + 1, COLS / 2 - 16, 3, 27);
    cwin[SCOREBOARD] = rectangle(cwin[ATTENDANCE].bottom() + 1, COLS / 2 - 16, 3, 27);
    cwin[BACK] = rectangle(cwin[SCOREBOARD].bottom() + 1, COLS / 2 - 16, 3, 27);
    
    auto updateptr = [&](int ptr, int stat, int end) {
        for (int i = stat; i <= end; ++i) {
                cwin[i].drawEdges();
                if (i == ptr) attron(A_REVERSE);
                for (int j = cwin[i].left() + 1; j < cwin[i].right(); ++j)
                    mvaddch(cwin[i].top() + 1, j, ' ');
                int y = cwin[i].left() + cwin[i].width() / 2 - ss[i].size() / 2;
                mvaddstr(cwin[i].top() + 1, y, ss[i].c_str());
                if (i == ptr) attroff(A_REVERSE);
            }
            refresh();
    };

    int curPtr = 0, lim = ss.size();
    while (true) {
        int input = getch();
        if (input == KEY_DOWN) {
            curPtr = (curPtr + 1) % lim;
        } else if (input == KEY_UP) {
            curPtr = (curPtr - 1 + lim) % lim;
        } else if (input == '\n' || input == ' ') {
            if (curPtr == INFO) displayInfo();
            else if (curPtr == ATTENDANCE) displayAttend();
            else if (curPtr == SCOREBOARD) displayScoreboard();
            else if (curPtr == STUDENT) displayStudent();
            else break;
        }
        updateptr(curPtr, 0, lim - 1);
    }
    rectangle(LINES / 3 + 1, 1, LINES - LINES / 3 - 3, COLS - 2).clear(1);
}

void LectCourseNode::displayInfo() {
    rectangle(LINES / 3 + 1, 1, LINES - LINES / 3 - 3, COLS - 2).clear(1);
    
    //lecturer account[0], name[1], start[2], end[3], day[4], shour[5], ehour[6], room[7]
    vector <string> text {
        "ID          :",
        "Name        :",
        "Start date: ",
        "End date    :",
        "Day of week :",
        "Start Hour  :",
        "End Hour    :",
        "Room        :"
    };
    vector <string> cur = tool->getContent(path + "/info");

    enum INFO {
        FRAME,
        SAVE_BUTTON,
        CANCEL_BUTTON
    };
    rectangle cwin[3];
    cwin[FRAME] = rectangle(LINES / 3 + 6, COLS / 2 - 25, 15, 50);
    cwin[SAVE_BUTTON] = rectangle(cwin[FRAME].bottom() - 1, cwin[FRAME].left() + cwin[FRAME].width() / 4 - 6, 3, 12);
    cwin[CANCEL_BUTTON] = rectangle(cwin[SAVE_BUTTON].top(), cwin[FRAME].left() + cwin[FRAME].width() * 3 / 4 - 6, 3, 12);
    
    cwin[FRAME].drawEdges();

    auto updateptr = [&] (int curPtr) {
        int tmp = (curPtr - text.size()) % 2;
        if (curPtr >= cur.size()) {
            if (!(tmp % 2)) {
                attron(A_REVERSE);
                mvaddstr(cwin[CANCEL_BUTTON].top() + 1, cwin[CANCEL_BUTTON].left() + 1, "  CANCEL  ");
                attroff(A_REVERSE);
                mvaddstr(cwin[SAVE_BUTTON].top() + 1, cwin[SAVE_BUTTON].left() + 1, "   SAVE   ");
            } else {
                mvaddstr(cwin[CANCEL_BUTTON].top() + 1, cwin[CANCEL_BUTTON].left() + 1, "  CANCEL  ");
                attron(A_REVERSE);
                mvaddstr(cwin[SAVE_BUTTON].top() + 1, cwin[SAVE_BUTTON].left() + 1, "   SAVE   ");
                attroff(A_REVERSE);
            }
        } else {
            mvaddstr(cwin[CANCEL_BUTTON].top() + 1, cwin[CANCEL_BUTTON].left() + 1, "  [ CANCEL ]  ");
            mvaddstr(cwin[SAVE_BUTTON].top() + 1, cwin[SAVE_BUTTON].left() + 1, "   [SAVE ]   ");
        }
        
        for (int i = 1; i < cur.size(); ++i) {
            mvaddstr(cwin[FRAME].top() + 1 + i, cwin[FRAME].left() + 3, text[i - 1].c_str());
            if (curPtr == i) attron(A_REVERSE);
            for (int y = cwin[FRAME].left() + cwin[FRAME].width() / 2; y < cwin[FRAME].right() - 2; ++y)
                mvaddch(cwin[FRAME].top() + 1 + i, y, ' ');
            mvaddstr(cwin[FRAME].top() + 1 + i, cwin[FRAME].left() + cwin[FRAME].width() / 2 + 2, cur[i].c_str());
            if (curPtr == i) attroff(A_REVERSE);
        }
    };

    int curPtr = 0, lim = cur.size() + 1;
    while (true) {
        int input = getch();
        if (input == KEY_UP || input == KEY_LEFT) {
            curPtr = (curPtr - 1 + lim) % lim;
        } else if (input == KEY_DOWN || input == KEY_RIGHT) {
            curPtr = (curPtr + 1) % lim;
        } else if (input == '\n' || input == ' ') {
            if (curPtr < cur.size() - 1) {
                attron(A_REVERSE);
                for (int y = cwin[FRAME].left() + cwin[FRAME].width() / 2; y < cwin[FRAME].right() - 2; ++y)
                    mvaddch(cwin[FRAME].top() + 2 + curPtr, y, ' ');
                attroff(A_REVERSE);
                string tex = sys->getInput(cwin[FRAME].top() + 2 + curPtr, cwin[FRAME].left() + cwin[FRAME].width() / 2 + 2);
                cur[curPtr + 1] = tex;
            } else  {
                if (curPtr < cur.size()) sys->updateData(path + "/info", cur);
                break;
            }
        }
        updateptr(curPtr + 1);
    }
     
    rectangle(LINES / 3 + 1, 1, LINES - LINES / 3 - 3, COLS - 2).clear(1);
}

void LectCourseNode::displayStudent() {
    rectangle(LINES/3 + 1, 1, LINES - LINES/3 - 3, COLS - 2).clear(1);

    rectangle cwin;
    cwin = rectangle(LINES/2 - 6, COLS/2 - 36, 24, 80);
    cwin.drawEdges();

    //mvprintw(LINES/2 - 5, COLS/2 - 3, classX.c_str());
    mvprintw(LINES/2 - 2, COLS/2 - 27 , "ID      First name    Last name     Gender         DoB");
    
    //Get information of students in class X
    //system(("echo " + path + " >> abc.log").c_str());
    vector <string> studentList = tool->getContent(path + "/student_list");

    vector <StudentNode> student;
    
    for (int i = 0; i < studentList.size(); ++i) {
        string classX = tool->getClass(studentList[i]);
        system(("echo " + classX + " " + studentList[i] + " >> abc.log").c_str());
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
                    /*isEdit = false;
                    
                    StudentNode studentSub("", "", "", "", "", "");
                    studentSub.editInfo(isRemove, isEdit, 1, 1);
                    student.push_back(studentSub);
                    ++size;*/
                }
                else {
                    isEdit = true;
                    student[curPtr].editInfo(isRemove,isEdit, true, true);
                    if (isRemove) {
                        studentList.erase(studentList.begin() + curPtr);
                        student.erase(student.begin() + curPtr);
                        sys->updateData(path + "/student_list", studentList);
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

void LectCourseNode::displayAttend() {
    rectangle(LINES / 3 + 1, 1, LINES - LINES / 3 - 3, COLS - 2).clear(1);
    
    rectangle cwin[3];
    cwin[0] = rectangle(LINES / 3 + 6, COLS / 2 - 25, 20, 50);
    cwin[1] = rectangle(cwin[0].bottom() - 1, cwin[0].left() + cwin[0].width() / 4 - 8, 3, 16);
    cwin[2] = rectangle(cwin[0].bottom() - 1, cwin[0].left() + cwin[0].width() * 3 / 4 - 8, 3, 16);

    cwin[0].drawEdges();

    mvaddstr(cwin[0].top() + 1, cwin[0].left() + cwin[0].width() / 4 - 5, "Student ID");
    mvaddstr(cwin[0].top() + 1, cwin[0].left() + cwin[0].width() * 3 / 4 - 5, "Attendance");

    vector <string> studentList;
    vector < vector <int> > comps;

    ifstream fin[2];
    fin[0].open(path + "/student_list");
    fin[1].open(path + "/attendance_list");

    vector <string> csv; csv.push_back("No,Student ID,Attendance");

    string id, classname;
    int dx = cwin[0].top() + 3, cnt = 0;
    while (fin[0] >> id) {
        mvaddstr(dx, cwin[0].left() + cwin[0].width() / 4 - id.size() / 2, id.c_str());
        
        vector <int> cur(10);
        for (int i = 0; i < 10; ++i) fin[1] >> cur[i];
        
        int x = accumulate(cur.begin(), cur.end(), 0), y = 10;
        
        comps.push_back(cur);
        studentList.push_back(id);
        csv.push_back(to_string(++cnt) + "," + id + "," + to_string(x) + "/" + to_string(y));
    }

    vector <string> buttons {
        "[  BACK  ]"
    };
    
    auto updateptr = [&](int ptr) {
        cwin[0].drawEdges();
        for (int i = 0; i < comps.size(); ++i) {
            if (i == ptr) attron(A_REVERSE);
            for (int k = cwin[0].left() + 1; k < cwin[0].right(); ++k) {
                mvaddch(cwin[0].top() + 3 + i, k, ' ');
            }
            mvprintw(cwin[0].top() + 3 + i, cwin[0].left() + cwin[0].width() / 4 - studentList[i].size() / 2, "%s", studentList[i].c_str());
            mvprintw(cwin[0].top() + 3 + i, cwin[0].left() + cwin[0].width() / 4 * 3 - 2, "%d / %d", accumulate(comps[i].begin(), comps[i].end(), 0), 10);

            if (i == ptr) attroff(A_REVERSE);
        }
        ptr -= comps.size();
        for (int i = 0; i < buttons.size(); ++i) {
            if (i == ptr) attron(A_REVERSE);
            mvaddstr(cwin[0].bottom(), cwin[0].left() + cwin[0].width() / (buttons.size() + 1) * (i + 1) - buttons[i].size() / 2, buttons[i].c_str());
            if (i == ptr) attroff(A_REVERSE);
        }
        refresh();
    };

    int cur = 0, lim = comps.size() + buttons.size();
    while (true) {
        int input = getch();
        if (input == KEY_UP || input == KEY_LEFT) {
            cur = (cur - 1 + lim) % lim;
        } else if (input == KEY_DOWN || input == KEY_RIGHT) {
            cur = (cur + 1) % lim;
        } else if (input == ' ' || input == '\n') {
            if (cur == lim - 1 ) break;
            else if (cur < comps.size()) {
                editAttendance(cur, comps[cur]);
                ofstream fout(path + "/attendance_list");
                for (auto it : comps) {
                    for (auto jt : it) fout << jt << " ";
                    fout << "\n";
                }
                fout.close();
            }
        }
        updateptr(cur);
    }

    rectangle(LINES / 3 + 1, 1, LINES - LINES / 3 - 3, COLS - 2).clear(1);
}

void LectCourseNode::editAttendance(int pos, vector <int> &att) {
    rectangle(LINES / 3 + 1, 1, LINES - LINES / 3 - 3, COLS - 2).clear(1);

    rectangle frame(LINES / 3 + 1, COLS / 2 - 10, 3 * 10 + 2 + 2, 25);
    frame.drawEdges();
    vector <rectangle> cwin;
    cwin.push_back(rectangle(frame.top() + 1, frame.left() + 8, 3, 10));
    for (int i = 1; i < 10; ++i) {
        rectangle cur(cwin.back().bottom() + 1, cwin.back().left(), cwin.back().height(), cwin.back().width());
        cwin.push_back(cur);
    }
    
    vector <rectangle> bwin(2);
    bwin[0] = rectangle(frame.bottom() - 1, frame.left() + frame.width() / 3 - 2, 3, 6);
    bwin[1] = rectangle(frame.bottom() - 1, frame.left() + frame.width() / 3 * 2 - 2, 3, 6);
    vector <string> comps;
    for (int i = 0; i < att.size(); ++i) comps.push_back(!att[i] ? " " : "x");
    vector <string> buttons {
        "SAVE",
        "BACK"
    };
    Windows *xwin = new Windows;
    xwin->setComponents(comps);
    xwin->setButtons(buttons);
    xwin->setCFrame(cwin);
    
    int cur = 0, lim = comps.size() + buttons.size();
    while (true) {
        int input = getch();
        if (input == KEY_UP || input == KEY_LEFT) {
            cur = (cur - 1 + lim) % lim;
        } else if (input == KEY_DOWN || input == KEY_RIGHT) {
            cur = (cur + 1) % lim;
        } else if (input == ' ' || input == '\n') {
            if (cur == lim - 1 ) break;
            else if (cur == lim - 2) {
                for (int i = 0; i < att.size(); ++i) att[i] = comps[i] == "x";
                break;
            } else {
                System *sys = new System;
                comps[cur] = sys->getInput(cwin[cur].top() + 1, cwin[cur].left() + 1, 0);
                xwin->setComponents(comps);
                delete sys;
            }
        }
        xwin->updatePtr(cur);
    }
    for (int i = 0; i < att.size(); ++i) att[i] = comps[i] == "x";
    delete xwin;
    rectangle(LINES / 3 + 1, 1, LINES - LINES / 3 - 3, COLS - 2).clear(1);
}


void LectCourseNode::displayScoreboard(string acc) {
    rectangle(LINES / 3 + 1, 1, LINES - LINES / 3 - 3, COLS - 2).clear(1);
    
    rectangle cwin[6];
    cwin[0] = rectangle(LINES / 3 + 6, COLS / 2 - 25, 20, 50);
    cwin[1] = rectangle(cwin[0].bottom() - 1, cwin[0].left() + cwin[0].width() / 12 - 4, 3, 13);
    cwin[2] = rectangle(cwin[0].bottom() - 1, cwin[0].left() + cwin[0].width() * 4 / 12 - 4, 3, 13);
    cwin[3] = rectangle(cwin[0].bottom() - 1, cwin[0].left() + cwin[0].width() * 6 / 12 - 4, 3, 13);
    cwin[4] = rectangle(cwin[0].bottom() - 1, cwin[0].left() + cwin[0].width() * 8 / 12 - 4, 3, 13);
    cwin[5] = rectangle(cwin[0].bottom() - 1, cwin[0].left() + cwin[0].width() * 10 / 12 - 4, 3, 13);


    cwin[0].drawEdges();

    mvaddstr(cwin[0].top() + 1, cwin[0].left() + cwin[0].width() / 12 - 2, "Student ID");
    mvaddstr(cwin[0].top() + 1, cwin[0].left() + cwin[0].width() * 4 / 12 - 2, "Midterm");
    mvaddstr(cwin[0].top() + 1, cwin[0].left() + cwin[0].width() * 6 / 12 - 2, "Final");
    mvaddstr(cwin[0].top() + 1, cwin[0].left() + cwin[0].width() * 8 / 12 - 2, "Lab");
    mvaddstr(cwin[0].top() + 1, cwin[0].left() + cwin[0].width() * 10 / 12 - 2, "Bonus");


    vector <string> studentList;
    vector < vector <double> > comps;

    ifstream fin[2];
    fin[0].open(path + "/student_list");
    fin[1].open(path + "/scoreboard");

    vector <string> csv; csv.push_back("No,Student ID,Midterm,Final,Lab,Bonus");

    string id;
    double mid,final,lab,bonus  ;
    int dx = cwin[0].top() + 3, cnt = 0;
    while (fin[0] >> id) {
        mvaddstr(dx, cwin[0].left() + cwin[0].width() / 4 - id.size() / 2, id.c_str());
        vector <double> cur(4);
        for (int i = 0; i < 4; ++i) fin[1] >> cur[i];
        comps.push_back(cur);
        studentList.push_back(id);
        csv.push_back(id + "," + to_string(cur[0])+","+to_string(cur[1])+","+to_string(cur[2])+","+to_string(cur[3]));
    }
    vector <string> buttons {
        "[  IMPORT  ]",
        "[  BACK  ]"
    };
    
    auto updateptr = [&](int ptr) {
        cwin[0].drawEdges();
        for (int i = 0; i < comps.size(); ++i) {
            if (i == ptr) attron(A_REVERSE);
            for (int k = cwin[0].left() + 1; k < cwin[0].right(); ++k) {
                mvaddch(cwin[0].top() + 3 + i, k, ' ');
            }
            mvprintw(cwin[0].top() + 3 + i, cwin[0].left() + cwin[0].width() / 4 - studentList[i].size() / 5 -8, "%s", studentList[i].c_str());
            mvprintw(cwin[0].top() + 3 + i, cwin[0].left() + cwin[0].width() / 5 * 3 - 14, "%.1f",comps[i][0]);
            mvprintw(cwin[0].top() + 3 + i, cwin[0].left() + cwin[0].width() / 5 * 2+4 , "%.1f",comps[i][1]);
            mvprintw(cwin[0].top() + 3 + i, cwin[0].left() + cwin[0].width() / 5 * 3+1 , "%.1f",comps[i][2]);
            mvprintw(cwin[0].top() + 3 + i, cwin[0].left() + cwin[0].width() *4/ 5  , "%.1f",comps[i][3]);
            if (i == ptr) attroff(A_REVERSE);
        }
        ptr -= comps.size();
        for (int i = 0; i < buttons.size(); ++i) {
            if (i == ptr) attron(A_REVERSE);
            mvaddstr(cwin[0].bottom(), cwin[0].left() + cwin[0].width() / (buttons.size() + 1) * (i + 1) - buttons[i].size() / 2, buttons[i].c_str());
            if (i == ptr) attroff(A_REVERSE);
        }
        refresh();
    };

    int cur = 0, lim = comps.size() + buttons.size();
    while (true) {
        int input = getch();
        if (input == KEY_UP || input == KEY_LEFT) {
            cur = (cur - 1 + lim) % lim;
        } else if (input == KEY_DOWN || input == KEY_RIGHT) {
            cur = (cur + 1) % lim;
        } else if (input == ' ' || input == '\n') {
            if (cur < comps.size())  {
                editScoreboard();
            } else if (cur == comps.size()) {
                vector <string> xxx = tool->getElement(path + "/", '/');
                int k = xxx.size();
                string yyy = xxx[k - 3] + "-" + xxx[k - 2] + "-" + xxx[k - 1];
                ifstream fin("data/lecturers/" + acc + "/" + yyy + "/" + "scoreboard.csv");
                string ss; getline(fin, ss);
                ofstream fout[2];
                fout[0].open(path + "/student_list");
                fout[1].open(path + "/scoreboard");
                while (getline(fin, ss)) {
                    xxx = tool->getElement(ss + ',', ',');
                    fout[0] << xxx[1] << "\n";
                    for (int i = 2; i < xxx.size(); ++i) fout[1] << stoi(xxx[i].c_str()) << " ";
                    fout[1] << "\n";
                }
                fout[0].close(); fout[1].close();

                attron(A_BLINK);
                mvaddstr(cwin[0].bottom() - 2, cwin[0].left() + cwin[0].width() / 2 - 2, "DONE!");
                attroff(A_BLINK);
            } else if (cur == lim - 1) break;
        }
        updateptr(cur);
    }

    rectangle(LINES / 3 + 1, 1, LINES - LINES / 3 - 3, COLS - 2).clear(1);
}
void LectCourseNode::editScoreboard(){
/*
    rectangle cwin[2];
    cwin[0] = rectangle(LINES/2 - 1, COLS/2 - 20, 18, 39);

    cwin[0].drawEdges();
    mvprintw(cwin[0].top() + 2, COLS/2 - 8, "Edit Information");
    mvprintw(cwin[0].top() + 5, cwin[0].left() + 3, "Student ID");
    mvprintw(cwin[0].top() + 6, cwin[0].left() + 3, "Mid term");
    mvprintw(cwin[0].top() + 7, cwin[0].left() + 3, "Final");
    mvprintw(cwin[0].top() + 8, cwin[0].left() + 3, "Lab");
    mvprintw(cwin[0].top() + 9, cwin[0].left() + 3, "Bonus");
    
    vector <rectangle> comps;
    vector <string> buttons;

    auto updateptr = [&](int ptr) {
        cwin[0].drawEdges();
        for (int i = 0; i < comps.size(); ++i) {
            if (i == ptr) attron(A_REVERSE);
            for (int k = cwin[0].left() + 1; k < cwin[0].right(); ++k) {
                mvaddch(cwin[0].top() + 3 + i, k, ' ');
            }
            mvprintw(cwin[0].top() + 3 + i, cwin[0].left() + cwin[0].width() / 4 - studentList[i].size() / 5 -8, "%s", studentList[i].c_str());
            mvprintw(cwin[0].top() + 3 + i, cwin[0].left() + cwin[0].width() / 5 * 3 - 14, "%.1f",comps[i][0]);
            mvprintw(cwin[0].top() + 3 + i, cwin[0].left() + cwin[0].width() / 5 * 2+4 , "%.1f",comps[i][1]);
            mvprintw(cwin[0].top() + 3 + i, cwin[0].left() + cwin[0].width() / 5 * 3+1 , "%.1f",comps[i][2]);
            mvprintw(cwin[0].top() + 3 + i, cwin[0].left() + cwin[0].width() *4/ 5  , "%.1f",comps[i][4]);
            if (i == ptr) attroff(A_REVERSE);
        }
        ptr -= comps.size();
        for (int i = 0; i < buttons.size(); ++i) {
            if (i == ptr) attron(A_REVERSE);
            mvaddstr(cwin[0].bottom(), cwin[0].left() + cwin[0].width() / (buttons.size() + 1) * (i + 1) - buttons[i].size() / 2, buttons[i].c_str());
            if (i == ptr) attroff(A_REVERSE);
        }
        refresh();
    };

    int cur = 0, lim = 0;
    int input = getch();
        if (input == KEY_UP || input == KEY_LEFT) {
            cur = (cur - 1 + lim) % lim;
        } else if (input == KEY_DOWN || input == KEY_RIGHT) {
            cur = (cur + 1) % lim;
        } else if (input == ' ' || input == '\n') {
            if (cur < comps.size())  {
                editScoreboard();
            } else if (cur == comps.size()) {
                
        updateptr(cur);
    }

    rectangle(LINES / 3 + 1, 1, LINES - LINES / 3 - 3, COLS - 2).clear(1);
*/}
