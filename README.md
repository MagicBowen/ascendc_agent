# README

本仓作为华为 CANN Ascend C 算子开发的工程模板, 用于 AI Agent 在本工程目录下参考样例代码和文档说明，完成自定义算子的开发、构建和测试。

AI Agent 在执行算子开发的过程中，会把过程上下文都记录下来，然后会使用另一个评价 Agent 来对算子开发过程的复杂度进行评估。

这里 Agent 框架采用 Claude Code, 模型 API 使用 DeepSeek V3.1。算子开发 Agent 和 评估 Agent 采用 Claude Code 的 SubAgent 功能实现。

## Operator Development Agent

算子开发的 Agent 按照如下要求独立完成算子开发、构建和测试任务。同时把一个算子的完整开发过程都记录下来，存放在 logs 目录下，每一个算子独立一个子目录。要求把算子开发过程中的所有关键过程，工具调用，文件访问，网络访问，失败重试，上下文的总消耗等细节都记录下来，作为后续评估 Agent 进行复杂度评估的依据。

算子的开发方式参考如下介绍和相关文档，算子开发的 Agent 需要严格遵守下面的要求进行算子开发，直至调通算子的构建和测试为止。如果最后算子开发失败，Agent 也需要把失败的过程和原因都记录下来，作为复杂度评估的依据。

### 算子目录结构

- `samples/`：示例算子的代码，包括源码、编译脚本和测试脚本。
- `docs/`：使用 Ascend C 进行算子开发的相关文档和开发说明。
- `ops/` ：用户（或者 Agent）开发的算子代码及其工程和测试脚本都放在本目录下，一个算子一个子目录分开存放。
- `env_setup.sh`：工具链安装在 docker 中，使用该脚本可以映射本目录到 Docker 容器中，进行算子的代码编译和测试。
- `README.md`：本说明文档，介绍仓库的目录结构和使用方法。

### 算子开发过程

#### 算子原型文件

用户提供算子原型文件（.json），定义算子的输入输出属性和算子元信息，作为算子开发的参考（参见 `samples/add_custom/AddCustom.json`）。

#### 代码编写

- 核函数开发：根据算子的数学表达式和计算逻辑，使用 Ascend C 提供的算子开发接口编写核函数代码（参见 `samples/add_custom/add_custom.cpp`）。 具体核函数开发过程中需要参考的开发文档，详见 `docs/` 目录下的相关文档。

- 核函数调用：在应用程序中调用核函数（参见 `samples/add_custom/main.cpp`），用户可以使用该应用程序进行算子的测试（注意：需要借助测试脚本为 main 函数生成核函数的输入数据）。 核函数调用代码的开发需要参考的开发文档，详见 `docs/` 目录下的相关文档。

#### 构建脚本

- `CMakeLists.txt`：编写 CMake 构建脚本，定义编译选项和链接库，确保算子代码能够正确编译和链接（参见 `samples/add_custom/CMakeLists.txt`）。

#### 测试开发

根据算子的功能和性能需求，编写相应的测试用例（参见 `samples/add_custom/scripts`）。

其中包括：
- 数据生成脚本：编写数据生成脚本，生成输入数据和真值数据（参见 `samples/add_custom/scripts/gen_data.py`）。
- 结果验证脚本：编写结果验证脚本，验证算子输出数据和真值数据是否一致（参见 `samples/add_custom/scripts/verify_result.py`）。

### 构建和测试

#### 环境准备

- CANN 的开发工具链安装在容器环境中，执行如下脚本进入容器环境

```bash
./env_setup.sh
```

- 配置环境变量: 进入容器环境后，需要配置环境变量

```bash
source /usr/local/Ascend/ascend-toolkit/set_env.sh
export ASCEND_INSTALL_PATH=/usr/local/Ascend/ascend-toolkit/latest
```

#### 构建和测试

第一次使用前，需要赋予 `run.sh` 运行权限：

```bash
chmod a+x run.sh
```

执行构建和测试：

```bash
bash run.sh -r [RUN_MODE] -v  [SOC_VERSION]
```

- RUN_MODE：编译方式，可选择CPU调试，NPU上板。支持参数为[cpu / npu], 容器环境中选择 cpu 模式
- SOC_VERSION：昇腾AI处理器型号，容器环境中选择 Ascend910B

例如在容器环境中如下执行：

```bash
bash run.sh -r cpu -v Ascend910B
```

run.sh 会进行构建，调用脚本进行测试数据生成，运行算子 CPU 侧的可执行程序，完成算子的调用和测试。

## Complexity Evaluation Agent

算子复杂度评估 Agent 负责对算子开发过程的复杂度进行评估。评估的依据是算子开发 Agent 在开发过程中的所有记录，包括上下文信息、工具调用、文件访问、网络访问、失败重试等细节。

用户输入 logs 目录下的指定算子，评估 Agent 会分析对应目录下该算子的开发过程记录，给出复杂度评估结果。评估结果包括复杂度评分和详细的复杂度分析报告。分析报告细节包括：

- 总的上下文消耗
- 访问的文件数目
- 访问的网络资源数目
- 调用工具的次数
- 失败重试的原因和次数
- 其他影响复杂度的因素

分析总结影响对应算子开发的复杂度的关键因素，给出优化建议，帮助用于改进算子开发的文档设计、API 接口设计、工具易用性设计等方面，降低算子开发的复杂度，提高开发效率。

此外，用户可以选择对比多个算子的开发复杂度，基于上面的维度对多个算子的开发过程记录进行分析并对比，给出各个算子的复杂度评分、排名和关键影响点，帮助用户了解不同算子开发的复杂度差异。
