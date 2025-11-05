---
description: Develop a Huawei CANN Ascend C operator using the [operator development agent](../agents/op-developer.md)
---

## Usage

```bash
/develop [operator_name] [optional: operator_description]
```

## Description

This command launches the operator development agent to create a complete Huawei CANN Ascend C operator from scratch. The agent will:

- Create operator prototype definition (JSON)
- Implement kernel function using Ascend C
- Develop host application for kernel invocation
- Configure build system with CMake
- Create test infrastructure with data generation and verification
- Build and test the operator in Docker environment
- Record all development activities for complexity evaluation

## Examples

```bash
# Develop a simple addition operator
/develop add_custom "Element-wise addition operator for two tensors"

# Develop a matrix multiplication operator
/develop matmul_custom "Matrix multiplication operator for AI workloads"

# Develop a convolution operator
/develop conv_custom "2D convolution operator for neural networks"
```

## Parameters

- **operator_name**: Name of the operator to develop (will be used for directory names and file prefixes)
- **operator_description**: Optional description of the operator's functionality and use cases

## Output

- Creates operator in `ops/[operator_name]/` directory
- Records development process in `logs/[operator_name]/` directory
- Builds and tests the operator in Docker environment
- Provides completion status and any encountered issues

## Development Process

1. **Operator Definition**: Creates JSON prototype file
2. **Kernel Implementation**: Writes Ascend C kernel function with `__aicore__` decorator
3. **Host Application**: Creates main.cpp for kernel invocation and memory management
4. **Build Configuration**: Sets up CMakeLists.txt with proper compilation flags
5. **Test Infrastructure**: Creates Python scripts for data generation and result verification
6. **Build & Test**: Executes build process and runs tests in Docker environment

## Requirements

- Docker environment available via `./env_setup.sh`
- Project follows Huawei CANN Ascend C development patterns
- All generated files follow project directory structure standards