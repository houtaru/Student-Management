//
//  student_window.cpp
//  Student Management
//
//  Created by Khoa Nguyen on 4/10/19.
//  Copyright © 2019 Khoa Nguyen. All rights reserved.
//

#include "student_window.hpp"


StudentWindow::StudentWindow() {}

StudentWindow::~StudentWindow() {}

void StudentWindow::load(string ID, string Class) {
    rectangle(LINES / 3 + 1, 1, LINES - LINES / 3 - 3, COLS - 2).clear(1);
    
    
    //Get name of that student
    ifstream studentInfo("data/classes/" + Class + "/" + ID);
    string firstName, lastName;
    getline(studentInfo, firstName);
    getline(studentInfo, lastName);
    studentInfo.close();
    
    
    //Display options for that student
    rectangle winStudent, win[6];
    winStudent = rectangle(LINES/3 + 3, COLS/2 - 35, 27, 70);
    win[PROFILE] = rectangle(winStudent.top() + 6, COLS/2 - maincomp[0].size()/2 - 3, 3, maincomp[0].size() + 4);
    win[CHECKIN] = rectangle(win[PROFILE].bottom() + 1, win[PROFILE].left(), 3, maincomp[0].size() + 4);
    win[SCHEDULE] = rectangle(win[CHECKIN].bottom() + 1, win[CHECKIN].left(), 3, maincomp[0].size() + 4);
    win[SCOREBOARD] = rectangle(win[SCHEDULE].bottom() + 1, win[SCHEDULE].left(), 3, maincomp[0].size() + 4);
    win[CHANGEPASSWORD] = rectangle(winStudent.bottom() - 4, winStudent.left() + 1, 3, maincomp[CHANGEPASSWORD].size() + 4);
    win[LOGOUT] = rectangle(winStudent.bottom() - 4, winStudent.right() - (maincomp[LOGOUT].size() + 4) - 1, 3, maincomp[LOGOUT].size() + 4);
    
    
    int curPtr = 0, size = maincomp.size();
    while (true) {
        int input = getch();
        bool exitWhile = false;
        
        switch (input) {
            case KEY_DOWN:
                curPtr = (curPtr + 1) % size;
                break;
            case KEY_UP:
                curPtr = (curPtr + size - 1) % size;
                break;
            case '\n': {
                if (curPtr == LOGOUT) {
                    exitWhile = true;
                    break;
                }
                    
                switch (curPtr) {
                    case PROFILE:
                        displayProfile(ID, Class);
                        break;
                    case CHECKIN:
                        displayCheckIn(ID, Class);
                        break;
                    case SCHEDULE:
                        displaySchedule(ID, Class);
                        break;
                    case SCOREBOARD:
                        displayScoreboard(ID, Class);
                        break;
                    case CHANGEPASSWORD:
                        changePassword(ID);
                        break;
                }
            }
        }
        
        // If user choose LOG OUT, finish this function
        if (exitWhile)
            break;
        
        // Update pointer
        winStudent.drawEdges();
        //attron(A_BOLD);
        attron(COLOR_PAIR(2));
        string welcome = "WELCOME, " + firstName + " " + lastName;
        attroff(COLOR_PAIR(2));
        //attroff(A_BOLD);
        mvprintw(winStudent.top() + 3, winStudent.right() - 3 - welcome.size(), (welcome).c_str());
        for (int i = 0; i < size - 2; ++i)
            win[i].drawEdges();
        for (int i = 0; i < size; ++i) {
            if (i == curPtr)
                attron(A_REVERSE);
            if (i == CHANGEPASSWORD)
                mvprintw(win[CHANGEPASSWORD].top() + 1, win[CHANGEPASSWORD].left() + 2, maincomp[CHANGEPASSWORD].c_str());
            else {
                if (i == LOGOUT)
                    mvprintw(win[LOGOUT].top() + 1, win[LOGOUT].left() + 2, maincomp[LOGOUT].c_str());
                else
                    mvprintw(win[i].top() + 1, win[i].left() + 2, maincomp[i].c_str());
            }
            
            if (i == curPtr)
                attroff(A_REVERSE);
        }
        refresh();
    }
    
    rectangle(LINES / 3 + 1, 1, LINES - LINES / 3 - 3, COLS - 2).clear(1);
}


