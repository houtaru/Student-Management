#ifndef GLOBAL_HPP_DEFINED
#define GLOBAL_HPP_DEFINED

#include <graphic.hpp>

class Global {
public:
    Global();

    ~Global();

    void load();
    
private:
    Graphic *graphic;
};

#endif