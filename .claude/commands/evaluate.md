---
description: Evaluate the complexity of operator development processes using the [evaluation agent](../agents/dev-evaluator.md)
---

## Usage

```bash
/evaluate [operator_name] [optional: comparison_operator_name...]
```

or

```bash
/evaluate [user specified log file]
```

## Description

This command launches the complexity evaluation agent to analyze the development process of Huawei CANN Ascend C operators. The agent will:

- Analyze development logs from the `logs/` directory or user specified operator developing log file
- Evaluate complexity based on comprehensive development records
- Provide detailed complexity analysis and optimization recommendations
- Support single operator evaluation and multi-operator comparison
- Using Chinese to output evaluation result

## Examples

```bash
# Evaluate a user specified operator developing log file
/evaluate ./logs/[log_file_name].md

# Evaluate a single operator
/evaluate add_custom

# Compare multiple operators
/evaluate add_custom matmul_custom conv_custom

# Evaluate with specific focus
/evaluate add_custom --focus file_access
```

## Parameters

- **operator_developing_log_file**: Log file to evaluate
- **operator_name**: Name of the operator to evaluate (must exist in `logs/` directory)
- **comparison_operator_name**: Optional additional operators for comparative analysis

## Output

### Single Operator Evaluation
- **Complexity Score**: Numerical rating (1-10 scale)
- **Detailed Analysis Report**: Breakdown by evaluation criteria
- **Key Complexity Factors**: Primary drivers of development complexity
- **Optimization Recommendations**: Specific suggestions for improvement

### Multi-Operator Comparison
- **Complexity Ranking**: Relative complexity scores across operators
- **Comparative Analysis**: Differences in development approaches
- **Common Complexity Patterns**: Recurring challenges across operators
- **Best Practices Identification**: Successful development patterns

## Evaluation Criteria

### 1. Context Consumption Analysis
- Total context tokens consumed during development
- Context usage patterns and efficiency
- Peak context usage and distribution

### 2. File Access Complexity
- Number of files accessed during development
- Types of files accessed (source code, documentation, configuration)
- File access patterns and frequency
- Documentation lookup efficiency

### 3. Network Resource Usage
- Number of network URLs resources accessed
- Types of network resources (documentation, APIs, tools)
- Network access patterns and dependencies
- External resource reliance

### 4. Tool Call Analysis
- Total number of tool calls made
- Types of tools used (file operations, build tools, testing tools)
- Tool call efficiency and success rates
- Tool dependency complexity

### 5. Failure and Retry Analysis
- Number of failure retries during development
- Reasons for failures (compilation errors, test failures, logic errors)
- Retry patterns and learning efficiency
- Error resolution effectiveness

## Analysis Dimensions

### Technical Complexity
- Ascend C API complexity and learning curve
- Memory management challenges
- Build system configuration complexity
- Testing infrastructure setup difficulty

### Process Complexity
- Development workflow efficiency
- Documentation accessibility and quality
- Tool integration and usability
- Error diagnosis and resolution complexity

### Resource Complexity
- Document complexity
- External dependency management
- Environment setup complexity
- Build and test execution complexity

### Summary by diagram or table
- Should the count of tools used, file accessed, web url fetched, failures, retries, token consumption, code lines generated...
- Classified the statistics, and using diagram or table to summary and show in human readable format

## Requirements

- Development logs must exist, user specified or in `logs/[operator_name]/` directory
- Logs should contain comprehensive development records
- Operator must have been developed using the operator development agent
- Evaluation output must in Chinese