#include <chrono>
#include <iostream>
#include <thread>

#include "strn.h"

const char* saturn_ascii = 
" .             "
" .##           "
"  #  ..        "
"     #####-    "
"   #########   "
"   ###.#####.  "
"    ###-####   "
"     .### .    "
"          # -# "
"            ## ";

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
    STRN::Builder w3_builder;
    w3_builder.beginHorizontalBox();
        w3_builder.artBlock(saturn_ascii, 15);
        w3_builder.beginVerticalBox();
            w3_builder.label("i didnt want your little bitchass anyway!~");
            w3_builder.label("busy woman");
            w3_builder.label("but if you need my love");
            w3_builder.label("my clothes are off im coming over to your place");
            w3_builder.label("the quick brown fox jumps over the lazy dog. oh yeah, and shes a really cute fox too actually!");
        w3_builder.endVerticalBox();
    w3_builder.endHorizontalBox();
    window3->setRoot(w3_builder.end());
    
    STRN::Label perf_label("fps: 0");
    STRN::Label perf_label2("dt: 0");
    window->setRoot(new STRN::VerticalBox({ &perf_label, &perf_label2 }));
    
    constexpr std::chrono::duration<double> ideal_frame_time(1.0 / 60.0);
    
    while (true)
    {
        auto last = std::chrono::high_resolution_clock::now();
        comp.update();
        auto now = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> duration = now - last;        
        perf_label.setText("fps: " + std::to_string(1.0 / duration.count()));
        perf_label2.setText("dt: " + std::to_string(duration.count() * 1000.0) + "ms");
        auto sleep_duration = ideal_frame_time - duration;
        std::this_thread::sleep_for(sleep_duration); // prevents us from spinning too hard
    }
}