#!/usr/bin/python3
# coding=utf-8
#
# Copyright (C) 2023-2024. Huawei Technologies Co., Ltd. All rights reserved.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
# ===============================================================================

import sys
import numpy as np

# for float32
relative_tol = 1e-4
absolute_tol = 1e-6
error_tol = 1e-3


def verify_result(output, golden):
    output = np.fromfile(output, dtype=np.float32).reshape(-1)
    golden = np.fromfile(golden, dtype=np.float32).reshape(-1)

    print(f"Output data shape: {output.shape}")
    print(f"Golden data shape: {golden.shape}")
    print(f"Output range: [{output.min():.6f}, {output.max():.6f}]")
    print(f"Golden range: [{golden.min():.6f}, {golden.max():.6f}]")

    different_element_results = np.isclose(output,
                                           golden,
                                           rtol=relative_tol,
                                           atol=absolute_tol,
                                           equal_nan=True)
    different_element_indexes = np.where(different_element_results == False)[0]

    print(f"Total elements: {golden.size}")
    print(f"Different elements: {different_element_indexes.size}")

    for index in range(min(len(different_element_indexes), 10)):
        real_index = different_element_indexes[index]
        golden_data = golden[real_index]
        output_data = output[real_index]
        print(
            "data index: %06d, expected: %-.9f, actual: %-.9f, rdiff: %-.6f" %
            (real_index, golden_data, output_data,
             abs(output_data - golden_data) / max(abs(golden_data), 1e-9)))

    error_ratio = float(different_element_indexes.size) / golden.size
    print("error ratio: %.4f, tolerance: %.4f" % (error_ratio, error_tol))

    if different_element_indexes.size > 0:
        print(f"First 10 mismatches shown above out of {different_element_indexes.size} total mismatches")

    return error_ratio <= error_tol


if __name__ == '__main__':
    try:
        if len(sys.argv) != 3:
            print("Usage: python3 verify_result.py <output_file> <golden_file>")
            sys.exit(1)

        res = verify_result(sys.argv[1], sys.argv[2])
        if not res:
            raise ValueError("[ERROR] result error")
        else:
            print("test pass")
    except Exception as e:
        print(e)
        sys.exit(1)