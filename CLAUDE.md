# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Project Overview

This is a Huawei CANN Ascend C operator development template for developing custom operators using Huawei's Ascend AI processors. The project provides a complete workflow for developing, building, and testing custom operators using Ascend C programming language.

## Architecture

### Directory Structure

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

- `logs/` - Operator Developer agent should record the developing process in this folder, each in its own subdirectory, used by evaluation agent to analyse.

- `tests/` - Users use this to record some test input/output information; the Agent doesn't need to care.

- `env_setup.sh` - This shell script help operator developer agent enter the docker env first before build and test.

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

### Development Workflow with Agents

1. **Operator Development**: Use the Operator Development Agent to create new operators
2. **Process Recording**: All development activities are automatically logged in `logs/`
3. **Complexity Evaluation**: Use the Complexity Evaluation Agent to analyze development efficiency
4. **Process Improvement**: Apply optimization recommendations to improve future development

### Logging Requirements

- Each operator development process creates detailed logs in `logs/[operator_name]/`
- Logs include: tool calls, file accesses, network accesses, failure retries, context consumption
- Logs serve as the basis for complexity evaluation and process optimization