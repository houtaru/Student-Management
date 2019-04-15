#include <lector.hpp>

Lector::Lector() {
    tool = new Tools;
}

Lector::~Lector() {
    delete tool;
}

vector <string> Lector::getCurrentCourse(string name) {
    vector <string> result ; 
    string path  = "data/courses/2019/Spring" ; 
    Tools *tool= new Tools ; 
    vector <string> CurrentCourse = tool -> read_directory(path)  ; 
    ifstream fin ; 
    for (int i=0;i<CurrentCourse.size()-1;++i){
        string account ; 
        fin.open(path+ "/"+CurrentCourse[i]+"/info") ; 
        fin >> account ; 
        if (name == account) 
            result.push_back(CurrentCourse[i]) ;  
    return result ; 
    }
}
