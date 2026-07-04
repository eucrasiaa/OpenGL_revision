#pragma once

#include <iostream>
#include <string_view>
#include <span>
#include <string>
#include <print>
#include <sstream>
#include <vector>
#include <concepts>
#include <functional>

namespace term {
  namespace ansi{
    // Cursor Movement
    constexpr std::string_view HOME          = "\033[H";
    constexpr std::string_view CLEAR_SCREEN  = "\033[2J";
    constexpr std::string_view CLEAR_LINE    = "\033[2K";
    constexpr std::string_view CLEAR_TO_EOL  = "\033[0K";
    constexpr std::string_view CURSOR_HIDE   = "\033[?25l";
    constexpr std::string_view CURSOR_SHOW   = "\033[?25h";
    constexpr std::string_view LINE_HOME     = "\033[1G";

    // Text Attributes
    constexpr std::string_view RESET         = "\033[0m";
    constexpr std::string_view BOLD          = "\033[1m";
    constexpr std::string_view DIM           = "\033[2m";
    constexpr std::string_view ITALIC        = "\033[3m";
    constexpr std::string_view UNDERLINE     = "\033[4m";
    constexpr std::string_view BLINK         = "\033[5m";
    constexpr std::string_view REVERSE       = "\033[7m";
    constexpr std::string_view STRIKETHROUGH = "\033[9m";

    // Standard Colors (Foreground)
    constexpr std::string_view FG_BLACK      = "\033[30m";
    constexpr std::string_view FG_RED        = "\033[31m";
    constexpr std::string_view FG_GREEN      = "\033[32m";
    constexpr std::string_view FG_YELLOW     = "\033[33m";
    constexpr std::string_view FG_BLUE       = "\033[34m";
    constexpr std::string_view FG_MAGENTA    = "\033[35m";
    constexpr std::string_view FG_CYAN       = "\033[36m";
    constexpr std::string_view FG_WHITE      = "\033[37m";

    // Bright Colors (Foreground)
    constexpr std::string_view FG_BRIGHT_BLACK   = "\033[90m";
    constexpr std::string_view FG_BRIGHT_RED     = "\033[91m";
    constexpr std::string_view FG_BRIGHT_GREEN   = "\033[92m";
    constexpr std::string_view FG_BRIGHT_YELLOW  = "\033[93m";
    constexpr std::string_view FG_BRIGHT_BLUE    = "\033[94m";
    constexpr std::string_view FG_BRIGHT_MAGENTA = "\033[95m";
    constexpr std::string_view FG_BRIGHT_CYAN    = "\033[96m";
    constexpr std::string_view FG_BRIGHT_WHITE   = "\033[97m";

    // Standard Colors (Background)
    constexpr std::string_view BG_BLACK      = "\033[40m";
    constexpr std::string_view BG_RED        = "\033[41m";
    constexpr std::string_view BG_GREEN      = "\033[42m";
    constexpr std::string_view BG_YELLOW     = "\033[43m";
    constexpr std::string_view BG_BLUE       = "\033[44m";
    constexpr std::string_view BG_MAGENTA    = "\033[45m";
    constexpr std::string_view BG_CYAN       = "\033[46m";
    constexpr std::string_view BG_WHITE      = "\033[47m";

    // Bright Colors (Background)
    constexpr std::string_view BG_BRIGHT_BLACK   = "\033[100m";
    constexpr std::string_view BG_BRIGHT_RED     = "\033[101m";
    constexpr std::string_view BG_BRIGHT_GREEN   = "\033[102m";
    constexpr std::string_view BG_BRIGHT_YELLOW  = "\033[103m";
    constexpr std::string_view BG_BRIGHT_BLUE    = "\033[104m";
    constexpr std::string_view BG_BRIGHT_MAGENTA = "\033[105m";
    constexpr std::string_view BG_BRIGHT_CYAN    = "\033[106m";
    constexpr std::string_view BG_BRIGHT_WHITE   = "\033[107m";
  }
  namespace utf8 {
    // Count the display width of a UTF-8 string (accounting for multi-byte chars)
    inline size_t display_width(std::string_view str) {
        size_t width = 0;
        for (size_t i = 0; i < str.size(); ) {
            unsigned char c = static_cast<unsigned char>(str[i]);
            if (c < 0x80) {
                // ASCII character
                if (c >= 0x20 || c == '\t') { // Printable or tab
                    width += (c == '\t') ? 4 : 1; // Assume tab = 4 spaces
                }
                ++i;
            } else if ((c & 0xE0) == 0xC0) {
                // 2-byte sequence
                width += 1; // Most CJK and special chars are 2 columns, but box chars are 1
                i += 2;
            } else if ((c & 0xF0) == 0xE0) {
                // 3-byte sequence (box drawing chars are here)
                width += 1;
                i += 3;
            } else if ((c & 0xF8) == 0xF0) {
                // 4-byte sequence
                width += 2; // Emoji etc typically 2 columns
                i += 4;
            } else {
                ++i; // Invalid, skip
            }
        }
        return width;
    }
    
