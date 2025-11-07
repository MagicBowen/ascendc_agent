# Issue Records

This document systematically records various types of issues identified during the prototype verification of the Ascend C operator development Agent, including technical challenges, engineering limitations, and optimization opportunities.

## 1. Documentation and Knowledge Base Challenges

### 1.1 Documentation Accessibility
- **Problem Description**: Huawei's official online documentation cannot be directly accessed by the Agent through WebFetch/WebSearch due to anti-crawling mechanisms or permission requirements
- **Impact**: Agent cannot obtain the latest API documentation and development guides in real-time
- **Potential Solutions**:
  - Establish localized documentation mirror
  - Develop dedicated documentation MCP service
  - Collaborate with Huawei to obtain API access permissions

### 1.2 Documentation Fragmentation
- **Problem Description**: Ascend C basic API documentation is scattered across multiple pages and chapters, lacking a unified API reference manual
- **Impact**: Agent needs to browse large amounts of content to find required information, increasing development complexity
- **Improvement Suggestions**:
  - Create unified API index documentation
  - Develop intelligent document retrieval system
  - Establish API classification and tagging system

### 1.3 Documentation Organization Chaos
- **Problem Description**: Reference manual content is fragmented, lacks coherence, and lacks a complete guide from beginner to advanced
- **Impact**: Steep learning curve, low development efficiency
- **Optimization Direction**:
  - Restructure documentation, establish clear hierarchical relationships
  - Provide progressive learning paths
  - Add practical cases and best practices

### 1.4 Localization Requirements
- **Problem Description**: Need for localized markdown format API manuals and tool guides
- **Importance**: Improve access speed, support offline development, facilitate version control
- **Implementation Plan**:
  - Regularly synchronize official documentation updates
  - Establish documentation version management mechanism
  - Develop automatic document conversion tools

### 1.5 Image Processing Limitations
- **Problem Description**: Online documentation contains many images that are difficult for models to process, lacking DSL diagrams like mermaid
- **Impact**: Agent cannot effectively parse architecture diagrams and flowcharts
- **Solutions**:
  - Develop image-to-text description tools
  - Promote use of standardized chart languages
  - Establish diagram library and documentation

## 2. Development Environment and Toolchain Issues

### 2.1 Documentation Inconsistency
- **Problem Description**: Example code in documentation conflicts with previously mentioned environment versions and build methods
- **Specific Manifestations**: Version number mismatches, outdated build commands, unclear dependency relationships
- **Improvement Measures**:
  - Establish documentation version control mechanism
  - Provide version compatibility matrix
  - Regularly verify example code availability

### 2.2 Initial Example Problems
- **Problem Description**: First "hello world" example cannot compile; documentation contains errors and lacks version alignment
- **Impact**: Difficult for beginners to get started, affects development experience
- **Fix Solutions**:
  - Verify all basic example code
  - Provide detailed error troubleshooting guides
  - Establish example code testing pipeline

### 2.3 Complex Engineering Patterns
- **Problem Description**: Multiple engineering patterns (direct calls, custom projects, ACLNN) and build methods (BiSheng plugin, old/new CMake projects) cause confusion
- **Challenges**: Difficulty in selection, complex configuration, high maintenance costs
- **Simplification Strategies**:
  - Provide pattern selection guides
  - Develop unified build templates
  - Establish best practice recommendations

### 2.4 Example Repository Overload
- **Problem Description**: Example repository contains too much scattered content, mixing multiple engineering patterns, making it difficult for Agent to focus
- **Optimization Solutions**:
  - Reorganize examples by functional modules
  - Provide clear navigation and search
  - Establish example classification system

### 2.5 Container Environment Setup
- **Problem Description**: Container environment requires manual environment variable setup, should be pre-configured
- **Improvement Direction**:
  - Pre-configure development environment images
  - Provide one-click startup scripts
  - Establish environment verification mechanism

### 2.6 MCP Integration
- **Problem Description**: Documentation should be accessible through mainstream online documentation MCPs (like context7)
- **Integration Plan**:
  - Evaluate mainstream MCP documentation services
  - Develop dedicated Ascend C MCP
  - Establish documentation synchronization mechanism

## 3. Agent Engineering Capability Limitations

### 3.1 Limited Project Types
- **Problem Description**: Currently only supports simple project types (independent kernel function implementation files with main functions calling ACL interfaces)
- **Limitations**: Cannot handle complex operators, multi-core collaboration, distributed computing scenarios
- **Extension Plan**:
  - Support multi-core operator development
  - Add distributed computing patterns
  - Develop complex operator templates

### 3.2 Simulation Environment Limitations
- **Problem Description**: Build and test environment uses Docker version, no real NPU execution verification
- **Impact**: Cannot verify real hardware performance, environmental difference risks exist
- **Solutions**:
  - Integrate cloud NPU test environment
  - Develop performance simulation tools
  - Establish hardware compatibility testing

### 3.3 Insufficient Operator Test Coverage
- **Problem Description**: Agent-generated operator test cases focus on simple vector operators, lacking tests for boundary cases, chunking strategies, dynamic shapes, cube operators, fusion operators, and hardware variants
- **Test Capability Enhancement**:
  - Develop intelligent test case generation
  - Add boundary condition testing
  - Support multi-hardware platform validation

### 3.4 Incomplete Context Dumping
- **Problem Description**: Operator development Agent's context dumping during development process is incomplete (solutions identified but need implementation verification)
- **Improvement Measures**:
  - Improve context capture mechanism
  - Establish development process tracking
  - Develop debugging information collection

### 3.5 Basic Metric Analysis
- **Problem Description**: Metric Agent only performs basic analysis, lacking specific measurement models or comparison capabilities
- **Analysis Capability Enhancement**:
  - Establish multi-dimensional evaluation indicator system
  - Develop performance benchmark testing
  - Implement cross-operator comparison analysis

## 4. Framework and Cost Optimization

### 4.1 Framework Limitations
- **Problem Description**: Current verification mainly uses Claude Code + Deepseek V3.2, need to evaluate alternative Agent frameworks and model APIs
- **Evaluation Direction**:
  - Test other open-source Agent frameworks
  - Evaluate different large language model performances
  - Establish framework selection criteria

### 4.2 Token Consumption Optimization
- **Problem Description**: Significant AI Agent token consumption - 1600+ API calls consumed 51,923,051 tokens in a few days, cost approximately ¥16+
- **Optimization Strategies**:
  - Develop token optimization algorithms
  - Establish caching mechanism
  - Optimize prompt design
  - Implement batch processing

## 5. Priority and Implementation Roadmap

### High Priority (1-2 weeks)
- [ ] Fix basic example code compilation issues
- [ ] Improve container environment pre-configuration
- [ ] Establish localized documentation mirror
- [ ] Optimize token consumption strategy

### Medium Priority (1-2 months)
- [ ] Develop intelligent test case generation
- [ ] Establish multi-dimensional evaluation indicator system
- [ ] Extend project type support
- [ ] Improve context dumping mechanism

### Long-term Planning (3-6 months)
- [ ] Integrate real NPU test environment
- [ ] Develop dedicated documentation MCP service
- [ ] Establish performance benchmark testing system
- [ ] Support complex operator development templates

## 6. Monitoring and Improvement Mechanisms

### 6.1 Issue Tracking
- Establish issue classification and priority system
- Regularly review and update issue status
- Track solution implementation effectiveness

### 6.2 Performance Monitoring
- Monitor Agent development efficiency indicators
- Track token consumption and costs
- Evaluate development quality improvements

### 6.3 Continuous Improvement
- Regularly collect user feedback
- Analyze development bottlenecks
- Optimize workflow processes