void StudentWindow::displayProfile(string ID, string Class) {
    rectangle(LINES/2 - 1, COLS/2 - 18, 16, 37).clear(1);
    rectangle winInfo(LINES/2 - 1, COLS/2 - 18, 16, 37);
    winInfo.drawEdges();
    
    
    //Get information of that student
    ifstream studentInfo("data/classes/" + Class + "/" + ID);
    string firstName, lastName, gender, DoB;
    getline(studentInfo, firstName);
    getline(studentInfo, lastName);
    getline(studentInfo, gender);
    getline(studentInfo, DoB);
    studentInfo.close();
    
    
    //Display information onto the screen
    //attron(A_BOLD);
    attron(COLOR_PAIR(2));
    mvprintw(winInfo.top() + 2, COLS/2 - 5, "Information");
    attroff(COLOR_PAIR(2));
    //attroff(A_BOLD);
    mvprintw(winInfo.top() + 5, winInfo.left() + 6, ("ID             " + ID).c_str());
    mvprintw(winInfo.top() + 6, winInfo.left() + 6, ("First name     " + firstName).c_str());
    mvprintw(winInfo.top() + 7, winInfo.left() + 6, ("Last name      " + lastName).c_str());
    mvprintw(winInfo.top() + 8, winInfo.left() + 6, ("Gender         " + gender).c_str());
    mvprintw(winInfo.top() + 9, winInfo.left() + 6, ("DoB            " + DoB).c_str());
    mvprintw(winInfo.top() + 10, winInfo.left() + 6, ("Class          " + Class).c_str());
    attron(A_REVERSE);
    mvprintw(winInfo.bottom() - 2, COLS/2 - 5, "  [BACK]  ");
    attroff(A_REVERSE);
    
    
    while (true) {
        int input = getch();
        if (input == '\n')
            break;
    }
    
    
    rectangle(LINES / 3 + 1, 1, LINES - LINES / 3 - 3, COLS - 2).clear(1);
    refresh();
}


struct score {
    double Midterm, Final, Lab, Bonus;
};

