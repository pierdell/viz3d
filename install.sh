BUILD_TYPE=$1

if [ -z "$BUILD_TYPE" ]
then
   BUILD_TYPE=Release
fi

if [ -z "$CC" ]
then
  CC=gcc
  echo "Setting the compiler to $CC"
fi

if [ -z "$CXX" ]
then
  CXX=g++
  echo "Setting the compiler to $CXX"
fi

CMakeGenerator="Unix Makefiles"
src_dir=$(pwd)
build_dir=${src_dir}/cmake-build-${BUILD_TYPE}
install_dir=${src_dir}/install

mkdir "${build_dir}"
mkdir "${build_dir}/external"

function check_command() { 
	echo "$(pwd)"
 }

cd "${build_dir}/external"
check_command

echo "[VIZ3D] -- Generating External Dependencies";
cmake -G "${CMakeGenerator}" -S ${src_dir}/external -DINSTALL_DIR=${install_dir} -DCMAKE_C_COMPILER=$CC -DCMAKE_CXX_COMPILER=$CXX
check_command

echo "[VIZ3D] -- Building External Dependencies"
cmake --build . --config Release
check_command

echo "[VIZ3D] -- Generating CMake File"
cd ${build_dir}
cmake -G "${CMakeGenerator}" -S ${src_dir} -DINSTALL_DIR=${install_dir}
check_command

echo "[VIZ3D] -- Building Main Project"
cmake --build . --config Release --target install
check_command
