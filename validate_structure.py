#!/usr/bin/env python3
"""
libssh2_esp Structure Validation Script
Validates the dual framework implementation structure
"""

import os
import json
import sys

def print_status(message, status="INFO"):
    colors = {
        "INFO": "\033[0;32m",
        "WARNING": "\033[1;33m", 
        "ERROR": "\033[0;31m",
        "SUCCESS": "\033[1;32m"
    }
    reset = "\033[0m"
    print(f"{colors.get(status, '')}[{status}]{reset} {message}")

def validate_file_structure():
    """Validate that all required files exist"""
    print_status("Validating file structure...")
    
    required_files = [
        "library.json",
        "CMakeLists.txt",
        "include/libssh2_esp.h", 
        "include/framework_detection.h",
        "src/libssh2_esp.c",
        "examples/arduino/platformio.ini",
        "examples/arduino/src/main.cpp",
        "examples/espidf/CMakeLists.txt",
        "examples/espidf/main/main.c",
        "README.md"
    ]
    
    missing_files = []
    for file_path in required_files:
        if not os.path.exists(file_path):
            missing_files.append(file_path)
    
    if missing_files:
        print_status("Missing required files:", "ERROR")
        for file_path in missing_files:
            print(f"  - {file_path}")
        return False
    
    print_status("✅ All required files present", "SUCCESS")
    return True

def validate_library_json():
    """Validate library.json structure"""
    print_status("Validating library.json...")
    
    try:
        with open('library.json', 'r') as f:
            data = json.load(f)
        
        required_fields = ['name', 'version', 'frameworks', 'platforms']
        missing_fields = [field for field in required_fields if field not in data]
        
        if missing_fields:
            print_status(f"Missing required fields: {missing_fields}", "ERROR")
            return False
        
        # Check frameworks
        frameworks = data.get('frameworks', [])
        if 'arduino' not in frameworks or 'espidf' not in frameworks:
            print_status("Missing required frameworks: arduino, espidf", "ERROR")
            return False
        
        # Check platforms
        platforms = data.get('platforms', [])
        if 'espressif32' not in platforms:
            print_status("Missing required platform: espressif32", "ERROR")
            return False
        
        print_status("✅ library.json is valid", "SUCCESS")
        return True
        
    except json.JSONDecodeError as e:
        print_status(f"Invalid JSON in library.json: {e}", "ERROR")
        return False
    except Exception as e:
        print_status(f"Error reading library.json: {e}", "ERROR")
        return False

def validate_submodule():
    """Validate libssh2 submodule"""
    print_status("Validating libssh2 submodule...")
    
    if not os.path.exists('libssh2'):
        print_status("libssh2 directory not found", "ERROR")
        return False
    
    if not os.path.exists('libssh2/include/libssh2.h'):
        print_status("libssh2 submodule not properly initialized", "ERROR")
        print_status("Run: git submodule update --init --recursive", "INFO")
        return False
    
    print_status("✅ libssh2 submodule properly initialized", "SUCCESS")
    return True

def validate_examples():
    """Validate example structure"""
    print_status("Validating examples...")
    
    # Arduino example
    arduino_files = [
        "examples/arduino/platformio.ini",
        "examples/arduino/src/main.cpp"
    ]
    
    # ESP-IDF example  
    espidf_files = [
        "examples/espidf/CMakeLists.txt",
        "examples/espidf/main/CMakeLists.txt",
        "examples/espidf/main/main.c"
    ]
    
    all_example_files = arduino_files + espidf_files
    missing_example_files = [f for f in all_example_files if not os.path.exists(f)]
    
    if missing_example_files:
        print_status("Missing example files:", "ERROR")
        for file_path in missing_example_files:
            print(f"  - {file_path}")
        return False
    
    print_status("✅ All example files present", "SUCCESS")
    return True

def validate_headers():
    """Basic validation of header files"""
    print_status("Validating header files...")
    
    headers = [
        "include/framework_detection.h",
        "include/libssh2_esp.h"
    ]
    
    for header in headers:
        if not os.path.exists(header):
            print_status(f"Header file missing: {header}", "ERROR")
            return False
        
        # Check for basic header guard
        with open(header, 'r') as f:
            content = f.read()
            if '#ifndef' not in content or '#define' not in content or '#endif' not in content:
                print_status(f"Header guard missing in {header}", "WARNING")
    
    print_status("✅ Header files structure validated", "SUCCESS")
    return True

def main():
    print("=== libssh2_esp Structure Validation ===")
    print("Validating dual framework implementation...\n")
    
    # Change to script directory
    script_dir = os.path.dirname(os.path.abspath(__file__))
    os.chdir(script_dir)
    
    tests = [
        ("File Structure", validate_file_structure),
        ("library.json", validate_library_json), 
        ("libssh2 Submodule", validate_submodule),
        ("Examples", validate_examples),
        ("Headers", validate_headers)
    ]
    
    passed = 0
    total = len(tests)
    
    for test_name, test_func in tests:
        print(f"\n--- {test_name} ---")
        if test_func():
            passed += 1
        else:
            print_status(f"❌ {test_name} validation failed", "ERROR")
    
    print(f"\n=== Validation Summary ===")
    print(f"Tests passed: {passed}/{total}")
    
    if passed == total:
        print_status("🎉 All validations passed!", "SUCCESS")
        print_status("libssh2_esp dual framework implementation is ready!", "SUCCESS")
        print("\nNext steps:")
        print("1. Test with actual Arduino/PlatformIO project")
        print("2. Test with actual ESP-IDF project")
        print("3. Verify SSH functionality with real server")
        return 0
    else:
        print_status(f"❌ {total - passed} validation(s) failed", "ERROR")
        return 1

if __name__ == "__main__":
    sys.exit(main())