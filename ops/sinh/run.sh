#!/bin/bash

# Set environment variables
source /usr/local/Ascend/ascend-toolkit/set_env.sh
export ASCEND_INSTALL_PATH=/usr/local/Ascend/ascend-toolkit/latest

# Default values
RUN_MODE="cpu"
SOC_VERSION="Ascend910B"

# Parse command line arguments
while getopts "r:v:" opt; do
    case $opt in
        r)
            RUN_MODE=$OPTARG
            ;;
        v)
            SOC_VERSION=$OPTARG
            ;;
        *)
            echo "Usage: $0 [-r cpu|sim|npu] [-v SOC_VERSION]"
            exit 1
            ;;
    esac
done

echo "Building sinh operator with RUN_MODE=$RUN_MODE, SOC_VERSION=$SOC_VERSION"

# Create build directory
mkdir -p build
cd build

# Configure with CMake
cmake .. -DRUN_MODE=$RUN_MODE -DSOC_VERSION=$SOC_VERSION

# Build the project
make -j$(nproc)

# Create input/output directories
mkdir -p ../input
mkdir -p ../output

# Generate test data
echo "Generating test data..."
cd ..
python3 scripts/gen_data.py

# Run the application
echo "Running sinh operator..."
cd build
./ascendc_kernels

# Verify results
echo "Verifying results..."
cd ..
python3 scripts/verify_result.py output/output_y.bin output/golden.bin

if [ $? -eq 0 ]; then
    echo "SUCCESS: sinh operator test passed!"
else
    echo "FAILED: sinh operator test failed!"
    exit 1
fi