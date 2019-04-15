#ifndef LECTOR_NODE_H_DEFINED
#define LECTOR_NODE_H_DEFINED

#include <ncurses.h>
#include <object.hpp>
#include <utils.hpp>

class LectorNode {
public:
    LectorNode();

    ~LectorNode();
    
    void load();

private:
    Tools *tool;
};

#endif
