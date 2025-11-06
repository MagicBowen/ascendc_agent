#!/usr/bin/python3
# coding=utf-8
#
# Copyright (C) 2023-2024. Huawei Technologies Co., Ltd. All rights reserved.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
# ===============================================================================

import numpy as np
import sys
import os


def gen_golden_data_simple(data_type='float16'):
    if data_type == 'float32':
        dtype = np.float32
    else:
        dtype = np.float16

    # Generate input data with reasonable ranges to avoid overflow
    input_x = np.random.uniform(1, 100, [8, 2048]).astype(dtype)
    input_y = np.random.uniform(1, 100, [8, 2048]).astype(dtype)

    # Perform element-wise subtraction: z = x - y
    golden = (input_x - input_y).astype(dtype)

    # Create directories if they don't exist
    os.makedirs("./input", exist_ok=True)
    os.makedirs("./output", exist_ok=True)

    input_x.tofile("./input/input_x.bin")
    input_y.tofile("./input/input_y.bin")
    golden.tofile("./output/golden.bin")

    print(f"Generated test data for {data_type}")
    print(f"Input x shape: {input_x.shape}, dtype: {input_x.dtype}")
    print(f"Input y shape: {input_y.shape}, dtype: {input_y.dtype}")
    print(f"Golden output shape: {golden.shape}, dtype: {golden.dtype}")


if __name__ == "__main__":
    data_type = 'float16'
    if len(sys.argv) > 1:
        data_type = sys.argv[1]

    if data_type not in ['float16', 'float32']:
        print("Invalid data type. Using default float16")
        data_type = 'float16'

    gen_golden_data_simple(data_type)