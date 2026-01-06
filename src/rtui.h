#pragma once

#include <string>
#include <vector>

#include "rtui.h"
#include "vec2.h"

namespace RTUI
{

class Context
{
    friend class Window;
    friend class Compositor;
private:
    std::vector<char> backing;
    int pitch = 0;
    Box2 permitted_bounds;
    
public:
    Context(const Context& other) = delete;
    Context(Context&& other) = delete;
    void operator=(const Context& other) = delete;
    void operator=(Context&& other) = delete;
    
    inline Vec2 getSize() const { return permitted_bounds.size(); }
    void draw(Vec2 position, char value);
    void drawBox(Vec2 start, Vec2 size);
    void drawText(Vec2 start, const std::string& text, size_t text_offset = 0, size_t max_length = -1);
    
    std::vector<char>::const_iterator begin() const;
    std::vector<char>::const_iterator end() const;
    
private:
    inline Context() : Context({1, 1}, ' ') { }
    Context(Vec2 size, char fill_value);
    ~Context() = default;
    
    void setPermittedBounds(Vec2 min, Vec2 max);
    void resize(Vec2 new_size, char fill_value);
}; 

class Widget
{
    Transform2 transform;
    // TODO: transform, z layer, visible, focusable
public:
    virtual void render(Context& ctx) const;
    
    virtual Vec2 getMinSize() const;
    virtual Vec2 getMaxSize() const;
};

class Window
{
    friend class Compositor;
private:
    // TODO: manages widgets, and draws them (name, controls and state management, borders)
    // widget transform tree
    
    Vec2 size = { 10, 10 };
    Vec2 position = { 5, 5 };
    std::string title = "window";
    bool allows_minimise = true;
    bool is_minimised = false;
    bool borderless = false;
    bool is_dirty = true;
    
public:
    Window(const Window& other) = delete;
    Window(Window&& other) = delete;
    void operator=(const Window& other) = delete;
    void operator=(Window&& other) = delete;
    
    inline Vec2 getSize() const { return size; }
    inline void setSize(Vec2 _size) { size = _size; is_dirty = true; }
    inline Vec2 getPosition() const { return position; }
    inline void setPosition(Vec2 _position) { position = _position; is_dirty = true; }
    inline std::string getTitle() const { return title; }
    inline void setTitle(const std::string& _title) { title = _title; is_dirty = true; }
    inline void setAllowsMinimise(bool _allows_minimise) { allows_minimise = _allows_minimise; if (!allows_minimise) is_minimised = false; is_dirty = true; }
    inline void setBorderless(bool _borderless) { borderless = _borderless; is_dirty = true; }
    inline bool getMinimised() const { return is_minimised; }
    
    void render(Context& ctx) const;
    
private:
    Window(const std::string& _title, bool _borderless, bool _minimised) : title(_title), borderless(_borderless), is_minimised(_minimised) { }
    ~Window() = default;
}; 

class Compositor
{
private:
    std::vector<Window*> windows;
    Context context;
    Vec2 size;
    
public:
    Compositor(const Compositor& other) = delete;
    Compositor(Compositor&& other) = delete;
    void operator=(const Compositor& other) = delete;
    void operator=(Compositor&& other) = delete;
    virtual ~Compositor() = default;
    
    virtual void update() { }
    Window* newWindow(std::string title, bool borderless = false, bool start_minimised = false);
    inline Vec2 getSize() const { return size; }
    
protected:
    inline Compositor() { }
    
    void renderWindows();
    inline const Context& getContext() const { return context; }
    void setSize(Vec2 new_size);
}; 

class TerminalCompositor : public Compositor
{
public:
    TerminalCompositor();
    TerminalCompositor(const TerminalCompositor& other) = delete;
    TerminalCompositor(TerminalCompositor&& other) = delete;
    void operator=(const TerminalCompositor& other) = delete;
    void operator=(TerminalCompositor&& other) = delete;
    ~TerminalCompositor() override = default;
    
    static Vec2 getScreenSize();
    static void setCursorVisible(bool visible);
    static void setCursorPosition(Vec2 position);
    
    void update() override;
};

// TODO: manages windows and the environment (desktop mode, fixed-size mode, single-window mode, handled by child classes)

}
