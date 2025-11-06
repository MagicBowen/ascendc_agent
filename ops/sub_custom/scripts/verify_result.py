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


def verify_result(output_file, golden_file, data_type='float16'):
    if data_type == 'float32':
        dtype = np.float32
        # Tolerances for float32
        relative_tol = 1e-6
        absolute_tol = 1e-8
        error_tol = 1e-6
    else:
        dtype = np.float16
        # Tolerances for float16
        relative_tol = 1e-3
        absolute_tol = 1e-5
        error_tol = 1e-3

    output = np.fromfile(output_file, dtype=dtype).reshape(-1)
    golden = np.fromfile(golden_file, dtype=dtype).reshape(-1)

    print(f"Verifying {data_type} results")
    print(f"Output shape: {output.shape}, dtype: {output.dtype}")
    print(f"Golden shape: {golden.shape}, dtype: {golden.dtype}")

    different_element_results = np.isclose(output,
                                           golden,
                                           rtol=relative_tol,
                                           atol=absolute_tol,
                                           equal_nan=True)
    different_element_indexes = np.where(different_element_results == False)[0]

    print(f"Total elements: {golden.size}")
    print(f"Different elements: {different_element_indexes.size}")

    for index in range(len(different_element_indexes)):
        real_index = different_element_indexes[index]
        golden_data = golden[real_index]
        output_data = output[real_index]
        print(
            "data index: %06d, expected: %-.9f, actual: %-.9f, rdiff: %-.6f" %
            (real_index, golden_data, output_data,
             abs(output_data - golden_data) / golden_data if golden_data != 0 else abs(output_data - golden_data)))
        if index == 100:
            print("... (showing first 100 differences)")
            break

    error_ratio = float(different_element_indexes.size) / golden.size
    print("error ratio: %.6f, tolerance: %.6f" % (error_ratio, error_tol))
    return error_ratio <= error_tol


if __name__ == '__main__':
    try:
        if len(sys.argv) < 3:
            print("Usage: python3 verify_result.py <output_file> <golden_file> [data_type]")
            sys.exit(1)

        data_type = 'float16'
        if len(sys.argv) > 3:
            data_type = sys.argv[3]

        if data_type not in ['float16', 'float32']:
            print("Invalid data type. Using default float16")
            data_type = 'float16'

        res = verify_result(sys.argv[1], sys.argv[2], data_type)
        if not res:
            raise ValueError("[ERROR] result error")
        else:
            print("test pass")
    except Exception as e:
        print(e)
        sys.exit(1)