void StudentWindow::displayScoreboard(string ID, string Class) {
    rectangle(LINES / 3 + 1, 1, LINES - LINES / 3 - 3, COLS - 2).clear(1);
    

    rectangle winScoreboard(LINES/3 + 3, COLS/2 - 35, 27, 70);
    winScoreboard.drawEdges();
    
    
    //Print name, ID and class of that student on to the screen
    string firstName, lastName;
    ifstream studentInfoFile("data/classes/" + Class + "/" + ID);
    getline(studentInfoFile, firstName);
    getline(studentInfoFile, lastName);
    studentInfoFile.close();
    
    int sizeName = (firstName + " " + lastName).size(), sizeIDClass = (ID + " - " + Class).size();
    //attron(A_BOLD);
    mvprintw(winScoreboard.top() + 4, COLS/2 - sizeName/2, (firstName + " " + lastName).c_str());
    mvprintw(winScoreboard.top() + 5, COLS/2 - sizeIDClass/2, (ID + " - " + Class).c_str());
    //attroff(A_BOLD);
    
    attron(COLOR_PAIR(2));
    mvprintw(winScoreboard.top() + 8, winScoreboard.left() + 7, "Course");
    mvprintw(winScoreboard.top() + 8, winScoreboard.right() - 12, "Bonus");
    mvprintw(winScoreboard.top() + 8, winScoreboard.right() - 20, "Lab");
    mvprintw(winScoreboard.top() + 8, winScoreboard.right() - 30, "Final");
    mvprintw(winScoreboard.top() + 8, winScoreboard.right() - 42, "Midterm");
    attroff(COLOR_PAIR(2));
    
    
    attron(A_REVERSE);
    mvprintw(winScoreboard.bottom() - 2, COLS/2 - 5, "  [BACK]  ");
    attroff(A_REVERSE);
    
    
    string stuff;
    string courseLink;
    
    //Get link to the courses in which that student enrolls
    studentInfoFile.open("data/classes/" + Class + "/" + ID);
    for (int i = 0; i < 4; ++i)
        getline(studentInfoFile,stuff);
    
    int count = 0;
    while (getline(studentInfoFile,courseLink)) {
        //Get course name
        string courseName;
        ifstream courseInfo(courseLink + "/info");
        getline(courseInfo, stuff);
        getline(courseInfo, courseName);
        courseInfo.close();
        
        //Get score from that course
        score scoreCourse;
        //Find index of that student in that course
        int index = 0;
        ifstream studentList(courseLink + "/student_list");
        string IDCompare;
        while (getline(studentList,IDCompare)) {
            if (IDCompare == ID)
                break;
            ++index;
        }
        studentList.close();
        
        ifstream scoreboard(courseLink + "/scoreboard");
        for (int i = 0; i < index; ++i)
            getline(scoreboard,stuff);
        scoreboard >> scoreCourse.Midterm >> scoreCourse.Final >> scoreCourse.Lab >> scoreCourse.Bonus;
        scoreboard.close();
        
        //Display score of that course onto screen
        mvprintw(winScoreboard.top() + 9+count, winScoreboard.left() + 7, courseName.c_str());
        mvprintw(winScoreboard.top() + 9+count, winScoreboard.right() - 11, "%.1f", scoreCourse.Bonus);
        mvprintw(winScoreboard.top() + 9+count, winScoreboard.right() - 20, "%.1f", scoreCourse.Lab);
        mvprintw(winScoreboard.top() + 9+count, winScoreboard.right() - 29, "%.1f", scoreCourse.Final);
        mvprintw(winScoreboard.top() + 9+count, winScoreboard.right() - 41, "%.1f", scoreCourse.Midterm);
        
        ++count;
    }
    studentInfoFile.close();
    
    
    while (true) {
        int input = getch();
        if (input == '\n')
            break;
    }
    
    
    rectangle(LINES / 3 + 1, 1, LINES - LINES / 3 - 3, COLS - 2).clear(1);
    refresh();
}


const vector <string> scheduleDate {
    "Mon",
    "Tue",
    "Wed",
    "Thu",
    "Fri",
    "Sat"
};

const vector <string> scheduleTime {
    "7:30",
    "-9:10",
    "9:30",
    "-11:10",
    "13:30",
    "-15:10",
    "15:30",
    "-17:10"
};

struct courseSchedule {
    string Name, Date, Time, Room;
};

