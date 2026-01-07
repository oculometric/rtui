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
    
    window3->setRoot(
        new RTUI::VerticalBox({
            new RTUI::Label("i didnt want your little bitchass anyway!~"),
            new RTUI::Label("busy woman"),
            new RTUI::Label("but if you need my love"),
            new RTUI::Label("my clothes are off im coming over to your place"),
        }));
    
    while (true)
        comp.update();
}