    // Pad a string to a specific display width
    inline std::string pad_to_width(std::string_view str, size_t target_width, bool left_align = true) {
        size_t current_width = display_width(str);
        if (current_width >= target_width) {
            return std::string(str);
        }
        
        size_t padding = target_width - current_width;
        if (left_align) {
            return std::string(str) + std::string(padding, ' ');
        } else {
            return std::string(padding, ' ') + std::string(str);
        }
    }
    
    // Center text within a given width
    inline std::string center_in_width(std::string_view str, size_t width) {
        size_t str_width = display_width(str);
        if (str_width >= width) {
            return std::string(str);
        }
        
        size_t total_padding = width - str_width;
        size_t left_padding = total_padding / 2;
        size_t right_padding = total_padding - left_padding;
        
        return std::string(left_padding, ' ') + std::string(str) + std::string(right_padding, ' ');
    }
}
  namespace box {
    constexpr std::string_view HORIZONTAL = "─";
    constexpr std::string_view VERTICAL   = "│";
    constexpr std::string_view TOP_LEFT   = "┌";
    constexpr std::string_view TOP_RIGHT  = "┐";
    constexpr std::string_view BOT_LEFT   = "└";
    constexpr std::string_view BOT_RIGHT  = "┘";
    constexpr std::string_view T_DOWN     = "┬";
    constexpr std::string_view T_UP       = "┴";
    constexpr std::string_view T_RIGHT    = "├";
    constexpr std::string_view T_LEFT     = "┤";
    constexpr std::string_view CROSS      = "┼";
    
    // Double-line
    constexpr std::string_view DHORIZONTAL = "═";
    constexpr std::string_view DVERTICAL   = "║";
    constexpr std::string_view DTOP_LEFT   = "╔";
    constexpr std::string_view DTOP_RIGHT  = "╗";
    constexpr std::string_view DBOT_LEFT   = "╚";
    constexpr std::string_view DBOT_RIGHT  = "╝";
    constexpr std::string_view DT_DOUBLE_DOWN     = "╦";
    constexpr std::string_view DT_DOUBLE_UP       = "╩";
    constexpr std::string_view DT_DOUBLE_RIGHT    = "╠";
    constexpr std::string_view DT_DOUBLE_LEFT     = "╣";
    constexpr std::string_view DCROSS      = "╬";
    
    // Rounded
    constexpr std::string_view RTOP_LEFT   = "╭";
    constexpr std::string_view RTOP_RIGHT  = "╮";
    constexpr std::string_view RBOT_LEFT   = "╰";
    constexpr std::string_view RBOT_RIGHT  = "╯";
    
    // Heavy
    constexpr std::string_view HHORIZONTAL = "━";
    constexpr std::string_view HVERTICAL   = "┃";
    constexpr std::string_view HTOP_LEFT   = "┏";
    constexpr std::string_view HTOP_RIGHT  = "┓";
    constexpr std::string_view HBOT_LEFT   = "┗";
    constexpr std::string_view HBOT_RIGHT  = "┛";
    constexpr std::string_view HT_DOWN     = "┳";
    constexpr std::string_view HT_UP       = "┻";
    constexpr std::string_view HT_RIGHT    = "┣";
    constexpr std::string_view HT_LEFT     = "┫";
    constexpr std::string_view HCROSS      = "╋";
    
    // Block elements
    constexpr std::string_view BLOCK_FULL  = "█";
    constexpr std::string_view BLOCK_UPPER = "▀";
    constexpr std::string_view BLOCK_LOWER = "▄";
    constexpr std::string_view BLOCK_LEFT  = "▌";
    constexpr std::string_view BLOCK_RIGHT = "▐";
    constexpr std::string_view BLOCK_MID   = "▒";
    constexpr std::string_view BLOCK_LIGHT = "░";
    
