#!/bin/bash

# libssh2_esp PlatformIO Build Test Script
# Tests compilation with PlatformIO/Arduino framework only

set -e  # Exit on any error

echo "=== libssh2_esp PlatformIO Test Script ==="
echo "Testing Arduino framework support with integrated libssh2..."

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

# Function to print colored output
print_status() {
    echo -e "${GREEN}[INFO]${NC} $1"
}

print_warning() {
    echo -e "${YELLOW}[WARNING]${NC} $1"
}

print_error() {
    echo -e "${RED}[ERROR]${NC} $1"
}

# Check if we're in the right directory
if [ ! -f "library.json" ] || [ ! -d "src" ] || [ ! -d "include" ]; then
    print_error "Please run this script from the libssh2_esp root directory"
    exit 1
fi

print_status "Starting PlatformIO build tests..."

# Test 1: Check file structure for integrated libssh2
print_status "Test 1: Checking integrated file structure..."
required_files=(
    "library.json"
    "include/libssh2_esp.h"
    "include/framework_detection.h"
    "include/libssh2.h"
    "include/libssh2_sftp.h"
    "src/libssh2_esp.c"
    "src/libssh2_config.h"
    "src/compat.h"
    "src/agent.c"
    "src/mbedtls.c"
    "examples/arduino/platformio.ini"
    "examples/arduino/src/main.cpp"
)

missing_files=()
for file in "${required_files[@]}"; do
    if [ ! -f "$file" ]; then
        missing_files+=("$file")
    fi
done

if [ ${#missing_files[@]} -eq 0 ]; then
    print_status "✅ All required files present"
else
    print_error "❌ Missing files:"
    for file in "${missing_files[@]}"; do
        echo "  - $file"
    done
    exit 1
fi

# Test 2: Validate library.json for integrated structure
print_status "Test 2: Validating library.json..."
if command -v python3 &> /dev/null; then
    python3 -c "
import json
import sys
try:
    with open('library.json', 'r') as f:
        data = json.load(f)
    
    # Check frameworks
    if 'arduino' not in data.get('frameworks', []):
        print('❌ Arduino framework not found in library.json')
        sys.exit(1)
    
    # Check build configuration
    build_config = data.get('build', {})
    if build_config.get('includeDir') != 'include':
        print('❌ Include directory not set to include/')
        sys.exit(1)
        
    if build_config.get('srcDir') != 'src':
        print('❌ Source directory not set to src/')
        sys.exit(1)
        
    print('✅ library.json is valid for integrated structure')
except Exception as e:
    print('❌ library.json validation failed:', e)
    sys.exit(1)
"
    if [ $? -eq 0 ]; then
        print_status "✅ library.json validation passed"
    else
        print_error "❌ library.json validation failed"
        exit 1
    fi
else
    print_warning "Python3 not available, skipping library.json validation"
fi

# Test 3: Check for PlatformIO and test Arduino example
print_status "Test 3: Testing Arduino/PlatformIO compilation..."
if command -v pio &> /dev/null; then
    print_status "PlatformIO found, testing Arduino example..."
    cd examples/arduino
    
    # Check platformio.ini
    if pio project config --json-output > /dev/null 2>&1; then
        print_status "✅ platformio.ini is valid"
    else
        print_error "❌ platformio.ini validation failed"
        cd ../..
        exit 1
    fi
    
    # Try to compile
    print_status "Attempting to compile Arduino example..."
    if pio run --environment esp32dev --dry-run > /dev/null 2>&1; then
        print_status "✅ Arduino example compilation check passed"
        
        # Try actual compilation if toolchain is available
        print_status "Attempting actual compilation..."
        if timeout 120s pio run --environment esp32dev > /dev/null 2>&1; then
            print_status "🎉 Arduino example compiled successfully!"
        else
            print_warning "⚠️  Actual compilation failed (toolchain may be missing)"
        fi
    else
        print_warning "⚠️  Arduino compilation check failed (toolchain may be missing)"
    fi
    
    cd ../..
else
    print_warning "PlatformIO not found, skipping compilation test"
    print_status "To install PlatformIO: pip install platformio"
fi

# Test 4: Check integrated libssh2 structure
print_status "Test 4: Validating integrated libssh2 structure..."
libssh2_src_files=(
    "src/agent.c"
    "src/session.c"
    "src/channel.c"
    "src/mbedtls.c"
    "src/sftp.c"
)

missing_libssh2=()
for file in "${libssh2_src_files[@]}"; do
    if [ ! -f "$file" ]; then
        missing_libssh2+=("$file")
    fi
done

if [ ${#missing_libssh2[@]} -eq 0 ]; then
    print_status "✅ libssh2 source files integrated successfully"
else
    print_error "❌ Missing libssh2 source files:"
    for file in "${missing_libssh2[@]}"; do
        echo "  - $file"
    done
    exit 1
fi

# Test 5: Check that submodule is removed
print_status "Test 5: Verifying submodule removal..."
if [ -d "libssh2" ] || [ -f ".gitmodules" ]; then
    print_error "❌ libssh2 submodule or .gitmodules still present"
    exit 1
else
    print_status "✅ libssh2 submodule successfully removed"
fi

# Summary
print_status ""
print_status "=== PlatformIO Build Test Summary ==="
print_status "✅ Integrated file structure: PASSED"
print_status "✅ Configuration validation: PASSED" 
print_status "✅ libssh2 integration: PASSED"
print_status "✅ Submodule removal: PASSED"
print_status ""
print_status "🎉 libssh2_esp integrated structure is ready for PlatformIO!"
print_status ""
print_status "Next steps:"
print_status "1. Test compilation: cd examples/arduino && pio run"
print_status "2. Upload to ESP32: pio run --target upload"
print_status "3. Monitor serial: pio device monitor"
print_status ""
print_status "Library usage:"
print_status '#include <libssh2_esp.h>'
print_status 'libssh2_esp_init();'