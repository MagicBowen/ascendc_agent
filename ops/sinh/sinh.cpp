/**
 * @file sinh.cpp
 *
 * Copyright (C) 2024. Huawei Technologies Co., Ltd. All rights reserved.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 */
#include "kernel_operator.h"

constexpr int32_t TOTAL_LENGTH = 8 * 2048;                            // total length of data
constexpr int32_t USE_CORE_NUM = 8;                                   // num of core used
constexpr int32_t BLOCK_LENGTH = TOTAL_LENGTH / USE_CORE_NUM;         // length computed of each core
constexpr int32_t TILE_NUM = 8;                                       // split data into 8 tiles for each core
constexpr int32_t BUFFER_NUM = 2;                                     // tensor num for each queue
constexpr int32_t TILE_LENGTH = BLOCK_LENGTH / TILE_NUM / BUFFER_NUM; // separate to 2 parts, due to double buffer

class KernelSinh {
public:
    __aicore__ inline KernelSinh() {}
    __aicore__ inline void Init(GM_ADDR x, GM_ADDR y)
    {
        xGm.SetGlobalBuffer((__gm__ float *)x + BLOCK_LENGTH * AscendC::GetBlockIdx(), BLOCK_LENGTH);
        yGm.SetGlobalBuffer((__gm__ float *)y + BLOCK_LENGTH * AscendC::GetBlockIdx(), BLOCK_LENGTH);
        pipe.InitBuffer(inQueueX, BUFFER_NUM, TILE_LENGTH * sizeof(float));
        pipe.InitBuffer(outQueueY, BUFFER_NUM, TILE_LENGTH * sizeof(float));
        pipe.InitBuffer(tempBuf1, TILE_LENGTH * sizeof(float));
        pipe.InitBuffer(tempBuf2, TILE_LENGTH * sizeof(float));
        pipe.InitBuffer(tempBuf3, TILE_LENGTH * sizeof(float));
    }
    __aicore__ inline void Process()
    {
        int32_t loopCount = TILE_NUM * BUFFER_NUM;
        for (int32_t i = 0; i < loopCount; i++) {
            CopyIn(i);
            Compute(i);
            CopyOut(i);
        }
    }

private:
    __aicore__ inline void CopyIn(int32_t progress)
    {
        AscendC::LocalTensor<float> xLocal = inQueueX.AllocTensor<float>();
        AscendC::DataCopy(xLocal, xGm[progress * TILE_LENGTH], TILE_LENGTH);
        inQueueX.EnQue(xLocal);
    }
    __aicore__ inline void Compute(int32_t progress)
    {
        AscendC::LocalTensor<float> xLocal = inQueueX.DeQue<float>();
        AscendC::LocalTensor<float> yLocal = outQueueY.AllocTensor<float>();
        AscendC::LocalTensor<float> expX = tempBuf1.Get<float>();
        AscendC::LocalTensor<float> expNegX = tempBuf2.Get<float>();
        AscendC::LocalTensor<float> negX = tempBuf3.Get<float>();

        // Calculate e^x
        AscendC::Exp(expX, xLocal, TILE_LENGTH);

        // Calculate -x for e^(-x)
        AscendC::Muls(negX, xLocal, -1.0f, TILE_LENGTH);

        // Calculate e^(-x)
        AscendC::Exp(expNegX, negX, TILE_LENGTH);

        // Calculate (e^x - e^(-x))
        AscendC::Sub(yLocal, expX, expNegX, TILE_LENGTH);

        // Scale by 0.5 to get (e^x - e^(-x)) / 2
        AscendC::Muls(yLocal, yLocal, 0.5f, TILE_LENGTH);

        outQueueY.EnQue<float>(yLocal);
        inQueueX.FreeTensor(xLocal);
    }
    __aicore__ inline void CopyOut(int32_t progress)
    {
        AscendC::LocalTensor<float> yLocal = outQueueY.DeQue<float>();
        AscendC::DataCopy(yGm[progress * TILE_LENGTH], yLocal, TILE_LENGTH);
        outQueueY.FreeTensor(yLocal);
    }

private:
    AscendC::TPipe pipe;
    AscendC::TQue<AscendC::QuePosition::VECIN, BUFFER_NUM> inQueueX;
    AscendC::TQue<AscendC::QuePosition::VECOUT, BUFFER_NUM> outQueueY;
    AscendC::TBuf<AscendC::TPosition::VECCALC> tempBuf1, tempBuf2, tempBuf3;
    AscendC::GlobalTensor<float> xGm;
    AscendC::GlobalTensor<float> yGm;
};

extern "C" __global__ __aicore__ void sinh_custom(GM_ADDR x, GM_ADDR y)
{
    KernelSinh op;
    op.Init(x, y);
    op.Process();
}