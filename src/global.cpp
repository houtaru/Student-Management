#include <global.hpp>

#include <md5.hpp>

Global::Global() {
    graphic = new Graphic();
    
}

Global::~Global() {
    delete graphic;
}

void Global::load() {
    graphic->load();
}
