#!/usr/bin/env bash

# project config
project_path=$(cd "$(dirname "$0")"; pwd)
project_name="${project_path##*/}"
build="build"
doc_doxygen="./docs/doxygen"
docker_image="cann_ascendc_agent:v1"

# After enter the docker container, you may need to set the Ascend environment variables:
# source /usr/local/Ascend/ascend-toolkit/set_env.sh
# export ASCEND_INSTALL_PATH=/usr/local/Ascend/ascend-toolkit/latest

detect_os() {
    case "$(uname)" in
        "Darwin")
            docker_cmd=docker
            docker_src_path="$project_path"
            docker_work_dir="/$project_name"
            ;;
        "MINGW"*)
            docker_cmd="winpty docker"
            docker_src_path="/$project_path"
            docker_work_dir="//$project_name"
            ;;
        "Linux")
            docker_cmd=docker
            docker_src_path="$project_path"
            docker_work_dir="/$project_name"
            ;;
        *)
            error "Unsupported operating system: $(uname)"
            exit 1
            ;;
    esac
}

detect_os

# output utils
command_exists() {
	command -v "$@" >/dev/null 2>&1
}

info() {
	echo ${BOLD}"$@"${RESET} >&2
}

error() {
	echo ${RED}"ERROR: $@"${RESET} >&2
}

success() {
	echo ${GREEN}"SUCCESS: $@"${RESET} >&2
}

underline() {
	echo "$(printf '\033[4m')$@$(printf '\033[24m')"
}

divider() {
    info "==============================================="
}

start_exec() {
    info "START: $@ ${project_name}"
    divider
}

success_exec() {
    divider
    success "$@ ${project_name}!"
}

failed_exec() {
    divider
    error "$@ ${project_name}!"
}

run_command() {
    local description="$1"
    shift
    start_exec "$description"
    if "$@"; then
        success_exec "$description"
    else
        failed_exec "$description"
        exit 1
    fi
}

setup_color() {
	# Only use colors if connected to a terminal
	if [ -t 1 ]; then
		RED=$(printf '\033[31m')
		GREEN=$(printf '\033[32m')
		YELLOW=$(printf '\033[33m')
		BLUE=$(printf '\033[34m')
		BOLD=$(printf '\033[1m')
		RESET=$(printf '\033[m')
	else
		RED=""
		GREEN=""
		YELLOW=""
		BLUE=""
		BOLD=""
		RESET=""
	fi
}

function setup_env() {
    validate_dependencies docker

    # Check if we're in an interactive terminal
    if [ -t 0 ] && [ -t 1 ]; then
        # Interactive mode - use -it flags
        run_command "setup docker environment of" $docker_cmd run -it -v $docker_src_path:/$project_name -w $docker_work_dir $docker_image /bin/bash
    else
        # Non-interactive mode - run basic shell with help message
        start_exec "setup docker environment of"
        echo "Running in non-interactive mode. Use './ccup.sh -u -b -t' to build and test the project."
        $docker_cmd run -v $docker_src_path:/$project_name -w $docker_work_dir $docker_image /bin/bash -c "echo 'Docker environment ready. Use ./ccup.sh commands to build and test.' && ./ccup.sh -h"
        if [ $? -ne 0 ]; then
            failed_exec "setup docker environment"
            exit 1
        fi
        success_exec "setup docker environment"
    fi
}

setup_color
setup_env
