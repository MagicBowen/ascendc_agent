/**
 * @file main.cpp
 *
 * Copyright (C) 2024. Huawei Technologies Co., Ltd. All rights reserved.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 */
#include "data_utils.h"
#ifndef ASCENDC_CPU_DEBUG
#include "acl/acl.h"
#else
#include "tikicpulib.h"
extern "C" __global__ __aicore__ void sinh_custom(GM_ADDR x, GM_ADDR y);
#endif

int32_t main(int32_t argc, char *argv[])
{
    uint32_t blockDim = 8;
    size_t inputByteSize = 8 * 2048 * sizeof(float);
    size_t outputByteSize = 8 * 2048 * sizeof(float);

#ifdef ASCENDC_CPU_DEBUG
    uint8_t *x = (uint8_t *)AscendC::GmAlloc(inputByteSize);
    uint8_t *y = (uint8_t *)AscendC::GmAlloc(outputByteSize);

    ReadFile("./input/input_x.bin", inputByteSize, x, inputByteSize);

    AscendC::SetKernelMode(KernelMode::AIV_MODE);
    ICPU_RUN_KF(sinh_custom, blockDim, x, y); // use this macro for cpu debug

    WriteFile("./output/output_y.bin", y, outputByteSize);

    AscendC::GmFree((void *)x);
    AscendC::GmFree((void *)y);
#else
    CHECK_ACL(aclInit(nullptr));
    int32_t deviceId = 0;
    CHECK_ACL(aclrtSetDevice(deviceId));
    aclrtStream stream = nullptr;
    CHECK_ACL(aclrtCreateStream(&stream));

    uint8_t *xHost, *yHost;
    uint8_t *xDevice, *yDevice;

    CHECK_ACL(aclrtMallocHost((void **)(&xHost), inputByteSize));
    CHECK_ACL(aclrtMallocHost((void **)(&yHost), outputByteSize));
    CHECK_ACL(aclrtMalloc((void **)&xDevice, inputByteSize, ACL_MEM_MALLOC_HUGE_FIRST));
    CHECK_ACL(aclrtMalloc((void **)&yDevice, outputByteSize, ACL_MEM_MALLOC_HUGE_FIRST));

    ReadFile("./input/input_x.bin", inputByteSize, xHost, inputByteSize);

    CHECK_ACL(aclrtMemcpy(xDevice, inputByteSize, xHost, inputByteSize, ACL_MEMCPY_HOST_TO_DEVICE));

    aclrtBinHandle binHandle = nullptr;
    aclrtFuncHandle funcHandle = nullptr;
    aclrtArgsHandle argsHandle = nullptr;
    aclrtParamHandle paramHandle = nullptr;

    const char *filePath = "./out/fatbin/ascendc_kernels/ascendc_kernels.o";
    CHECK_ACL(aclrtBinaryLoadFromFile(filePath, nullptr, &binHandle));
    CHECK_ACL(aclrtBinaryGetFunction(binHandle, "sinh_custom", &funcHandle));
    CHECK_ACL(aclrtKernelArgsInit(funcHandle, &argsHandle));

    CHECK_ACL(aclrtKernelArgsAppend(argsHandle, (void **)&xDevice, sizeof(uintptr_t), &paramHandle));
    CHECK_ACL(aclrtKernelArgsAppend(argsHandle, (void **)&yDevice, sizeof(uintptr_t), &paramHandle));
    CHECK_ACL(aclrtKernelArgsFinalize(argsHandle));

    CHECK_ACL(aclrtLaunchKernelWithConfig(funcHandle, blockDim, stream, nullptr, argsHandle, nullptr));
    CHECK_ACL(aclrtSynchronizeStream(stream));

    CHECK_ACL(aclrtMemcpy(yHost, outputByteSize, yDevice, outputByteSize, ACL_MEMCPY_DEVICE_TO_HOST));
    WriteFile("./output/output_y.bin", yHost, outputByteSize);

    CHECK_ACL(aclrtBinaryUnLoad(binHandle));
    CHECK_ACL(aclrtFree(xDevice));
    CHECK_ACL(aclrtFree(yDevice));
    CHECK_ACL(aclrtFreeHost(xHost));
    CHECK_ACL(aclrtFreeHost(yHost));

    CHECK_ACL(aclrtDestroyStream(stream));
    CHECK_ACL(aclrtResetDevice(deviceId));
    CHECK_ACL(aclFinalize());
#endif
    return 0;
}