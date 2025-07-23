#!/usr/bin/env bash

GREEN="\033[1;32m"
RED="\033[1;31m"
ILC="\033[3m"
ORG="\033[1;33m"
RST="\033[0m"

PROGRAM_NAME="vulkan_engine"

function _error()
{
    echo -e "${RED}${BOLD}[❌] ERROR:\n${RST}\t$1\n\t${ILC}\"$2\"${RST}"
    exit 84
}

function _success()
{
    echo -e "${GREEN}[✅] SUCCESS:\t${RST} ${ILC}$1${RST}"
}

function _info()
{
    echo -e "${ORG}[🚧] RUNNING:\t${RST} ${ILC}$1${RST}"
}

function _build_shader_cache()
{
    _info "compiling shaders to SPIR-V..."

    cd ./assets/shaders || _error "cd failed"
    if ! make -j $(nproc) ; then
        _error "shader compilation error" "failed to compile shaders to SPIR-V"
    fi
    cd ../.. || _error "cd failed"

    _success "shaders compiled to SPIR-V !"
}

function _check_cmd()
{
    if ! { command -v $1 > /dev/null; } 2>&1; then
        _error "command '$1' not found" "please install '$1' or 'nix develop' 🤓"
    fi
    _success "command '$1' found"
}

function _base_run()
{
    local cmake_args="$1"

    _check_cmd cmake
    _check_cmd ninja
    _check_cmd glslangValidator

    _info "updating external submodules..."
    git submodule update --init --recursive
    _success "updated external submodules !"
    _build_shader_cache
    mkdir -p build
    cd build || _error "mkdir failed"

    cmake .. -G Ninja $cmake_args
    if ! ninja $PROGRAM_NAME; then
        _error "compilation error" "failed to compile $PROGRAM_NAME"
    fi
    _success "compiled $PROGRAM_NAME"
}

function _all()
{
    _base_run "-DCMAKE_BUILD_TYPE=Release -DENABLE_DEBUG=OFF"
    exit 0
}

function _debug()
{
    _base_run "-DCMAKE_BUILD_TYPE=Debug -DENABLE_DEBUG=ON"
    exit 0
}

function _tests_run()
{
    _base_run "-DCMAKE_BUILD_TYPE=Debug -DENABLE_DEBUG=ON"
    cd .. || _error "cd failed"
    if ! ./unit_tests; then
        _error "unit tests error" "unit tests failed!"
    fi
    _success "unit tests succeed!"
    if [ "$(uname -s)" == 'Darwin' ]; then
        xcrun llvm-profdata merge -sparse unit_tests-*.profraw -o unit_tests.profdata
        xcrun llvm-cov report ./unit_tests -instr-profile=unit_tests.profdata -ignore-filename-regex='.*/tests/.*' -enable-name-compression > code_coverage.txt
    else
        gcovr -r . --exclude tests/ > code_coverage.txt
    fi
    cat code_coverage.txt
    exit 0
}

function _clean()
{
    rm -rf build
}

function _fclean()
{
    _clean
    rm -rf $PROGRAM_NAME unit_tests plugins code_coverage.txt unit_tests-*.profraw unit_tests.profdata vgcore* cmake-build-debug
}

for args in "$@"
do
    case $args in
        -h|--help)
            cat << EOF
USAGE:
      $0    builds $PROGRAM_NAME project

ARGUMENTS:
      $0 [-h|--help]    displays this message
      $0 [-d|--debug]   debug flags compilation
      $0 [-c|--clean]   clean the project
      $0 [-f|--fclean]  fclean the project
      $0 [-t|--tests]   run unit tests
EOF
        exit 0
        ;;
    -c|--clean)
        _clean
        exit 0
        ;;
    -f|--fclean)
        _fclean
        exit 0
        ;;
    -d|--debug)
        _debug
        ;;
    -t|--tests)
        _tests_run
        ;;
    -r|--re)
        _fclean
        _all
        ;;
    *)
        _error "Invalid arguments: " "$args"
    esac
done

_all