    struct Style {
        std::string_view top_left, top_right, bot_left, bot_right;
        std::string_view horizontal, vertical;
        std::string_view t_down, t_up, t_right, t_left, cross;
    };
    
    constexpr Style SINGLE = {
        TOP_LEFT, TOP_RIGHT, BOT_LEFT, BOT_RIGHT,
        HORIZONTAL, VERTICAL,
        T_DOWN, T_UP, T_RIGHT, T_LEFT, CROSS
    };
    
    constexpr Style DOUBLE = {
        DTOP_LEFT, DTOP_RIGHT, DBOT_LEFT, DBOT_RIGHT,
        DHORIZONTAL, DVERTICAL,
        DT_DOUBLE_DOWN, DT_DOUBLE_UP, DT_DOUBLE_RIGHT, DT_DOUBLE_LEFT, DCROSS
    };
    
    constexpr Style ROUNDED = {
        RTOP_LEFT, RTOP_RIGHT, RBOT_LEFT, RBOT_RIGHT,
        HORIZONTAL, VERTICAL,
        T_DOWN, T_UP, T_RIGHT, T_LEFT, CROSS
    };
    
    constexpr Style HEAVY = {
        HTOP_LEFT, HTOP_RIGHT, HBOT_LEFT, HBOT_RIGHT,
        HHORIZONTAL, HVERTICAL,
        HT_DOWN, HT_UP, HT_RIGHT, HT_LEFT, HCROSS
    };
    
    constexpr Style ASCII = {
        "+", "+", "+", "+", "-", "|",
        "+", "+", "+", "+", "+"
    };
  }

class Output {
public:
    enum class Target { Stdout, String };
    
    explicit Output(Target target = Target::Stdout) : m_target(target) {}
    
    // Move cursor to absolute position (1-based)
    Output& move_to(int row, int col) {
        write("\033[{};{}H", row, col);
        return *this;
    }
    
    // Move cursor up/down by rows, optionally clearing lines
    Output& move_up(int rows = 1, bool clear = false) {
        write("\033[{}A", rows);
        if (clear) write("{}", ansi::CLEAR_LINE);
        return *this;
    }
    
    Output& move_down(int rows = 1, bool clear = false) {
        write("\033[{}B", rows);
        if (clear) write("{}", ansi::CLEAR_LINE);
        return *this;
    }
    
    Output& move_left(int cols = 1) {
        write("\033[{}D", cols);
        return *this;
    }
    
    Output& move_right(int cols = 1) {
        write("\033[{}C", cols);
        return *this;
    }
    
    // Clear operations
    Output& clear_line() {
        write("{}", ansi::CLEAR_LINE);
        return *this;
    }
    
    Output& clear_screen() {
        write("{}{}", ansi::CLEAR_SCREEN, ansi::HOME);
        return *this;
    }
    
    Output& clear_to_eol() {
        write("{}", ansi::CLEAR_TO_EOL);
        return *this;
    }
    
    // Cursor visibility
    Output& cursor_hide() {
        write("{}", ansi::CURSOR_HIDE);
        return *this;
    }
    
    Output& cursor_show() {
        write("{}", ansi::CURSOR_SHOW);
        return *this;
    }
    
    // Color setting (8-bit)
    Output& fg_8bit(int color) {
        write("\033[38;5;{}m", color);
        return *this;
    }
    
    Output& bg_8bit(int color) {
        write("\033[48;5;{}m", color);
        return *this;
    }
    
    // Color setting (24-bit RGB)
    Output& fg_rgb(int r, int g, int b) {
        write("\033[38;2;{};{};{}m", r, g, b);
        return *this;
    }
    
    Output& bg_rgb(int r, int g, int b) {
        write("\033[48;2;{};{};{}m", r, g, b);
        return *this;
    }
    
    // Text attributes
    Output& bold() { write("{}", ansi::BOLD); return *this; }
    Output& dim() { write("{}", ansi::DIM); return *this; }
    Output& italic() { write("{}", ansi::ITALIC); return *this; }
    Output& underline() { write("{}", ansi::UNDERLINE); return *this; }
    Output& reset() { write("{}", ansi::RESET); return *this; }
    