void StudentWindow::displaySchedule(string ID, string Class) {
    rectangle(LINES / 3 + 1, 1, LINES - LINES / 3 - 3, COLS - 2).clear(1);
    
    
    //Print name, ID and class of that student on to the screen
    string firstName, lastName;
    ifstream studentInfoFile("data/classes/" + Class + "/" + ID);
    getline(studentInfoFile, firstName);
    getline(studentInfoFile, lastName);
    studentInfoFile.close();
    
    int sizeName = (firstName + " " + lastName).size(), sizeIDClass = (ID + " - " + Class).size();
    //attron(A_BOLD);
    mvprintw(LINES/3 + 3, COLS/2 - sizeName/2, (firstName + " " + lastName).c_str());
    mvprintw(LINES/3 + 4, COLS/2 - sizeIDClass/2, (ID + " - " + Class).c_str());
    //attroff(A_BOLD);
    
    
    attron(A_REVERSE);
    mvprintw(LINES - 6, COLS/2 - 4, "  [BACK]  ");
    attroff(A_REVERSE);
    
    
    //Draw table
    mvaddch(LINES/3 + 7, COLS/2 - 49, ACS_ULCORNER);
    mvaddch(LINES/3 + 7, COLS/2 + 49, ACS_URCORNER);
    mvaddch(LINES/3 + 7+16, COLS/2 - 49, ACS_LLCORNER);
    mvaddch(LINES/3 + 7+16, COLS/2 + 49, ACS_LRCORNER);
    
    for (int i = COLS/2 - 49; i <= COLS/2 + 49; i += 14)
        for (int j = LINES/3 + 8; j < LINES/3 + 7+16; ++j) {
            if ((j == LINES/3 + 9 || j == LINES/3 + 12 ||
                j == LINES/3 + 15 || j == LINES/3 + 17 || j == LINES/3 + 20) && (i != COLS/2 - 49 && i != COLS/2 + 49))
                mvaddch(j, i, ACS_PLUS);
            else
                mvaddch(j, i, ACS_VLINE);
        }
    
    for (int i = LINES/3 + 7; i <= LINES/3 + 7+16; ++i) {
        if (i == LINES/3 + 7 || i == LINES/3 + 9 ||
            i == LINES/3 + 12 || i == LINES/3 + 15 ||
            i == LINES/3 + 17 || i == LINES/3 + 20 ||
            i == LINES/3 + 23)
            for (int j = COLS/2 - 48; j < COLS/2 + 49; ++j)
                if (j != COLS/2 - 35 && j != COLS/2 - 21 &&
                    j != COLS/2 - 7 && j != COLS/2 + 7 &&
                    j != COLS/2 + 21 && j != COLS/2 + 35)
                    mvaddch(i, j, ACS_HLINE);
    }
    
    for (int i = COLS/2 - 35; i <= COLS/2 + 35; i += 14) {
        mvaddch(LINES/3 + 7, i, ACS_TTEE);
        mvaddch(LINES/3 + 7+16, i, ACS_BTEE);
    }
    
    for (int i = LINES/3 + 9; i <= LINES/3 + 20; ++i) {
        if (i == LINES/3 + 9 ||
            i == LINES/3 + 12 || i == LINES/3 + 15 ||
            i == LINES/3 + 17 || i == LINES/3 + 20) {
            mvaddch(i, COLS/2 - 49, ACS_LTEE);
            mvaddch(i, COLS/2 + 49, ACS_RTEE);
        }
    }
    
    
    //Display date and time onto screen
    attron(COLOR_PAIR(2));
    for (int i = 0; i < scheduleDate.size(); ++i)
    mvprintw(LINES/3 + 8, COLS/2 - 35+6+i*14, scheduleDate[i].c_str());
    
    mvprintw(LINES/3 + 10, COLS/2 - 49+5, scheduleTime[0].c_str());
    mvprintw(LINES/3 + 11, COLS/2 - 49+5, scheduleTime[1].c_str());
    
    mvprintw(LINES/3 + 13, COLS/2 - 49+5, scheduleTime[2].c_str());
    mvprintw(LINES/3 + 14, COLS/2 - 49+5, scheduleTime[3].c_str());
    
    mvprintw(LINES/3 + 18, COLS/2 - 49+5, scheduleTime[4].c_str());
    mvprintw(LINES/3 + 19, COLS/2 - 49+5, scheduleTime[5].c_str());
    
    mvprintw(LINES/3 + 21, COLS/2 - 49+5, scheduleTime[6].c_str());
    mvprintw(LINES/3 + 22, COLS/2 - 49+5, scheduleTime[7].c_str());
    attroff(COLOR_PAIR(2));
    
    //Get the info of that course
    courseSchedule courseInfoSchedule;
    string stuff;
    string courseLink;
    studentInfoFile.open("data/classes/" + Class + "/" + ID);
    for (int i = 0; i < 4; ++i)
        getline(studentInfoFile,stuff);
    
    while (getline(studentInfoFile,courseLink)) {
        //Get course info for schedule
        ifstream courseInfo(courseLink + "/info");
        getline(courseInfo, stuff);
        getline(courseInfo, courseInfoSchedule.Name);
        for (int i = 0; i < 3; ++i)
            getline(courseInfo, stuff);
        getline(courseInfo, courseInfoSchedule.Date);
        getline(courseInfo, courseInfoSchedule.Time);
        getline(courseInfo, stuff);
        getline(courseInfo, courseInfoSchedule.Room);
        courseInfo.close();
        
        //Get the row and column to display info of course onto schedule
        int row, column;
        for (int i = 0; i < scheduleDate.size(); ++i)
            if (courseInfoSchedule.Date == scheduleDate[i]) {
                column = COLS/2 - 35+6+i*14 - 2;
                break;
            }
        if (courseInfoSchedule.Time == scheduleTime[0])
            row = LINES/3 + 10;
        else {
            if (courseInfoSchedule.Time == scheduleTime[2])
                row = LINES/3 + 13;
            else {
                if(courseInfoSchedule.Time == scheduleTime[2])
                    row = LINES/3 + 18;
                else
                    row = LINES/3 + 21;
            }
        }
        
        //Display course info onto schedule
        mvprintw(row, column, (courseInfoSchedule.Name).c_str());
        mvprintw(row + 1, column + 2, (courseInfoSchedule.Room).c_str());
    }
    studentInfoFile.close();
    
    
    while (true) {
        int input = getch();
        if (input == '\n')
            break;
    }
    
    
    rectangle(LINES / 3 + 1, 1, LINES - LINES / 3 - 3, COLS - 2).clear(1);
    refresh();
}


