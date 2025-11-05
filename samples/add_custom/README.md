## 目录结构介绍
```
├── AddKernelInvocationAcl
│   ├── cmake                   // 编译工程文件
│   ├── scripts
│   │   ├── gen_data.py         // 输入数据和真值数据生成脚本
│   │   └── verify_result.py    // 验证输出数据和真值数据是否一致的验证脚本
│   ├── add_custom.cpp          // 算子kernel实现
│   ├── CMakeLists.txt          // 编译工程文件
│   ├── data_utils.h            // 数据读入写出函数
│   ├── main.cpp                // 主函数，调用算子的应用程序，含CPU域及NPU域调用
│   ├── AddCustom.json          // 算子描述文件, 定义算子输入输出属性, 用于程序员阅读参考
│   ├── README.md               // 本示例工程的说明文档
│   └── run.sh                  // 编译运行算子的脚本
```
## 代码实现介绍
本调用样例中实现的是固定shape为8*2048的Add算子。
- kernel实现  
  Add算子的数学表达式为：
  ```
  z = x + y
  ```
  计算逻辑是：Ascend C提供的矢量计算接口的操作元素都为LocalTensor，输入数据需要先搬运进片上存储，然后使用计算接口完成两个输入参数相加，得到最终结果，再搬出到外部存储上。

  Add算子的实现流程分为3个基本任务：CopyIn，Compute，CopyOut。CopyIn任务负责将Global Memory上的输入Tensor xGm和yGm搬运到Local Memory，分别存储在xLocal、yLocal，Compute任务负责对xLocal、yLocal执行加法操作，计算结果存储在zLocal中，CopyOut任务负责将输出数据从zLocal搬运至Global Memory上的输出Tensor zGm中。具体请参考[add_custom.cpp](./add_custom.cpp)。

- 调用实现
  1. CPU侧运行验证主要通过ICPU_RUN_KF CPU调测宏等CPU调测库提供的接口来完成；
  2. NPU侧运行验证主要通过使用aclrtLaunchKernelWithConfig函数调用来完成。

  应用程序通过 ASCENDC_CPU_DEBUG 宏区分代码逻辑运行于CPU侧还是NPU侧。

  容器环境下实际执行的是 ASCENDC_CPU_DEBUG 宏定义的CPU侧代码逻辑。

## 运行样例算子
  - 进入容器环境：
    ```bash
    ./env_setup.sh
    ```

  - 配置环境变量
    ```bash
    source /usr/local/Ascend/ascend-toolkit/set_env.sh
    export ASCEND_INSTALL_PATH=/usr/local/Ascend/ascend-toolkit/latest
    ```

  - 构建和测试执行

    ```bash
    chmod a+x run.sh
    bash run.sh -r [RUN_MODE] -v  [SOC_VERSION]
    ```
    - RUN_MODE：编译方式，可选择CPU调试，NPU上板。支持参数为[cpu / npu], 容器环境中选择 cpu 模式
    - SOC_VERSION：昇腾AI处理器型号，容器环境中选择 Ascend910B

    例如在容器环境中如下执行：
    ```bash
    bash run.sh -r cpu -v Ascend910B
    ```
