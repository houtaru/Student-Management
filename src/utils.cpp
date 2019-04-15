#include "utils.hpp"

/*      *       *       *       *       TOOL       *       *       *       */

Tools::Tools() {
    sys = new System;
}

Tools::~Tools() {
    delete sys;
}

void Tools::createFolder(string path) {
    system(("mkdir -p \"" + path + "\"").c_str());
}

string Tools::createFolder(string init, string raw) {
    vector <string> e = getElement(raw, '-');
    for (auto it : e) {
        init += "/" + it;
        createFolder(init);
    }
    return init;
}

int Tools::findNext(string s, int i, char value) {
    return find(s.begin() + i, s.end(), value) - s.begin();
}

vector <string> Tools::getElement(string s, char value) {
    vector <string> result;
    for (int i = 0; i < s.size(); ) {
        int j = findNext(s, i, value);
        result.push_back(s.substr(i, j - i));
        i = j + 1;
    }
    return result;
}

void Tools::deleteLineFromFile(string path, int pos) {
    ifstream fin(path);
    ofstream fout("temp");
    string line;
    int cnt = 0;
    while (getline(fin, line)) {
        cnt++;
        if (cnt != pos) fout << line << "\n";
    }
    remove(path.c_str());
    rename("temp", path.c_str());
}

vector <string>  Tools::read_directory(const std::string& name)
{
    vector <string> result ; 
    DIR* dirp = opendir(name.c_str());
    struct dirent * dp;
    while ((dp = readdir(dirp)) != NULL) {
        if (dp->d_name[0] != '.') result.push_back(dp->d_name);
    }
    closedir(dirp);
    return result; 
}

vector <string> Tools::getContent(string path) {
    ifstream fin(path);
    vector <string> result;
    string s;
    while (getline(fin, s)) result.push_back(s);
    return result;
}

string Tools::getClass(string id) {
    ifstream fin("data/account");
    string a, b, c, f;
    while (fin >> f >> a >> b >> c) {
        if (id == a)
            return c;
    }
    fin.close();
    return "";
}

bool Tools::isExist(string path, string x) {
    vector <string> des = getElement(x + ' ', ' ');
    
    ifstream fin(path);
    string ss;
    while (getline(fin, ss)) {
        while (ss.size() && !isalpha(ss.back()) && !isdigit(ss.back())) ss.pop_back();
        vector <string> cur = getElement(ss + ' ', ' ');

        bool ok = true;
        for (int i = 0; i < des.size(); ++i) if (strcmp(des[i].c_str(), cur[i].c_str()) != 0) {
            ok = false;
            break;
        }
        if (ok) {
            fin.close();
            return true;
        }
    }
    fin.close();
    return false;
}

bool Tools::isFileExist(string path) {
    ifstream fin(path);
    return fin.is_open();
}

bool Tools::isDirExist(string path) {
    struct stat info;
    if (stat(path.c_str(), &info) != 0) return false;
    return true;
}

bool Tools::checkPass(string pass, string id) {
    ifstream fin("data/account");
    int type;
    string a, b, c;
    while (fin >> type >> a >> b >> c) if (a == id) {
        system(("echo " + MD5(pass).hexdigest() + " " + b + " >> error.log").c_str());
        if (MD5(pass).hexdigest() == b) {
            return true;
        }
    }
    fin.close();
    return false;
}

void Tools::updatePassword(string id, string pass) {
    vector < vector <string> > comps;
    ifstream fin("data/account");
    string ss;
    while (getline(fin, ss)) {
        while (!ss.empty() && !isdigit(ss.back()) && !isalpha(ss.back())) {
            ss.pop_back();
        }
        comps.push_back(getElement(ss + ' ', ' '));
    }
    for (int i = 0; i < (int) comps.size(); ++i) if (comps[i][1] == id) {
        comps[i][2] = MD5(pass).hexdigest();
        break;
    }
    fin.close();
    ofstream fout("data/account");
    for (auto it : comps) {
        for (auto jt : it) fout << jt << " ";
        fout << "\n";
    }
    fout.close();
}

/*      *       *       *       *       SYSTEM       *       *       *       */

System::System() {}

System::~System() {}

string System::getInput(int a, int b, bool flag) {
    string text;
    int x = a, y = b;
    attron(A_REVERSE);
    while (true) {
        int input = getch();
        if (input == '\n') break;
        if ((input == 127 || input == 263) && y > b) {
            mvaddch(x, --y, ' ');
            if (!text.empty()) text.pop_back();
        }
        if (isalpha(char(input)) || isdigit(char(input)) || char(input) == ':' || char(input) == '-' || char(input) == '.') {
            text.push_back(char(input));
            mvaddch(x, y++, (flag) ? '*' : char(input));
        }
    }
    attroff(A_REVERSE);
    refresh();
    return text;
}

string System::getID(int x, int y) { return id = getInput(x, y); }

string System::getPassword(int x, int y) { return string((pass = getInput(x, y, true)).size(), '*'); }

string System::getPassword(int x, int y, string &ss) {
    ss = getInput(x, y, true);
    return string(ss.size(), '*');
}

int System::checkPass() {
    ifstream fin("data/account");
    int type;
    string a, b, c;
    while (fin >> type >> a >> b >> c) if (a == id) {
        if (MD5(pass).hexdigest() == b) {
            return type;
        }
    }
    fin.close();
    return -1;
}

string System::getClassName(string ss) {
    ifstream fin("data/account");
    int type;
    string a, b, c;
    while (fin >> type >> a >> b >> c) if (a == id) {
        return c;
    }
    fin.close();
    return string("Invalid");
}


void System::updateData(string path, const vector <string> &a) {
    ofstream fout(path);
    for (auto it : a) fout << it << "\n";
    fout.close();
}

string System::getClass() {
    ifstream fin("data/account");
    string a, b, c, f;
    while (fin >> f >> a >> b >> c) {
        if (id == a)
            return c;
    }
    fin.close();
    return "";
}
