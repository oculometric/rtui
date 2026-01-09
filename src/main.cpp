#include <chrono>
#include <iostream>
#include <thread>

#include "strn.h"

int main()
{
    STRN::TerminalCompositor comp;
    STRN::Window* window = comp.newWindow("test");
    window->setSize({ 20, 4 });
    STRN::Window* window2 = comp.newWindow("lucylucy");
    window2->setPosition({ 16, 8 });
    window2->setSize({ 26, 6 });
    STRN::Window* window3 = comp.newWindow("big guy");
    window3->setPosition({ 50, 2 });
    window3->setSize({ 60, 16 });
    
    window3->setRoot(
        new STRN::HorizontalBox({
            new STRN::ArtBlock(
          " .             "
          " .##           "
          "  #  ..        "
          "     #####-    "
          "   #########   "
          "   ###.#####.  "
          "    ###-####   "
          "     .### .    "
          "          # -# "
          "            ## "
, 15),
            new STRN::VerticalBox({
                new STRN::Label("i didnt want your little bitchass anyway!~"),
                new STRN::Label("busy woman"),
                new STRN::Label("but if you need my love"),
                new STRN::Label("my clothes are off im coming over to your place"),
                new STRN::Label("the quick brown fox jumps over the lazy dog. oh yeah, and shes a really cute fox too actually!"),
            })
        }));
    
    STRN::Label perf_label("fps: 0");
    STRN::Label perf_label2("dt: 0");
    window->setRoot(new STRN::VerticalBox({ &perf_label, &perf_label2 }));
    
    size_t i = 0;
    auto begin = std::chrono::high_resolution_clock::now();
    auto last = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> ideal_frame_time(1.0f / 120.0f);
    
    while (i < 100000)
    {
        comp.update();
        auto now = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> duration = now - last;
        std::this_thread::sleep_for(ideal_frame_time - duration);
        last = now;
        perf_label.text = "fps: " + std::to_string(1.0f / duration.count());
        perf_label2.text = "dt: " + std::to_string(duration.count() * 1000.0) + "ms";
        ++i;
    }
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<float> duration = end - begin;
    std::cout << "HALTED" << std::endl;
    std::cout << duration.count() << "s to render " << i << " frames" << std::endl;
    std::cout << "aka " << (float)i / duration.count() << "fps";
    
    while (true);
}