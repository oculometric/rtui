#pragma once

#include <string>
#include <vector>

#include "vec2.h"

// TODO: organise into files with context, widget, window, compositor in one, subcompositors in another, subwidgets in another

namespace RTUI
{

static Vec2 clip(Vec2 min_size, Vec2 max_size, Vec2 available_size)
{
    int x = 0;
    if (max_size.x == -1) x = available_size.x;
    else x = std::min(available_size.x, max_size.x);
    int y = 0;
    if (max_size.y == -1) y = available_size.y;
    else y = std::min(available_size.y, max_size.y);
    
    return Vec2{ x, y };
}

class Context
{
    friend class Window;
    friend class Compositor;
private:
    std::vector<char> backing;
    int pitch = 0;
    std::vector<Box2> bounds_stack;
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
    
    // TODO: context should handle clipping (for scrollable regions)
    // TODO: make this so that widgets can't just pop and push their own bounds....
    void pushPermittedBounds(Vec2 min, Vec2 max);
    void popPermittedBounds();
    
private:
    inline Context() : Context({1, 1}, ' ') { }
    Context(Vec2 size, char fill_value);
    ~Context() = default;
    
    void clear(char fill_value);
    void resize(Vec2 new_size, char fill_value);
}; 

// TODO: proper dirty flags for the entire tree
#pragma region WIDGETS

class Widget
{
public:
    Transform2 transform; // stores position within parent, and size
protected:
    std::vector<Widget*> children;
    Widget* parent = nullptr;
    bool visible = false;
    
public:
    Widget() = default;
    // TODO: move and copy constructors?
    virtual ~Widget() = default;
    
    virtual void arrange(Vec2 available_area) { transform.size = clip(getMinSize(), getMaxSize(), available_area); }
    virtual void render(Context& ctx) const { }
    
    virtual Vec2 getMinSize() const { return Vec2{ 1, 1 }; }
    virtual Vec2 getMaxSize() const { return Vec2{ -1, -1 }; }
};

class Label : public Widget
{
public:
    std::string text;
    
public:
    Label(const std::string& _text) : text(_text) { }
    
    void render(Context& ctx) const override { ctx.drawText(Vec2{ 0, 0 }, text); }
    
    Vec2 getMinSize() const override { return Vec2{ 0, 1 }; }
    Vec2 getMaxSize() const override { return Vec2{ -1, 1 }; }
};

class ArrangedBox : public Widget
{
public:
    void render(Context& ctx) const override;
};

class VerticalBox : public ArrangedBox
{
public:
    VerticalBox(const std::vector<Widget*>& _children) { children = _children; }
    
    void arrange(Vec2 available_area) override;
};

class HorizontalBox : public ArrangedBox
{
public:
    HorizontalBox(const std::vector<Widget*>& _children) { children = _children; }
    
    void arrange(Vec2 available_area) override;
};

class ArtBlock : public Widget
{
public:
    std::string ascii;
    int pitch;
    
public:
    ArtBlock(const std::string& _ascii, int _pitch) : ascii(_ascii), pitch(_pitch) { }
    
    void render(Context& ctx) const override;
    
    Vec2 getMinSize() const override { return Vec2{ pitch, static_cast<int>(ascii.size()) / pitch }; }
    Vec2 getMaxSize() const override { return getMinSize(); }
};

// TODO: border
// TODO: spacers
// TODO: dividers
// TODO: button
// TODO: list view
// TODO: tab view
// TODO: bitmap image
// TODO: toggle button
// TODO: radio button
// TODO: grid boxes

#pragma endregion

class Window
{
    friend class Compositor;
private:
    Widget* root = nullptr;
    // TODO: manages widgets, and draws them (controls and state management)
    
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
    
    Vec2 getSize() const { return size; }
    void setSize(Vec2 _size) { size = _size; is_dirty = true; }
    Vec2 getPosition() const { return position; }
    void setPosition(Vec2 _position) { position = _position; is_dirty = true; }
    std::string getTitle() const { return title; }
    void setTitle(const std::string& _title) { title = _title; is_dirty = true; }
    void setAllowsMinimise(bool _allows_minimise) { allows_minimise = _allows_minimise; if (!allows_minimise) is_minimised = false; is_dirty = true; }
    void setBorderless(bool _borderless) { borderless = _borderless; is_dirty = true; }
    bool getMinimised() const { return is_minimised; }
    void setRoot(Widget* _root) { root = _root; is_dirty = true; }
    
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
    const Context& getContext() const { return context; }
    void clearContext() { context.clear(' '); } 
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
