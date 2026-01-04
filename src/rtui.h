#pragma once

#include <string>
#include <vector>

#include "vec2.h"

namespace RTUI
{

class Context
{
    friend class Window;
    friend class Compositor;
    // TODO: stores drawing information and buffers
private:
    std::vector<char> backing;
    int pitch = 0;
    Vec2 permitted_bounds;
    
public:
    Context(const Context& other) = delete;
    Context(Context&& other) = delete;
    void operator=(const Context& other) = delete;
    void operator=(Context&& other) = delete;
    
    void setChrxel(Vec2 position, char value);
    
private:
    Context(Vec2 size, char fill_value);
    ~Context();
    
    void setPermittedBounds(Vec2 min, Vec2 max);
}; 

class Widget
{
    // TODO: transform, z layer, visible, focusable
public:
    virtual void render(Context& ctx) const;
    
    virtual Vec2 getMinSize() const;
    virtual Vec2 getMaxSize() const;
};

class Window
{
private:
    // TODO: manages widgets, and draws them (name, controls and state management, borders)
    // widget transform tree
    
    Vec2 size = { 10, 10 };
    std::string title = "window";
    bool allows_minimise = true;
    bool is_minimised = false;
    bool borderless = false;
    bool is_dirty = false;
    
public:
    inline Vec2 getSize() const { return size; }
    inline void setSize(Vec2 _size) { size = _size; is_dirty = true; }
    inline std::string getTitle() const { return title; }
    inline void setTitle(const std::string& _title) { title = _title; is_dirty = true; }
    inline void setAllowsMinimise(bool _allows_minimise) { allows_minimise = _allows_minimise; if (!allows_minimise) is_minimised = false; is_dirty = true; }
    inline void setBorderless(bool _borderless) { borderless = _borderless; is_dirty = true; }
    inline bool getMinimised() const { return is_minimised; }
    
    void render(Context& ctx) const;
}; 

class Compositor; // TODO: manages windows and the environment (desktop mode, fixed-size mode, single-window mode)

}