void StudentWindow::displayCheckIn(string ID, string Class) {
    rectangle(LINES / 3 + 1, 1, LINES - LINES / 3 - 3, COLS - 2).clear(1);
    
    
    //Print name, ID and class of that student on to the screen
    string firstName, lastName;
    ifstream studentInfoFile("data/classes/" + Class + "/" + ID);
    getline(studentInfoFile, firstName);
    getline(studentInfoFile, lastName);
    studentInfoFile.close();
    
    int sizeName = (firstName + " " + lastName).size(), sizeIDClass = (ID + " - " + Class).size();
    //attron(A_BOLD);
    mvprintw(LINES/3 + 3, COLS/2 - sizeName/2, (firstName + " " + lastName).c_str());
    mvprintw(LINES/3 + 4, COLS/2 - sizeIDClass/2, (ID + " - " + Class).c_str());
    //attroff(A_BOLD);
    
    
    attron(A_REVERSE);
    mvprintw(LINES - 6, COLS/2 - 4, "  [BACK]  ");
    attroff(A_REVERSE);
    
    
    //Get number of courses in which that student enrolls
    studentInfoFile.open("data/classes/" + Class + "/" + ID);
    string stuff;
    int count = 0;
    for (int i = 0; i < 4; ++i)
        getline(studentInfoFile, stuff);
    while (getline(studentInfoFile, stuff))
        ++count;
    studentInfoFile.close();
    
    
    //Draw table
    mvaddch(LINES/3 + 7, COLS/2 - 38, ACS_ULCORNER);
    mvaddch(LINES/3 + 7, COLS/2 + 37, ACS_URCORNER);
    mvaddch(LINES/3 + 7+(count+1)*2, COLS/2 - 38, ACS_LLCORNER);
    mvaddch(LINES/3 + 7+(count+1)*2, COLS/2 + 37, ACS_LRCORNER);
    
    for (int i = 0; i < count+1; ++i)
        mvaddch(LINES/3 + 8+2*i, COLS/2 - 38, ACS_VLINE);
    for (int i = 0; i < count+1; ++i)
        for (int j = COLS/2 - 23; j <= COLS/2 + 37; j += 6)
            mvaddch(LINES/3 + 8+2*i, j, ACS_VLINE);
    
    for (int i = COLS/2 - 37; i <= COLS/2 + 36; ++i)
        for (int j = LINES/3 + 7; j <= LINES/3 + 7+(count+1)*2; j += 2)
            mvaddch(j, i, ACS_HLINE);
    
    for (int i = COLS/2 - 23; i < COLS/2 + 37; i += 6) {
        mvaddch(LINES/3 + 7, i, ACS_TTEE);
        mvaddch(LINES/3 + 7+(count+1)*2, i, ACS_BTEE);
    }
    
    for (int i = LINES/3 + 9; i <= LINES/3 + 7+count*2; i += 2) {
        mvaddch(i, COLS/2 - 38, ACS_LTEE);
        mvaddch(i, COLS/2 + 37, ACS_RTEE);
    }
    
    for (int i = COLS/2 - 23; i < COLS/2 + 37; i += 6)
        for (int j = LINES/3 + 9; j <
             LINES/3 + 7+(count+1)*2; j += 2)
            mvaddch(j, i, ACS_PLUS);
    
    
    //Display course and weeks
    attron(COLOR_PAIR(2));
    mvprintw(LINES/3 + 8, COLS/2-38 + 3, "Course");
    for (int i = 0; i < 10; ++i) {
        if (i == 9)
            mvprintw(LINES/3 + 8, COLS/2-23 + 2+6*9, "W10");
        else
            mvprintw(LINES/3 + 8, COLS/2-23 + 2+6*i, ("W0" + to_string(i+1)).c_str());
    }
    attroff(COLOR_PAIR(2));
    
    
    //Display check-in result on to table
    string courseLink;
    studentInfoFile.open("data/classes/" + Class + "/" + ID);
    for (int i = 0; i < 4; ++i)
        getline(studentInfoFile,stuff);
    
    count = 0;
    while (getline(studentInfoFile,courseLink)) {
        //Get course name
        string courseName;
        ifstream courseInfo(courseLink + "/info");
        getline(courseInfo, stuff);
        getline(courseInfo, courseName);
        courseInfo.close();
        
        //Get attendance from that course
        int attendance[10];
        //Find index of that student in that course
        int index = 0;
        ifstream studentList(courseLink + "/student_list");
        string IDCompare;
        while (getline(studentList,IDCompare)) {
            if (IDCompare == ID)
                break;
            ++index;
        }
        studentList.close();
        
        ifstream attendanceList(courseLink + "/attendance_list");
        for (int i = 0; i < index; ++i)
            getline(attendanceList,stuff);
        for (int i = 0; i < 10; ++i)
            attendanceList >> attendance[i];
        attendanceList.close();
        
        //Display score of that course onto screen
        mvprintw(LINES/3 + 10+2*count, COLS/2-38 + 3, (courseName).c_str());
        for (int i = 0; i < 10; ++i) {
            if (attendance[i] == 0)
                mvprintw(LINES/3 + 10+2*count, COLS/2 - 23 + 3+6*i, "-");
            else
                mvprintw(LINES/3 + 10+2*count, COLS/2 - 23 + 3+6*i, "X");
        }
        
        ++count;
    }
    studentInfoFile.close();
    
    
    while (true) {
        int input = getch();
        if (input == '\n')
            break;
    }
    
    
    refresh();
    rectangle(LINES / 3 + 1, 1, LINES - LINES / 3 - 3, COLS - 2).clear(1);
}

