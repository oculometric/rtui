#include <chrono>
#include <iostream>

#include "rtui.h"

int main()
{
    RTUI::TerminalCompositor comp;
    RTUI::Window* window = comp.newWindow("test");
    window->setSize({ 20, 3 });
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
            new RTUI::Label("the quick brown fox jumps over the lazy dog. oh yeah, and shes a really cute fox too actually!"),
        }));
    
    RTUI::Label perf_label("fps: 0");
    window->setRoot(&perf_label);
    
    size_t i = 0;
    auto begin = std::chrono::high_resolution_clock::now();
    auto last = std::chrono::high_resolution_clock::now();
    while (i < 100000)
    {
        auto now = std::chrono::high_resolution_clock::now();
        std::chrono::duration<float> duration = now - last;
        last = now;
        perf_label.text = "fps: " + std::to_string(1.0f / duration.count());
        comp.update();
        ++i;        
    }
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<float> duration = end - begin;
    std::cout << "HALTED" << std::endl;
    std::cout << duration.count() << "s to render " << i << " frames" << std::endl;
    std::cout << "aka " << (float)i / duration.count() << "fps";
    
    while (true);
}