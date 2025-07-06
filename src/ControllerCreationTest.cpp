#include "ControllerCreationTest.hpp"

bool Ctrl::created = false;
bool SimpleCtrl::created = false;
bool WsCtrl::created = false;

void Ctrl::initPathRouting()
{
    created = true;
}

void SimpleCtrl::initPathRouting()
{
    created = true;
}

void WsCtrl::initPathRouting()
{
    created = true;
}