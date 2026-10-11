#!/usr/bin/env bash
set -euo pipefail

project_dir="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")/.." && pwd)"
build_dir="$project_dir/build"

cmake -S "$project_dir" -B "$build_dir" -DCMAKE_BUILD_TYPE=Debug
cmake --build "$build_dir" --parallel
ctest --test-dir "$build_dir" --output-on-failure
