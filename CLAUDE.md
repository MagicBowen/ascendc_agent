# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Project Overview

This is a Huawei CANN Ascend C operator development project for developing custom operators using Huawei's Ascend AI processors. The project provides a complete workflow for developing, building, and testing custom operators using Ascend C programming language.

## Directory Structure

- `samples/` - Example for operator developer
  - `add_custom/` - Sample addition operator
    - `add_custom.cpp` - Kernel function implementation
    - `main.cpp` - Host-side application code
    - `CMakeLists.txt` - Build configuration
    - `AddCustom.json` - Operator prototype definition
    - `scripts/` - Test data generation and verification
      - `gen_data.py` - Generate input and golden data
      - `verify_result.py` - Validate operator output

- `docs/` - Development documentation for agents
  - `ascendc_guide.md` - Links to official Ascend C documentation
  - `ascendc_apis.md` - Ascend C device APIs that kernel function could use

- `ops/` - Agent-developed operators, each in its own subdirectory

- `logs/` - Recording the operator developing process in this folder, each in its own subdirectory, used by evaluation subagent to analyse later.

- `tests/` - User records some test input/output information in this folder; the Agent doesn't need to care.

- `env_setup.sh` - This shell script help agent enter the docker env for building and testing.

## Ascend C Operator Development Guide

### Directory Structure Requirements

- All developed operators must be placed in `ops/` directory
- Each operator in its own subdirectory (e.g., `ops/my_operator/`)
- Each operator directory must contain:
  - Kernel function (.cpp)
  - Host application (main.cpp)
  - Operator prototype (.json)
  - CMakeLists.txt (include related cmake scripts in cmake folder)
  - run.sh script (only execute in docker env)
  - scripts/ directory with test data generation and verification

### Important Notes

- **Operator Organization**: All agent-developed operators must be placed in the `ops/` directory, each in its own subdirectory
- **Testing**: Each operator directory must contain its own `run.sh` script for building and testing;
- **No Root Scripts**: Do not create test scripts in the project root directory
- **Docker Environment**: The project uses Docker containers for development environment isolation, must run build and test through `run.sh` of operator in docker env (according `env_setup.sh` in project root).

### Environment Setup

- Use Docker development environment for build and test, according `./env_setup.sh`
- Set environment variables inside container:
  ```bash
  source /usr/local/Ascend/ascend-toolkit/set_env.sh
  export ASCEND_INSTALL_PATH=/usr/local/Ascend/ascend-toolkit/latest
  ```

### Build and Test Commands

Each operator has a `run.sh`, using it for build and test. This script can only be executed in docker env.

```bash
# Inside operator directory
chmod a+x run.sh
bash run.sh -r cpu -v Ascend910B
```

### Documentation References

See `docs/ascendc_guide.md` for official Huawei Ascend C documentation links covering:
- Operator development examples
- ACL interface reference
- Ascend C API manual
- Vector operator development
- Debugging tools

See `docs/ascendc_apis.md` for Ascend C kernel basic APIs.

### Development Process

#### 1. Operator Definition
- Create operator prototype JSON file defining inputs, outputs, and data types
- Reference `samples/add_custom/AddCustom.json` for format
- Design the kernel function implementation logic of the operator, determine which mathematical formulas to use, and what underlying Ascend C APIs are needed to implement the corresponding algorithm. Search the API manual to match all the corresponding APIs.

#### 2. Kernel Function Implementation
- Write Ascend C kernel functions using `__aicore__` decorator
- Implement SPMD programming model
- Manage tensor operations, memory buffers, and compute pipelines
- Reference `samples/add_custom/add_custom.cpp` for implementation patterns
- Kernel function should be implemented by using Ascend C API. All callable Ascend C APIs, along with their parameters and usage, need to be WebFetch/WebSearch in the related URL in `docs/ascendc_apis.md`; you must not guess.

#### 3. Host Application Development
- Create main application to call the kernel
- Handle memory allocation and data transfer using ACL APIs
- Support both CPU debug and NPU execution modes
- Reference `samples/add_custom/main.cpp` for implementation patterns
- All callable ACL APIs, along with their parameters and usage, need to be WebFetch/WebSearch in the related URL in `docs/ascendc_guide.md`; you must not guess.

#### 4. Build Configuration
- Create CMakeLists.txt with proper compilation options and library linking (copy and modify `samples/add_custom/CMakeLists.txt`)
- Copy `samples/add_custom/cmake` to this operator for cmake script dependecies.

#### 5. Test Infrastructure
- Create test data generation scripts (Python)
- Create result verification scripts
- Generate input data and golden data for validation (according files in `samples/add_custom/scripts`)
- For main.cpp load the test data, according the `samples/add_custom/data_utils.h`, could copy and modify it.
- Reference `samples/add_custom/scripts/` for test patterns

#### 6. Build and Test
- Create run.sh script for building and testing (copy and modify `samples/add_custom/run.sh`)
- Execute build and test by `run.sh` of operator in Docker environment (According `./env_setup.sh`)
- Test in CPU mode with SOC_VERSION=Ascend910B, `run.sh -r cpu -v Ascend910B`
- If build or test failed, should fix

You should complete the full operator development until the operator's build and testing are successfully debugged. If the operator development fails in the end, you also need to record the failure process and reasons as the basis for complexity evaluation.

## Agent Framework

This project uses Claude Code's SubAgent functionality with two specialized agents:

### Operator Development Agent (.claude/agents/op-developer.md)

Responsible for complete operator development lifecycle:
- Develops custom operators from prototype definition to testing
- Creates all necessary files (kernel functions, host applications, build scripts)
- Follows project directory structure and development workflow
- Records all development activities in `logs/` directory for evaluation

**Usage**: Launch this agent when you need to develop a new operator from scratch.

### Complexity Evaluation Agent (.claude/agents/dev-evaluator.md)

Evaluates operator development complexity based on development records:
- Analyzes development logs from `logs/` directory
- Provides complexity scores and detailed analysis reports
- Identifies optimization opportunities for development process
- Supports single operator evaluation and multi-operator comparison

**Usage**: Launch this agent to evaluate the complexity of operator development processes.
