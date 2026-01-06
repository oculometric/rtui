#include "rtui.h"

int main()
{
    RTUI::TerminalCompositor comp;
    RTUI::Window* window = comp.newWindow("test");
    RTUI::Window* window2 = comp.newWindow("lucylucy");
    window2->setPosition({ 16, 8 });
    window2->setSize({ 26, 6 });
    RTUI::Window* window3 = comp.newWindow("big guy");
    window3->setPosition({ 50, 2 });
    window3->setSize({ 60, 16 });
    
    while (true)
        comp.update();
}