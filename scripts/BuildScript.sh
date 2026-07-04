#!/bin/bash
set -e

RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m' 

PROJECT_NAME="OpenGL_Revision"
BUILD_DIR="build"
BACKEND="SDL2"
BUILD_TYPE="Debug"
ENABLE_ASAN="OFF"
ENABLE_UBSAN="OFF"
GENERATOR=""

show_help() {
    cat << EOF
Usage: ./build.sh [COMMAND] [OPTIONS]

Commands:
    d, debug        - Debug build (default)
    r, release      - Release build (optimized)
    f, frelease     - Fast Release (no debug symbols)
    a, asan         - Debug + AddressSanitizer
    u, ubsan        - Debug + UndefinedBehaviorSanitizer
    cov, coverage   - Release + Coverage
    run             - Build and run
    rc              - Run last successful build (no rebuild)
    gdb             - Run with GDB
    valgrind        - Run with Valgrind
    clean           - Clean build directory
    k, kill         - Kill running process
    help            - Show this help

Options:
    --backend=SDL2|RAYLIB    - Select graphics backend (default: SDL2)
    --generator=Ninja|Make   - Force specific generator

Examples:
    ./build.sh d --backend=RAYLIB
    ./build.sh run --generator=Ninja
EOF
}

parse_args() {
    while [[ $# -gt 0 ]]; do
        case $1 in
            d|debug)
                BUILD_TYPE="Debug"
                ENABLE_ASAN="OFF"
                ENABLE_UBSAN="OFF"
                shift
                ;;
            r|release)
                BUILD_TYPE="Release"
                ENABLE_ASAN="OFF"
                ENABLE_UBSAN="OFF"
                shift
                ;;
            f|frelease)
                BUILD_TYPE="Release"
                ENABLE_ASAN="OFF"
                ENABLE_UBSAN="OFF"
                shift
                ;;
            a|asan)
                BUILD_TYPE="Debug"
                ENABLE_ASAN="ON"
                ENABLE_UBSAN="OFF"
                shift
                ;;
            u|ubsan)
                BUILD_TYPE="Debug"
                ENABLE_ASAN="OFF"
                ENABLE_UBSAN="ON"
                shift
                ;;
            cov|coverage)
                BUILD_TYPE="Debug"
                ENABLE_ASAN="OFF"
                ENABLE_UBSAN="OFF"
                ENABLE_COVERAGE="ON"
                shift
                ;;
            run)
                BUILD_TYPE="Debug"
                ENABLE_ASAN="OFF"
                ENABLE_UBSAN="OFF"
                DO_RUN=1
                shift
                ;;
            rc)
                DO_RC=1
                shift
                ;;
            gdb)
                DO_GDB=1
                shift
                ;;
            valgrind)
                DO_VALGRIND=1
                shift
                ;;
            clean)
                DO_CLEAN=1
                shift
                ;;
            k|kill)
                DO_KILL=1
                shift
                ;;
            --backend=*)
                BACKEND="${1#*=}"
                shift
                ;;
            --generator=*)
                GENERATOR="-G ${1#*=}"
                shift
                ;;
            help)
                show_help
                exit 0
                ;;
            *)
                echo -e "${RED}Unknown command: $1${NC}"
                show_help
                exit 1
                ;;
        esac
    done
}

configure_cmake() {
    echo -e "${GREEN}Configuring CMake...${NC}"
    echo "  Build Type: $BUILD_TYPE"
    echo "  Backend: $BACKEND"
    echo "  ASan: $ENABLE_ASAN"
    echo "  UBSan: $ENABLE_UBSAN"
    
    mkdir -p $BUILD_DIR
    cd $BUILD_DIR
    
    cmake $GENERATOR .. \
        -DCMAKE_BUILD_TYPE=$BUILD_TYPE \
        -DBACKEND=$BACKEND \
        -DENABLE_ASAN=$ENABLE_ASAN \
        -DENABLE_UBSAN=$ENABLE_UBSAN \
        -DENABLE_COVERAGE=$ENABLE_COVERAGE
    
    cd ..
}

parse_args "$@"

if [ "$DO_CLEAN" = "1" ]; then
    echo -e "${YELLOW}Cleaning build directory...${NC}"
    rm -rf $BUILD_DIR
    exit 0
fi

if [ "$DO_KILL" = "1" ]; then
    echo -e "${YELLOW}Killing $PROJECT_NAME...${NC}"
    pkill $PROJECT_NAME 2>/dev/null || true
    exit 0
fi

if [ "$DO_RC" = "1" ]; then
    if [ -f "$BUILD_DIR/$PROJECT_NAME" ] || [ -f "$BUILD_DIR/$PROJECT_NAME.exe" ]; then
        exec ./$BUILD_DIR/$PROJECT_NAME
    else
        echo -e "${RED}Binary not found. Please build first.${NC}"
        exit 1
    fi
fi

if [ ! -d "$BUILD_DIR" ]; then
    configure_cmake
fi

echo -e "${GREEN}Building...${NC}"
cmake --build $BUILD_DIR --config $BUILD_TYPE

if [ "$DO_VALGRIND" = "1" ]; then
    echo -e "${GREEN}Running Valgrind...${NC}"
    cd $BUILD_DIR
    valgrind --leak-check=full --track-origins=yes ./$PROJECT_NAME
    cd ..
elif [ "$DO_GDB" = "1" ]; then
    echo -e "${GREEN}Running GDB...${NC}"
    cd $BUILD_DIR
    gdb ./$PROJECT_NAME
    cd ..
elif [ "$DO_RUN" = "1" ]; then
    echo -e "${GREEN}Running $PROJECT_NAME...${NC}"
    cd $BUILD_DIR
    exec ./$PROJECT_NAME
else
    echo -e "${GREEN}Build complete!${NC}"
    echo "To run: ./build.sh run"
    echo "To debug: ./build.sh gdb"
fi