    // Raw text output
    template<typename... Args>
    Output& print(std::format_string<Args...> fmt, Args&&... args) {
        write(fmt, std::forward<Args>(args)...);
        return *this;
    }
    
    // Get the accumulated string (for string target)
    std::string str() const { return m_buffer.str(); }
    
    // Clear the string buffer
    void clear_buffer() { m_buffer.str(""); m_buffer.clear(); }
    
private:
    template<typename... Args>
    void write(std::format_string<Args...> fmt, Args&&... args) {
        auto formatted = std::format(fmt, std::forward<Args>(args)...);
        if (m_target == Target::Stdout) {
            std::cout << formatted;
        } else {
            m_buffer << formatted;
        }
    }
    
    Target m_target;
    std::ostringstream m_buffer;
};

// Convenience functions for direct stdout output
inline auto out() { return Output(Output::Target::Stdout); }
inline auto str_out() { return Output(Output::Target::String); }

// ──────────────────────── Box Builder ────────────────────────
class BoxBuilder {
public:
    BoxBuilder() = default;
    
    BoxBuilder& style(const box::Style& s) { m_style = &s; return *this; }
    BoxBuilder& width(int w) { m_width = w; return *this; }
    BoxBuilder& height(int h) { m_height = h; return *this; }
    BoxBuilder& title(std::string_view t) { m_title = t; return *this; }
    BoxBuilder& border_color(std::string_view c) { m_border_color = c; return *this; }
    BoxBuilder& title_color(std::string_view c) { m_title_color = c; return *this; }
    BoxBuilder& content_color(std::string_view c) { m_content_color = c; return *this; }
    
    BoxBuilder& add_line(std::string_view line) {
        m_lines.push_back(std::string(line));
        return *this;
    }
    
    template<typename... Args>
    BoxBuilder& add_line(std::format_string<Args...> fmt, Args&&... args) {
        m_lines.push_back(std::format(fmt, std::forward<Args>(args)...));
        return *this;
    }
    
    // Build to a string
    std::string build() const {
        const auto& s = m_style ? *m_style : box::SINGLE;
        int w = m_width > 0 ? m_width : calculate_width();
        std::string result;
        
        // Top border
        append_border_line(result, s.top_left, s.horizontal, s.top_right, w);
        
        // Title line (if present)
        if (!m_title.empty()) {
            result += m_border_color;
            result += s.vertical;
            result += ansi::RESET;
            result += m_title_color;
            // Use display width for centering
            result += utf8::center_in_width(m_title, w - 2);
            result += m_border_color;
            result += s.vertical;
            result += ansi::RESET;
            result += '\n';
            
            // Separator
            append_border_line(result, s.t_right, s.horizontal, s.t_left, w);
        }
        
        // Content lines
        int content_lines = m_height > 0 ? m_height : static_cast<int>(m_lines.size());
        for (int i = 0; i < content_lines; ++i) {
            result += m_border_color;
            result += s.vertical;
            result += ansi::RESET;
            result += m_content_color;
            
            if (i < static_cast<int>(m_lines.size())) {
                result += utf8::pad_to_width(m_lines[i], w - 2, true);
            } else {
                result += std::string(w - 2, ' ');
            }
            
            result += m_border_color;
            result += s.vertical;
            result += ansi::RESET;
            result += '\n';
        }
        
        // Bottom border
        append_border_line(result, s.bot_left, s.horizontal, s.bot_right, w);
        
        return result;
    }
    
    // Print directly to stdout
    void render() const {
        std::cout << build();
    }
    
    // Output to an Output object
    void render_to(Output& out) const {
        out.print("{}", build());
    }
    
private:
    void append_border_line(std::string& result, std::string_view left, 
                           std::string_view fill, std::string_view right, int width) const {
        result += m_border_color;
        result += left;
        // Fill with horizontal chars, accounting for their display width
        int remaining = width - 2; // Subtract left and right borders
        while (remaining > 0) {
            result += fill;
            --remaining;
        }
        result += right;
        result += ansi::RESET;
        result += '\n';
    }
    
    int calculate_width() const {
        int max_w = utf8::display_width(m_title);
        for (const auto& line : m_lines) {
            max_w = std::max(max_w, static_cast<int>(utf8::display_width(line)));
        }
        return max_w + 4; // Add padding and borders
    }
    
