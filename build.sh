#!/bin/bash

# Moildev ImGui Build & Run Helper Script

set -e

PROJECT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
BUILD_DIR="$PROJECT_DIR/build"

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

# Default action
ACTION=${1:-"run"}

# Functions
print_usage() {
    echo "Usage: $0 [action]"
    echo ""
    echo "Actions:"
    echo "  build           - Build the project only"
    echo "  run             - Run the application (requires already built)"
    echo "  build-run       - Build and run in one command"
    echo "  rebuild         - Clean and build from scratch"
    echo "  clean           - Remove build directory"
    echo "  help            - Show this message"
    echo ""
    echo "Examples:"
    echo "  $0 build        # Build only"
    echo "  $0 run          # Run existing build"
    echo "  $0 build-run    # Build and run"
}

build_project() {
    echo -e "${BLUE}=== Building Moildev ImGui ===${NC}"
    
    if [ ! -d "$BUILD_DIR" ]; then
        echo -e "${YELLOW}Creating build directory...${NC}"
        mkdir -p "$BUILD_DIR"
    fi
    
    cd "$BUILD_DIR"
    
    echo -e "${YELLOW}Running CMake...${NC}"
    cmake .. -DCMAKE_BUILD_TYPE=Release
    
    echo -e "${YELLOW}Compiling (using 4 parallel jobs)...${NC}"
    make -j4
    
    echo -e "${GREEN}✓ Build completed successfully${NC}"
}

run_application() {
    if [ ! -f "$BUILD_DIR/unicorn" ]; then
        echo -e "${RED}✗ Executable not found. Please build first: $0 build${NC}"
        exit 1
    fi
    
    echo -e "${BLUE}=== Running Moildev ImGui ===${NC}"
    cd "$BUILD_DIR"
    ./unicorn
}

rebuild_project() {
    echo -e "${BLUE}=== Rebuilding Moildev ImGui ===${NC}"
    
    if [ -d "$BUILD_DIR" ]; then
        echo -e "${YELLOW}Removing old build directory...${NC}"
        rm -rf "$BUILD_DIR"
    fi
    
    build_project
    echo -e "${GREEN}✓ Rebuild completed${NC}"
}

clean_project() {
    echo -e "${BLUE}=== Cleaning build directory ===${NC}"
    
    if [ -d "$BUILD_DIR" ]; then
        rm -rf "$BUILD_DIR"
        echo -e "${GREEN}✓ Clean completed${NC}"
    else
        echo -e "${YELLOW}Build directory doesn't exist${NC}"
    fi
}

# Main logic
case "$ACTION" in
    build)
        build_project
        ;;
    run)
        run_application
        ;;
    build-run)
        build_project
        echo ""
        run_application
        ;;
    rebuild)
        rebuild_project
        ;;
    clean)
        clean_project
        ;;
    help|--help|-h)
        print_usage
        ;;
    *)
        echo -e "${RED}✗ Unknown action: $ACTION${NC}"
        print_usage
        exit 1
        ;;
esac

exit 0
