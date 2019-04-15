#ifndef STUDENT_WINDOW_H_DEFINED
#define STUDENT_WINDOW_H_DEFINED

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <iomanip>
#include <stdio.h>
#include <ncurses.h>

#include <object.hpp>
#include <utils.hpp>

using namespace std;

enum student {
    PROFILE,
    CHECKIN,
    SCHEDULE,
    SCOREBOARD,
    CHANGEPASSWORD,
    LOGOUT
};

class StudentWindow {
public:
    StudentWindow();

    ~StudentWindow();
    
    void load(string ID, string Class);
    
    void displayProfile(string ID, string Class);
    
    void displaySchedule(string ID, string Class);
    
    void displayScoreboard(string ID, string Class);

    void displayCheckIn(string ID, string Class);

    void changePassword(string ID);

private:
    const vector <string> maincomp {
        "       PROFILE       ",
        "   CHECK-IN RESULT   ",
        "       SCHEDULE      ",
        "      SCOREBOARD     ",
        "  [CHANGE PASSWORD]  ",
        "  [LOG OUT]  "
    };
};

#endif