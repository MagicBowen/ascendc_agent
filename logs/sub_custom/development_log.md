# SubCustom Operator Development Log

## Operator Overview
- **Operator Name**: sub_custom
- **Function**: Element-wise subtraction between two input tensors
- **Supported Data Types**: float16, float32
- **Operation**: z = x - y
- **Development Date**: 2025-11-06

## Development Process

### 1. Initial Setup and Study
- Created operator directory structure in `/ops/sub_custom/`
- Studied sample add_custom operator implementation
- Examined Ascend C API documentation for Sub operation
- Created necessary subdirectories: scripts/, cmake/

### 2. Operator Prototype Definition
- Created `SubCustom.json` operator prototype file
- Defined two input tensors (x, y) and one output tensor (z)
- Supported both float16 and float32 data types
- Used ND format for flexible tensor shapes

### 3. Kernel Function Implementation
- Created `sub_custom.cpp` kernel function
- Implemented template-based KernelSub class supporting both float16 and float32
- Used Ascend C Sub API for element-wise subtraction
- Implemented SPMD programming model with 8 cores
- Used double buffering for efficient data processing
- Created two kernel functions: `sub_custom_half` and `sub_custom_float`

### 4. Host Application Development
- Created `main.cpp` host application
- Copied `data_utils.h` from sample for file I/O utilities
- Implemented support for both CPU debug and NPU execution modes
- Added command-line argument support for data type selection
- Handled memory allocation and data transfer using ACL APIs

### 5. Build Configuration
- Created `CMakeLists.txt` build configuration
- Copied cmake scripts from sample for CPU/NPU compilation
- Configured for Ascend910B SOC version
- Set up proper compilation flags and library linking

### 6. Test Infrastructure
- Created `scripts/gen_data.py` for test data generation
- Supports both float16 and float32 data types
- Generates random input data and golden output (x - y)
- Created `scripts/verify_result.py` for result validation
- Uses appropriate tolerances for each data type

### 7. Build and Test Script
- Created `run.sh` script for automated build and test
- Supports command-line arguments for run mode, SOC version, and data type
- Handles both CPU debug and NPU execution modes
- Automates build, test data generation, execution, and verification

## Development Challenges and Solutions

### Challenge 1: Template-based Kernel Implementation
- **Issue**: Needed to support both float16 and float32 data types
- **Solution**: Created template-based KernelSub class with separate kernel functions for each data type

### Challenge 2: Build Configuration
- **Issue**: Initial compilation errors due to unused variables
- **Solution**: Fixed variable declarations and ensured proper conditional compilation

### Challenge 3: Test Data Generation
- **Issue**: Needed to generate appropriate test data for subtraction
- **Solution**: Created data generation script that produces reasonable input ranges to avoid overflow

## Testing Results

### Float16 Test
- **Build Status**: Success
- **Execution Status**: Success
- **Verification**: All 16,384 elements matched golden data (0 errors)
- **Error Ratio**: 0.000000 (within tolerance of 0.001000)

### Float32 Test
- **Build Status**: Success
- **Execution Status**: Success
- **Verification**: All 16,384 elements matched golden data (0 errors)
- **Error Ratio**: 0.000000 (within tolerance of 0.000001)

## Files Created

### Core Files
- `/ops/sub_custom/sub_custom.cpp` - Kernel function implementation
- `/ops/sub_custom/main.cpp` - Host application
- `/ops/sub_custom/SubCustom.json` - Operator prototype
- `/ops/sub_custom/CMakeLists.txt` - Build configuration
- `/ops/sub_custom/run.sh` - Build and test script

### Support Files
- `/ops/sub_custom/data_utils.h` - File I/O utilities
- `/ops/sub_custom/scripts/gen_data.py` - Test data generation
- `/ops/sub_custom/scripts/verify_result.py` - Result verification
- `/ops/sub_custom/cmake/` - CMake build scripts

## API Usage

### Ascend C APIs Used
- `AscendC::Sub` - Element-wise subtraction
- `AscendC::DataCopy` - Memory data transfer
- `AscendC::TPipe` - Pipeline management
- `AscendC::TQue` - Queue management
- `AscendC::GlobalTensor` - Global memory tensor
- `AscendC::LocalTensor` - Local memory tensor

### ACL APIs Used (Host Side)
- `aclInit`, `aclFinalize` - ACL initialization
- `aclrtSetDevice`, `aclrtResetDevice` - Device management
- `aclrtMalloc`, `aclrtFree` - Memory allocation
- `aclrtMemcpy` - Memory transfer
- `aclrtCreateStream`, `aclrtDestroyStream` - Stream management

## Performance Characteristics
- **Data Size**: 8 × 2048 elements per tensor
- **Core Usage**: 8 cores
- **Tile Processing**: 8 tiles per core with double buffering
- **Memory Layout**: Optimized for vector processing

## Conclusion
The sub_custom operator has been successfully developed, built, and tested for both float16 and float32 data types. The implementation follows Huawei Ascend C best practices and demonstrates proper use of the SPMD programming model. All tests pass with zero errors, confirming the correctness of the element-wise subtraction implementation.