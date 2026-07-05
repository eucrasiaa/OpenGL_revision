#pragma once

// #define ANSI_USE_CONST_CHAR  // drop <string_view> -> use const char*
// #define ANSI_NO_FUNCTS       // drop <cstdio> + all print functions

#ifndef ANSI_USE_CONST_CHAR
    #include <string_view>
    // Modern default: zero-overhead string views
    #define ANSI_STR_TYPE constexpr std::string_view
#else
    //legacy
    #define ANSI_STR_TYPE constexpr const char*
#endif

#ifndef ANSI_NO_FUNCTS
    #include <cstdio> 
#endif

namespace term {

    namespace mv { 
        ANSI_STR_TYPE CLEAR_ALL       = "\033[2J";
        ANSI_STR_TYPE CLEAR_LINE      = "\033[2K";
        ANSI_STR_TYPE CLEAR_TO_EOL    = "\033[0K";
        ANSI_STR_TYPE CURSOR_HOME     = "\033[H";
        ANSI_STR_TYPE CURSOR_HIDE     = "\033[?25l";
        ANSI_STR_TYPE CURSOR_SHOW     = "\033[?25h";
        ANSI_STR_TYPE CURSOR_LINEHOME = "\033[1G"; 
    }

    namespace format {
        ANSI_STR_TYPE RESET         = "\033[0m";
        ANSI_STR_TYPE BOLD          = "\033[1m";
        ANSI_STR_TYPE DIM           = "\033[2m";
        ANSI_STR_TYPE ITALIC        = "\033[3m";
        ANSI_STR_TYPE UNDERLINE     = "\033[4m";
        ANSI_STR_TYPE BLINK         = "\033[5m";
        ANSI_STR_TYPE REVERSE       = "\033[7m";
        ANSI_STR_TYPE STRIKETHROUGH = "\033[9m";
    }

    namespace fg {
        ANSI_STR_TYPE BLACK   = "\033[30m";
        ANSI_STR_TYPE RED     = "\033[31m";
        ANSI_STR_TYPE GREEN   = "\033[32m";
        ANSI_STR_TYPE YELLOW  = "\033[33m";
        ANSI_STR_TYPE BLUE    = "\033[34m";
        ANSI_STR_TYPE MAGENTA = "\033[35m";
        ANSI_STR_TYPE CYAN    = "\033[36m";
        ANSI_STR_TYPE WHITE   = "\033[37m";

        ANSI_STR_TYPE B_BLACK   = "\033[90m";
        ANSI_STR_TYPE B_RED     = "\033[91m";
        ANSI_STR_TYPE B_GREEN   = "\033[92m";
        ANSI_STR_TYPE B_YELLOW  = "\033[93m";
        ANSI_STR_TYPE B_BLUE    = "\033[94m";
        ANSI_STR_TYPE B_MAGENTA = "\033[95m";
        ANSI_STR_TYPE B_CYAN    = "\033[96m";
        ANSI_STR_TYPE B_WHITE   = "\033[97m";
    }

    namespace bg {
        ANSI_STR_TYPE BLACK   = "\033[40m";
        ANSI_STR_TYPE RED     = "\033[41m";
        ANSI_STR_TYPE GREEN   = "\033[42m";
        ANSI_STR_TYPE YELLOW  = "\033[43m";
        ANSI_STR_TYPE BLUE    = "\033[44m";
        ANSI_STR_TYPE MAGENTA = "\033[45m";
        ANSI_STR_TYPE CYAN    = "\033[46m";
        ANSI_STR_TYPE WHITE   = "\033[47m";
    }

    #ifndef ANSI_NO_FUNCTS
    #ifndef ANSI_USE_CONST_CHAR
    inline void print(std::string_view sv) {
        std::fwrite(sv.data(), sizeof(char), sv.size(), stdout);
    }
    #else
    inline void print(const char* str) {
        std::fputs(str, stdout);
    }
    #endif

    inline void clear_all()   { print(mv::CLEAR_ALL); }
    inline void clear_line()  { print(mv::CLEAR_LINE); }
    inline void cursor_home() { print(mv::CURSOR_HOME); }
    inline void move_to(int row, int col) {
        std::fprintf(stdout, "\033[%d;%dH", row, col);
    }
    inline void move_up(int rows, bool clear = false) {
        if (clear) {
            std::fprintf(stdout, "\033[%dA\033[2K", rows);
        } else {
            std::fprintf(stdout, "\033[%dA", rows);
        }
    }
    inline void move_down(int rows, bool clear = false) {
        if (clear) {
            std::fprintf(stdout, "\033[%dB\033[2K", rows);
        } else {
            std::fprintf(stdout, "\033[%dB", rows);
        }
    }
    // 8 & 24 bit with fprintf
    inline void set_fg_8bit(int color_id) {
        std::fprintf(stdout, "\033[38;5;%dm", color_id);
    }

    inline void set_bg_8bit(int color_id) {
        std::fprintf(stdout, "\033[48;5;%dm", color_id);
    }

    inline void set_fg_rgb(int r, int g, int b) {
        std::fprintf(stdout, "\033[38;2;%d;%d;%dm", r, g, b);
    }

    inline void set_bg_rgb(int r, int g, int b) {
        std::fprintf(stdout, "\033[48;2;%d;%d;%dm", r, g, b);
    }

#endif // ANSI_NO_FUNCTS

} // namespace term
#undef ANSI_STR_TYPE
