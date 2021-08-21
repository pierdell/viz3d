BUILD_TYPE=$1

if [ -z "$BUILD_TYPE" ]
then
   BUILD_TYPE=Release
fi

CMakeGenerator="Unix Makefiles"
src_dir=$(pwd)
build_dir=${src_dir}/cmake-build-${BUILD_TYPE}
mkdir "${build_dir}"
mkdir "${build_dir}/external"

function check_command() { 
	echo "$(pwd)"
 }

cd "${build_dir}/external"
check_command

echo "[CT_ICP] -- Generating External Dependencies";
cmake -G "${CMakeGenerator}" -S ${src_dir}/external
check_command

echo "[CT_ICP] -- Building External Dependencies"
cmake --build . --config Release
check_command

echo "[CT_ICP] -- Generating CMake File"
cd ${build_dir}
cmake -G "${CMakeGenerator}" -S ${src_dir}
check_command

echo "[CT_ICP] -- Building Main Project"
cmake --build . --config Release --target install
check_command
