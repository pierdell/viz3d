BUILD_TYPE=$1

if [ "$BUILD_TYPE" -z ]
then
   BUILD_TYPE=Release
fi

CMakeGenerator="Unix Makefiles"
src_dir=$(pwd)
build_dir=src_dir/cmake-build-${BUILD_TYPE}
mkdir ${build_dir}
mkdir ${build-dir}/external

cd ${build_dir}/external

echo "[CT_ICP] -- Generating External Dependencies"
cmake -G "${CMakeGenerator}" -S ${src_dir}/external
echo "[CT_ICP] -- Building External Dependencies"
cmake --build . --config Release

echo "[CT_ICP] -- Generating CMake File"
cd ${build_dir}
cmake -G "${CMakeGenerator}" -S ${src_dir}
echo "[CT_ICP] -- Building Main Project"
cmake --build . --config Release --target install

