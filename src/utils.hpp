#ifndef UTILS_H_DEFINED
#define UTILS_H_DEFINED

#include <vector>
#include <string>
#include <fstream>
#include <cstdlib>
#include <iostream>
#include <algorithm>
#include <string.h>

#include <md5.hpp>

#include <dirent.h>
#include <ncurses.h>
#include <sys/stat.h> 
#include <sys/types.h>


using namespace std;

class System {
public:
    System();

    ~System();

    string getInput(int x, int y, bool flag = false);

    string getID(int x, int y);
    
    string getPassword(int x, int y);

    string getPassword(int x, int y, string &ss);

    int checkPass();

    string getClassName(string ss);

    void updateData(string path, const vector <string> &a);
    
    string getClass();

private:
    string pass, id;
};

class Tools {
public:
    Tools();
    
    ~Tools();

    void createFolder(string path);
        
    string createFolder(string init, string raw);
    
    int findNext(string s, int i, char value);
    
    vector <string> getElement(string s, char value);
    
    void deleteLineFromFile(string path, int pos);

    vector <string> read_directory(const std::string& name);

    vector <string> getData(int id, string path);

    vector <string> getContent(string path);
    
    string getClass(string id);

    bool isExist(string path, string content); //check if the content exist in the file.

    bool isFileExist(string path);

    bool isDirExist(string path);

    bool checkPass(string pass, string id);

    void updatePassword(string id, string pass);

private:
    System *sys;
};

#endif