    const box::Style* m_style = &box::SINGLE;
    int m_width = 0;
    int m_height = 0;
    std::string m_title;
    std::string m_border_color;
    std::string m_title_color = std::string(ansi::BOLD);
    std::string m_content_color;
    std::vector<std::string> m_lines;
};

// ──────────────────────── Multi-Box Layout ────────────────────────
class MultiBox {
public:
    enum class Layout { Horizontal, Vertical, Grid };
    
    MultiBox& layout(Layout l) { m_layout = l; return *this; }
    MultiBox& columns(int cols) { m_columns = cols; return *this; }
    MultiBox& spacing(int s) { m_spacing = s; return *this; }
    
    MultiBox& add_box(BoxBuilder box) {
        m_boxes.push_back(std::move(box));
        return *this;
    }
    
    std::string build() const {
        if (m_boxes.empty()) return {};
        
        std::vector<std::string> box_strings;
        for (const auto& box : m_boxes) {
            box_strings.push_back(box.build());
        }
        
        switch (m_layout) {
            case Layout::Horizontal:
                return build_horizontal(box_strings);
            case Layout::Vertical:
                return build_vertical(box_strings);
            case Layout::Grid:
                return build_grid(box_strings);
        }
        return {};
    }
    
    void render() const {
        std::cout << build();
    }
    
private:
    std::string build_horizontal(const std::vector<std::string>& boxes) const {
        // Split each box into lines
        std::vector<std::vector<std::string>> box_lines;
        size_t max_lines = 0;
        
        for (const auto& box : boxes) {
            auto lines = split_lines(box);
            max_lines = std::max(max_lines, lines.size());
            box_lines.push_back(std::move(lines));
        }
        
        // Find max line lengths for each box to pad properly
        std::vector<size_t> box_widths;
        for (const auto& lines : box_lines) {
            size_t max_len = 0;
            for (const auto& line : lines) {
                max_len = std::max(max_len, utf8::display_width(line));
            }
            box_widths.push_back(max_len);
        }
        
        // Pad boxes to same height and width
        for (size_t i = 0; i < box_lines.size(); ++i) {
            auto& lines = box_lines[i];
            size_t target_width = box_widths[i];
            
            while (lines.size() < max_lines) {
                lines.push_back(std::string(target_width, ' '));
            }
            
            // Pad each line to target width
            for (auto& line : lines) {
                line = utf8::pad_to_width(line, target_width, true);
            }
        }
        
        // Combine horizontally
        std::string result;
        for (size_t line = 0; line < max_lines; ++line) {
            for (size_t box = 0; box < box_lines.size(); ++box) {
                result += box_lines[box][line];
                if (box < box_lines.size() - 1) {
                    result += std::string(m_spacing, ' ');
                }
            }
            result += '\n';
        }
        
        return result;
    }
    
    std::string build_vertical(const std::vector<std::string>& boxes) const {
        std::string result;
        for (size_t i = 0; i < boxes.size(); ++i) {
            result += boxes[i];
            if (i < boxes.size() - 1) {
                result += std::string(m_spacing, '\n');
            }
        }
        return result;
    }
    
    std::string build_grid(const std::vector<std::string>& boxes) const {
        // Simple grid layout - could be enhanced
        return build_vertical(boxes);
    }
    
    static std::vector<std::string> split_lines(const std::string& text) {
        std::vector<std::string> lines;
        std::istringstream stream(text);
        std::string line;
        while (std::getline(stream, line)) {
            lines.push_back(line);
        }
        return lines;
    }
    