void StudentWindow::changePassword(string ID) {
    rectangle(LINES / 3 + 1, 1, LINES - LINES / 3 - 3, COLS - 2).clear(1);
    rectangle frame(LINES/2 - 1, COLS/2 - 25, 3 + 3 + 3 + 2 + 3 + 2, 50);
    
    vector <rectangle> cwin(3);
    vector <rectangle> bwin(2);
    vector <string> text {
        "Current password",
        "New password",
        "Confirm password"
    };
    vector <string> comps {
        "", "", ""
    };
    vector <string> comp_text {
        "", "", ""
    };
    vector <string> buttons {
        "SAVE",
        "CANCEL"
    };

    cwin[0] = rectangle(frame.top() + 1, frame.left() + 20, 3, 28);
    cwin[1] = rectangle(cwin[0].bottom() + 1, frame.left() + 20, 3, 28);
    cwin[2] = rectangle(cwin[1].bottom() + 1, frame.left() + 20, 3, 28);

    bwin[0] = rectangle(frame.bottom() - 3, frame.left() + frame.width() / 4 - 2, 3, 10);
    bwin[1] = rectangle(frame.bottom() - 3, frame.left() + frame.width() / 4 * 3 - 2, 3, 10);

    Windows *win = new Windows;
    win->setCFrame(cwin);
    win->setBFrame(bwin);
    win->setComponents(comps);
    win->setButtons(buttons);

    int cur = 0, lim = comps.size() + buttons.size();
    while (true) {
        int input = getch();
        if (input == KEY_UP || input == KEY_LEFT) {
            cur = (cur - 1 + lim) % lim;
        } else if (input == KEY_DOWN || input == KEY_RIGHT) {
            cur = (cur + 1) % lim;
        } else if (input == ' ' || input == '\n') {
            if (cur < comps.size()) {
                System *sys = new System;
                for (; cur < comps.size(); ++cur) {
                    comps[cur] = "";
                    win->setComponents(comps);
                    win->updatePtr(cur, (1 << 0) + (1 << 1) + (1 << 2));
                    comps[cur] = sys->getPassword(cwin[cur].top() + 1, cwin[cur].left() + 2, comp_text[cur]);
                    win->setComponents(comps);
                }
                delete sys;
            } else if (cur == comps.size()) {
                Tools *tool = new Tools;
                if (tool->checkPass(comp_text[0], ID)) {
                    if (MD5(comp_text[1]).hexdigest() == MD5(comp_text[2]).hexdigest()) {
                        tool->updatePassword(ID, comp_text[1]);
                        rectangle(frame.bottom() - 5, frame.left() + frame.width() / 2 - 2, 1, 8).clear(1);
                        attron(A_BLINK);
                        init_pair(4, COLOR_CYAN, COLOR_BLACK);
                        attron(COLOR_PAIR(4));
                        mvaddstr(frame.bottom() - 5, frame.left() + frame.width() / 2 - 2, "DONE!      ");
                        attroff(COLOR_PAIR(4));
                        attroff(A_BLINK);
                    } else {
                        attron(A_BLINK);
                        attron(COLOR_PAIR(4));
                        mvaddstr(frame.bottom() - 5, frame.left() + frame.width() / 2 - 2, "Invalid!  ");
                        attroff(COLOR_PAIR(4));
                        attroff(A_BLINK);
                    }
                } else {
                    attron(A_BLINK);
                    attron(COLOR_PAIR(4));
                    mvaddstr(frame.bottom() - 5, frame.left() + frame.width() / 2 - 2, "Invalid! 1");
                    attroff(COLOR_PAIR(4));
                    attroff(A_BLINK);
                }
            } else if (cur == lim- 1) {
                break;
            }
        }
        frame.drawEdges();
        for (int i = 0; i < text.size(); ++i) {
            init_pair(2, COLOR_YELLOW, COLOR_BLACK);
            attron(COLOR_PAIR(2));
            mvaddstr(frame.top() + 2 + 3 * i, frame.left() + 2, text[i].c_str());
            attroff(COLOR_PAIR(2));
        }
        mvprintw(1, 1, "%d", cur);
        win->updatePtr(cur, (1 << 0) + (1 << 1) + (1 << 2));
    }
    
    rectangle(LINES / 3 + 1, 1, LINES - LINES / 3 - 3, COLS - 2).clear(1);
}
