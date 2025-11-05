# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Project Overview

This is a Huawei CANN Ascend C operator development template for developing custom operators using Huawei's Ascend AI processors. The project provides a complete workflow for developing, building, and testing custom operators using Ascend C programming language.

## Key Commands

### Environment Setup

```bash
# Enter the Docker development environment
./env_setup.sh

# Inside the container, set environment variables
source /usr/local/Ascend/ascend-toolkit/set_env.sh
export ASCEND_INSTALL_PATH=/usr/local/Ascend/ascend-toolkit/latest
```

### Build and Test

```bash
# Give execution permission to run.sh (first time only)
chmod a+x run.sh

# Build and test in CPU mode (for container environment)
bash run.sh -r cpu -v Ascend910B

# Build and test in NPU mode (for actual hardware)
bash run.sh -r npu -v Ascend910B
```

### Manual Build Process

```bash
# Clean build directories
rm -rf build out

# Configure with CMake
cmake -B build -DRUN_MODE=cpu -DSOC_VERSION=Ascend910B -DCMAKE_BUILD_TYPE=Debug

# Build the project
cmake --build build -j

# Install to out directory
cmake --install build
```

## Architecture

### Directory Structure

- `samples/` - Example operator implementations
  - `add_custom/` - Sample addition operator
    - `add_custom.cpp` - Kernel function implementation
    - `main.cpp` - Host-side application code
    - `CMakeLists.txt` - Build configuration
    - `AddCustom.json` - Operator prototype definition
    - `scripts/` - Test data generation and verification
      - `gen_data.py` - Generate input and golden data
      - `verify_result.py` - Validate operator output

- `docs/` - Development documentation
  - `ascendc_guide.md` - Links to official Ascend C documentation

- `cmake/` - CMake module files for CPU and NPU builds

### Core Components

1. **Kernel Functions** (`add_custom.cpp`):
   - Implemented using Ascend C API with `__aicore__` decorator
   - Uses SPMD (Single Program Multiple Data) programming model
   - Manages tensor operations, memory buffers, and compute pipelines

2. **Host Application** (`main.cpp`):
   - Handles memory allocation and data transfer
   - Uses ACL (Ascend Computing Language) APIs
   - Supports both CPU debug and NPU execution modes

3. **Operator Definition** (`AddCustom.json`):
   - Defines operator inputs, outputs, and data types
   - Specifies tensor formats and parameter requirements

4. **Test Infrastructure**:
   - Python scripts for data generation and result verification
   - Binary file I/O for tensor data
   - Numerical precision validation with tolerance settings

## Development Workflow

1. **Define Operator**: Create JSON prototype file specifying inputs/outputs
2. **Implement Kernel**: Write Ascend C kernel function with proper memory management
3. **Create Host Code**: Write main application to call the kernel
4. **Build**: Use CMake to compile for target platform (CPU/NPU)
5. **Test**: Generate test data and verify operator correctness

## Important Notes

- The project uses Docker containers for development environment isolation
- Ascend C requires specific compiler flags and library linking
- CPU mode is for debugging, NPU mode is for actual hardware execution
- Data types are primarily float16 (half precision) for AI workloads
- Memory management follows Ascend C's GM (Global Memory) and LocalTensor patterns

## Documentation References

See `docs/ascendc_guide.md` for official Huawei Ascend C documentation links covering:
- Operator development examples
- ACL interface reference
- Ascend C API manual
- Vector operator development
- Debugging tools

## Build Modes

- `cpu`: CPU debug mode using simulator libraries
- `npu`: NPU execution mode for actual hardware
- `sim`: Simulator mode (alternative to CPU debug)

## Supported SOC Versions

- Ascend910A, Ascend910B
- Ascend310B1-B4, Ascend310P1, Ascend310P3
- Ascend910B1-B4