    Layout m_layout = Layout::Vertical;
    int m_columns = 1;
    int m_spacing = 0;
    std::vector<BoxBuilder> m_boxes;
};

// ──────────────────────── Updated Performance Display ────────────────────────
inline void print_engine_perf(double fps, double total_ms, double update_ms, double render_ms) {
    auto perf_color = (total_ms > 16.67) ? ansi::FG_BRIGHT_RED : ansi::FG_BRIGHT_GREEN;
    double budget_percent = (total_ms / 16.67) * 100.0;
    
    BoxBuilder box;
    box.style(box::ROUNDED)
       .width(44)
       .title("ENGINE PERFORMANCE")
       .border_color(std::string(ansi::FG_BRIGHT_CYAN))
       .title_color(std::string(ansi::BOLD) + std::string(ansi::FG_BRIGHT_YELLOW))
       .content_color(std::string(ansi::FG_WHITE))
       .add_line("Frame Rate:     {} {:>8.2f} FPS", perf_color, fps)
       .add_line("Frame Budget:   {} {:>8.2f} ms{} ({:>5.1f}%)", 
                 perf_color, total_ms, ansi::DIM, budget_percent)
       .add_line("┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄")
       .add_line("  Core Update:  {} {:>8.2f} ms", ansi::FG_BRIGHT_BLUE, update_ms)
       .add_line("  GPU Render:   {} {:>8.2f} ms", ansi::FG_BRIGHT_MAGENTA, render_ms);
    
    out().move_to(1, 1)
         .clear_line()
         .print("{}", box.build());
    
    fflush(stdout);
}

// ──────────────────────── Debug Logger Integration Example ────────────────────────
class DebugDisplay {
public:
    void add_panel(std::string_view name, std::function<std::string()> content_fn) {
        m_panels.emplace_back(std::string(name), std::move(content_fn));
    }
    
    void render() {
        MultiBox layout;
        layout.layout(MultiBox::Layout::Horizontal).spacing(2);
        
        for (auto& [name, fn] : m_panels) {
            BoxBuilder box;
            box.style(box::SINGLE)
               .title(name)
               .border_color(std::string(ansi::FG_BRIGHT_BLUE))
               .add_line(fn());
            layout.add_box(std::move(box));
        }
        
        out().move_to(1, 1).clear_screen();
        layout.render();
        fflush(stdout);
    }
    
private:
    std::vector<std::pair<std::string, std::function<std::string()>>> m_panels;
};

} // namespace term

// ──────────────────────── Backward Compatibility Aliases ────────────────────────
#define TERM_CLEAR_ALL       "\033[2J"
#define TERM_CLEAR_LINE      "\033[2K"
#define TERM_CURSOR_HOME     "\033[H"
#define TERM_CURSOR_HIDE     "\033[?25l"
#define TERM_CURSOR_SHOW     "\033[?25h"
#define TERM_CURSOR_LINEHOME "\033[1G" 

#define TERM_RESET          "\033[0m"
#define TERM_BOLD           "\033[1m"
#define TERM_DIM            "\033[2m"
#define TERM_UNDERLINE      "\033[4m"
#define TERM_BLINK          "\033[5m"
#define TERM_REVERSE        "\033[7m"

#define TERM_FG_BLACK       "\033[30m"
#define TERM_FG_RED         "\033[31m"
#define TERM_FG_GREEN       "\033[32m"
#define TERM_FG_YELLOW      "\033[33m"
#define TERM_FG_BLUE        "\033[34m"
#define TERM_FG_MAGENTA     "\033[35m"
#define TERM_FG_CYAN        "\033[36m"
#define TERM_FG_WHITE       "\033[37m"

#define TERM_FG_B_BLACK     "\033[90m"
#define TERM_FG_B_RED       "\033[91m"
#define TERM_FG_B_GREEN     "\033[92m"
#define TERM_FG_B_YELLOW    "\033[93m"
#define TERM_FG_B_BLUE      "\033[94m"
#define TERM_FG_B_MAGENTA   "\033[95m"
#define TERM_FG_B_CYAN      "\033[96m"
#define TERM_FG_B_WHITE     "\033[97m"

#define TERM_BG_BLACK       "\033[40m"
#define TERM_BG_RED         "\033[41m"
#define TERM_BG_GREEN       "\033[42m"
#define TERM_BG_YELLOW      "\033[43m"
#define TERM_BG_BLUE        "\033[44m"
#define TERM_BG_MAGENTA     "\033[45m"
#define TERM_BG_CYAN        "\033[46m"
#define TERM_BG_WHITE       "\033[47m"

#define TERM_BG_B_BLACK     "\033[100m"
#define TERM_BG_B_RED       "\033[101m"
#define TERM_BG_B_GREEN     "\033[102m"
#define TERM_BG_B_YELLOW    "\033[103m"
#define TERM_BG_B_BLUE      "\033[104m"
#define TERM_BG_B_MAGENTA   "\033[105m"
#define TERM_BG_B_CYAN      "\033[106m"
#define TERM_BG_B_WHITE     "\033[107m"
