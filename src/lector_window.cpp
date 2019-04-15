#include <lector_window.hpp>

LectureWindow::LectureWindow() {};

void LectureWindow::init(string lecture_account){
    vector <rectangle> bwin ; 
    
    buttons = lector->getCurrentCourse(lecture_account) ;
    buttons.push_back("      BACK      ") ; 
    
    frame = rectangle(LINES / 3 + 5, COLS / 2 - 25, 15, 50);   
    bwin.push_back(rectangle(frame.top() + 1, frame.left() + 10, 3, 30));
    for (int i=0 ;i <  buttons.size()-1;++i) {
          rectangle ss = rectangle(bwin.back().bottom() + 1, bwin.back().left(), 3, 30);
          bwin.push_back(ss);
    }
    win->setBFrame(bwin);
    win->setButtons(buttons);
    win->setCFrame(vector <rectangle> (0));
    win->setComponents(vector<string> (0));
    rectangle(LINES / 3 + 1, 1, LINES - LINES / 3 - 3, COLS - 2).clear(1);
}

LectureWindow::~LectureWindow(){
    delete win ; 
    delete lector ; 
} 

void LectureWindow::load(string lecture_account){
    init(lecture_account);
    int bottom = LINES / 3;
    int cur = 0;
    while (true) {
        int mod = buttons.size() ; 
        int input = getch();
        if (input == KEY_DOWN ) {
            cur = (cur + 1) % mod;
        } if (input == KEY_UP ) {
            cur = (cur -1 + mod ) % mod; 
        } else if (input=='\n'){
            if (cur==mod-1)  
                break ; 
            else 
            {
                info_window(lecture_account, cur) ;  
            }
        }
        win->updatePtr(cur);  
    }
    rectangle(LINES / 3 + 1, 1, LINES - LINES / 3 - 3, COLS - 2).clear(1);
}
void LectureWindow::info_window(string lecture_account, int cur1){
  vector <string> buttonss = { 
        "Students" ,
        "Attendence",
        "Scoreboard",
        "Cancel"
    } ;
    vector <rectangle> bwin(4) ; 
    enum BUTTONS {
        Students , 
        Attendence,
        Scoreboard,  
        Cancel  
    } ; 
    Windows *cwin = new Windows;  
    frame = rectangle(LINES / 3 + 5, COLS / 2 - 25, 15, 50);    
    bwin[Students] = rectangle(frame.top() + 1, frame.left() + 10, 3, 30);
    bwin[Attendence] = rectangle(bwin[Students].bottom() + 1, bwin[Students].left(), 3, 30);  
    bwin[Scoreboard] = rectangle(bwin[Attendence].bottom() + 1, bwin[Attendence].left(),3,30 );
    bwin[Cancel] = rectangle(bwin[Scoreboard].bottom() + 1, bwin[Scoreboard].left(),3,30 );
    cwin->setBFrame(bwin);
    cwin->setComponents(comps);
    cwin->setButtons(buttonss);

    rectangle(LINES / 3 + 1, 1, LINES - LINES / 3 - 3, COLS - 2).clear(1);

    int bottom = LINES / 3;
    int cur = 0;
    while (true) {
        int input = getch();
        if (input == KEY_DOWN ) {
            cur = (cur + 1) % 4;
        } if (input == KEY_UP ) {
            cur = (cur + 3) % 4;
        } else if (input=='\n'){
            if (cur==0){
                LectCourseNode *courseNode = new LectCourseNode("data/courses/2019/Spring/"+buttons[cur1]) ; 
                courseNode->displayStudent() ; 
                rectangle(LINES / 3 + 1, 1, LINES - LINES / 3 - 3, COLS - 2).clear(1);
            }
            if (cur==1){ 
                LectCourseNode *courseNode = new LectCourseNode("data/courses/2019/Spring/"+buttons[cur1]) ; 
                courseNode->displayAttend() ; 
                rectangle(LINES / 3 + 1, 1, LINES - LINES / 3 - 3, COLS - 2).clear(1);
        
            }
            if (cur == 2) {
                LectCourseNode *courseNode = new LectCourseNode("data/courses/2019/Spring/"+buttons[cur1]) ; 
                courseNode->displayScoreboard("aaa") ;  
                rectangle(LINES / 3 + 1, 1, LINES - LINES / 3 - 3, COLS - 2).clear(1);
            }
            else 
                break ; 
        }
        cwin->updatePtr(cur);  
    }
    rectangle(LINES / 3 + 1, 1, LINES - LINES / 3 - 3, COLS - 2).clear(1);
}
