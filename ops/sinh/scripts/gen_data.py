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


def gen_golden_data_sinh():
    # Generate input data with various values to test sinh function
    # Include positive, negative, and zero values
    input_x = np.concatenate([
        np.random.uniform(-5, 5, [4, 1024]),  # Mixed positive and negative
        np.random.uniform(0, 3, [2, 1024]),   # Positive values
        np.random.uniform(-3, 0, [2, 1024])   # Negative values
    ]).astype(np.float32)

    # Calculate golden data using numpy sinh function
    golden = np.sinh(input_x).astype(np.float32)

    # Create input and output directories
    import os
    os.makedirs("./input", exist_ok=True)
    os.makedirs("./output", exist_ok=True)

    input_x.tofile("./input/input_x.bin")
    golden.tofile("./output/golden.bin")

    print(f"Generated input data shape: {input_x.shape}")
    print(f"Input data range: [{input_x.min():.3f}, {input_x.max():.3f}]")
    print(f"Golden data range: [{golden.min():.3f}, {golden.max():.3f}]")


if __name__ == "__main__":
    gen_golden_data_sinh()