#include <staff.hpp>

/*          *           *           OVERALL           *           *           */

Staff::Staff() {
    sys = new System;
    tool = new Tools;
    staffClass = new StaffClass;
    staffCourse = new StaffCourse;
    srand(time(NULL));
}

Staff::~Staff() {
    delete sys;
    delete tool;
    delete staffClass;
    delete staffCourse;
}

void Staff::import() {
    // Create staff account
    if (!tool->isExist("data/account", "0 admin")) {
        system(("echo 0 admin " + MD5("admin").hexdigest() + " x >> data/account").c_str());
    }
    // Create lecturer account
    vector <string> lect { "aaa", "bbb", "ccc"};
    char k = '1';
    for (auto it : lect) if (!tool->isExist("data/account", "1 " + it)) {
        system(("echo 1 " + it + " " + MD5(string(4, k++)).hexdigest() + " x >> data/account").c_str());
    }

    system("chmod +x import.sh"); //Excute import.sh
    system("./import.sh classes"); //Get list of class
    system("./import.sh courses"); //Get list of courses

    //define variables
    ifstream fin[2];
    string ss;

    //open files
    fin[0].open("data/classes_list");
    fin[1].open("data/courses_list");

    // Import classes list
    while (fin[0] >> ss) {
        staffClass->import(ss);
    }

    // Import courses list
    while (fin[1] >> ss) {
        staffCourse->import(ss);
    }

    // Close files
    fin[0].close();
    fin[1].close();
}

/*          *           *           CLASS           *           *           */
StaffClass::StaffClass() {
    tool = new Tools;
}

StaffClass::~StaffClass() {
    delete tool;
}

void StaffClass::import(string class_name) {
    tool->createFolder("data/classes/" + class_name);
    
    ifstream fin("data/init/classes/" + class_name + ".csv");
    ofstream fout[2];
    string s; getline(fin, s);
    while (getline(fin, s)) {
        // No[0], Student ID[1], Last name[2], First name[3], gender[4], DoB[5]
        vector <string> cur = tool->getElement(s + ',', ',');
        // check wheither student information is storage or not.
        if (tool->isFileExist("data/classes/" + class_name + "/" + cur[1])) continue;

        // storage infomation
        fout[0].open("data/classes/" + class_name + "/" + cur[1]);
        fout[1].open("data/account", ios::app);

        if (!isdigit(cur[5].back())) cur[5].pop_back();
        vector <string> dob = tool->getElement(cur[5] + "-", '-');

        fout[0] << cur[2] << "\n" << cur[3] << "\n" << cur[4] << "\n";
        fout[0] << dob[2] << "-" << dob[1] << "-" << dob[0] << endl;
        
        //create account for student
        fout[1] << 2 << " " << cur[1] << " " << MD5(dob[2] + dob[1] + dob[0]) << " " << class_name << endl;
        system(("echo " + cur[1] + " " + dob[2] + dob[1] + dob[0] + " >> account.log").c_str());

        fout[0].close();
        fout[1].close();
    }
    fin.close();
}

vector <string> StaffClass::getClassList() {
    return tool->read_directory("data/classes/");
}

vector <string> StaffClass::getStudentList(string classtmp) {
    return tool->read_directory("data/classes/" + classtmp);
}

/*          *           *           COURSE           *           *           */
StaffCourse::StaffCourse() {
    tool = new Tools;
    staffClass = new StaffClass;
}

StaffCourse::~StaffCourse() {
    delete tool;
    delete staffClass;
}

void StaffCourse::import(string filename) {
    string raw = tool->createFolder("data/courses", filename);

    ifstream fin("data/init/courses/" + filename + ".csv");
    ofstream fout[4];

    string s; getline(fin, s);
    while (getline(fin, s)) {
        //No[0], Course ID[1], Course Name[2], Class[3], LecturerAcount[4], Start Date[5],End Date[6],Day of Week[7],Start Hour[8],End Hour[9],Room[10]
        vector <string> cur = tool->getElement(s + ",", ',');
        vector <string> c = staffClass->getStudentList(cur[3]);
        
        // if storage of course exist then skip
        if (tool->isDirExist(raw + "/" + cur[1])) continue;

        //storage course info to lecturer storage
        tool->createFolder("data/lecturers/" + cur[4]);
        tool->createFolder("data/lecturers/" + cur[4] + "/" + filename + "-" + cur[1]);

        // create course storage
        tool->createFolder(raw + "/" + cur[1]);

        // save course information to storage
        fout[0].open(raw + "/" + cur[1] + "/info");
        fout[1].open(raw + "/" + cur[1] + "/student_list");
        fout[2].open(raw + "/" + cur[1] + "/attendance_list");
        fout[3].open(raw + "/" + cur[1] + "/scoreboard");

        //save infomation fo storage
        fout[0] << cur[4] << "\n";
        for (int i = 1; i < (int) cur.size(); ++i) if (i != 3 && i != 4) 
            fout[0] << cur[i] << "\n";

        for (int i = 0; i < (int) c.size(); ++i) {
            fout[1] << c[i] << "\n";
            for (int i = 0; i < 10; ++i) fout[2] << rand() % 2 << " ";
            fout[2] << "\n";
            for (int i = 0; i < 4; ++i) fout[3] << rand() % 10 << " ";
            fout[3] << "\n"; // score: mid final lab bonus
            
            ofstream out("data/classes/" + cur[3] + "/" + c[i], ios::app);
            out << raw + "/" + cur[1] << "\n";
            out.close();
        }
        for (int i = 0; i < 4; ++i) fout[i].close();
    }
    fin.close();
}

vector <string> StaffCourse::getYearList(string path){
    return tool->read_directory(path) ; 
}

vector <string> StaffCourse::getSemesterList(string path){
    return tool->read_directory(path) ;
}

vector <string> StaffCourse::getCourseList(string path){
    return tool->read_directory(path) ; 
}

vector <string> StaffCourse::getCoursesInfo(string path){
    vector <string> result; 
    ifstream fin; 
    fin.open(path); 
    string s; 
    while (getline (fin,s))
        result.push_back(s); 
    return result ; 
}

vector <string> StaffCourse::getData(int id, string path) {
    switch (id) {
        case 0:
            return getYearList(path);
        case 1:
            return getSemesterList(path);
        case 2:
            return getCourseList(path);
        case 3:
            return getCoursesInfo(path);
    }
    return vector <string>();
}
