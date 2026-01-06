#include "rtui.h"

#include <stdexcept>
#include <Windows.h>
#include <iostream>

using namespace RTUI;

#pragma region CONTEXT

void Context::draw(Vec2 position, char value)
{
    Vec2 real_position = position + permitted_bounds.min;
    if (real_position.x >= permitted_bounds.max.x
        || real_position.y >= permitted_bounds.max.y)
        return;
    size_t offset = real_position.x + (real_position.y * pitch);
    backing[offset] = value;
}

// TODO: make this actually platform-independent https://stackoverflow.com/questions/4494306/drawing-tables-in-terminal-using-ansi-box-characters
void Context::drawBox(Vec2 start, Vec2 size)
{
    if (size.x < 2 || size.y < 2)
        return;
    draw(start, 0xDA);
    for (int a = start.x + 1; a < start.x + size.x - 1; ++a)
    {
        draw(Vec2{ a, start.y }, 0xC4);
        draw(Vec2{ a, start.y + size.y - 1 }, 0xC4);
    }
    draw(start + Vec2{ size.x - 1, 0 }, 0xBF);
    draw(start + Vec2{ 0, size.y - 1 }, 0xC0);
    for (int a = start.y + 1; a < start.y + size.y - 1; ++a)
    {
        draw(Vec2{ start.x, a }, 0xB3);
        draw(Vec2{ start.x + size.x - 1, a }, 0xB3);
    }
    draw(start + size - Vec2{ 1, 1 }, 0xD9);
}

void Context::drawText(Vec2 start, const std::string& text, size_t text_offset, size_t max_length)
{
    if (start.x + permitted_bounds.min.x >= permitted_bounds.max.x)
        return;
    for (size_t text_pos = text_offset; text_pos < text_offset + max_length; ++text_pos)
    {
        if (text_pos >= text.size())
            return;
        if (text[text_pos] == '\n')
            return;
        draw(start, text[text_pos]);
        ++start.x;
    }
}

std::vector<char>::const_iterator Context::begin() const
{
    return backing.begin();
}

std::vector<char>::const_iterator Context::end() const
{
    return backing.end();
}

Context::Context(Vec2 size, char fill_value)
{
    if (size.x <= 0 || size.y <= 0)
        throw std::invalid_argument("context size must be greater than zero");
    backing.resize(static_cast<size_t>(size.x) * static_cast<size_t>(size.y), fill_value);
    pitch = size.x;
    permitted_bounds = { { 0, 0 }, size };
}

void Context::setPermittedBounds(Vec2 min, Vec2 max)
{
    permitted_bounds = { min, max };
}

void Context::resize(Vec2 new_size, char fill_value)
{
    if (new_size.x <= 0 || new_size.y <= 0)
        throw std::invalid_argument("context size must be greater than zero");
    backing.resize(static_cast<size_t>(new_size.x) * static_cast<size_t>(new_size.y), fill_value);
    pitch = new_size.x;
    permitted_bounds = { { 0, 0 }, new_size };
}

#pragma endregion

void Window::render(Context& ctx) const
{
    if (!borderless)
    {
        ctx.drawBox(position, size);
        ctx.drawText(position + Vec2{ 1, size.y - 1 }, title, 0, size.x - 2);
        ctx.draw(position + Vec2{ size.x - 2, size.y - 1}, 0xFE);
    }
}

#pragma region COMPOSITOR

Window* Compositor::newWindow(std::string title, bool borderless, bool start_minimised)
{
    Window* window = new Window(title, borderless, start_minimised);
    windows.push_back(window);
    
    return window;
}

void Compositor::renderWindows()
{
    for (const Window* window : windows)
        if (!window->getMinimised())
            window->render(context);
}

void Compositor::setSize(Vec2 new_size)
{
    if (size == new_size)
        return;
    size = new_size;
    context.resize(new_size, ' ');
}

TerminalCompositor::TerminalCompositor() : Compositor()
{
    std::ios_base::sync_with_stdio(false);
#if defined(_WIN32)
    SetConsoleMode(GetStdHandle(STD_OUTPUT_HANDLE), ENABLE_VIRTUAL_TERMINAL_PROCESSING | ENABLE_PROCESSED_OUTPUT | ENABLE_WRAP_AT_EOL_OUTPUT);
#elif defined(__linux__)
    signal(SIGINT, linuxControlHandler);
    signal(SIGQUIT, linuxControlHandler);
    signal(SIGTSTP, linuxControlHandler);
    signal(SIGWINCH, linuxResizeHandler);
    termios new_termios;
    tcgetattr(STDIN_FILENO, &new_termios);
    tcgetattr(STDIN_FILENO, &original_termios);

    new_termios.c_iflag &= ~(IGNBRK | BRKINT | IXON);
    new_termios.c_lflag &= ~(ICANON | ECHO);
    new_termios.c_cc[VMIN] = 1;
    new_termios.c_cc[VSUSP] = 255;
    tcsetattr(STDIN_FILENO, TCSANOW, &new_termios);
#endif
    setSize(getScreenSize());
    setCursorVisible(false);
}

Vec2 TerminalCompositor::getScreenSize()
{
#if defined(_WIN32)
        CONSOLE_SCREEN_BUFFER_INFO info;
        GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &info);
        return Vec2{ (info.srWindow.Right - info.srWindow.Left) + 1, (info.srWindow.Bottom - info.srWindow.Top) + 1 };
#elif defined(__linux__)
        struct winsize size;
        ioctl(STDOUT_FILENO, TIOCGWINSZ, &size);
        return Vec2{ (int)size.ws_col, (int)size.ws_row };
#endif
}

void TerminalCompositor::setCursorVisible(bool visible)
{
#if defined(_WIN32)
        CONSOLE_CURSOR_INFO info;
        GetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &info);
        info.bVisible = visible;
        SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &info);
#elif defined(__linux__)
        if (visible) cout << ANSI_SHOW_CURSOR;
        else std::cout << ANSI_HIDE_CURSOR;
#endif
}

void TerminalCompositor::setCursorPosition(Vec2 position)
{
    std::cout << '\033' << '[' << position.y << ';' << position.x << 'H';
}

void TerminalCompositor::update()
{
    setSize(getScreenSize());
    // TODO: handle input
    renderWindows();
    std::string result(getSize().x * getSize().y, ' ');
    size_t off = 0;
    for (auto it = getContext().begin(); it != getContext().end(); ++it, ++off)
        result[off] = *it;
    std::cout << result;
    setCursorPosition({ 0, 0 });
}

#pragma endregion
