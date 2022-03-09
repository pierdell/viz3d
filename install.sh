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



check_status_code() {
   if [ $1 -ne 0 ]
   then
	echo "[VIZ3D] Failure. Exiting."
	exit 1
   fi
}


cd "${build_dir}/external"
check_status_code $?

echo "[VIZ3D] -- Generating External Dependencies";
cmake -G "${CMakeGenerator}" -S ${src_dir}/external -DCMAKE_BUILD_TYPE=${BUILD_TYPE} -DINSTALL_ROOT=${install_dir} -DCMAKE_C_COMPILER=$CC -DCMAKE_CXX_COMPILER=$CXX
check_status_code $?

echo "[VIZ3D] -- Building External Dependencies"
cmake --build . --config Release
check_status_code $?

echo "[VIZ3D] -- Generating CMake File"
cd ${build_dir}
cmake -G "${CMakeGenerator}" -S ${src_dir} -DINSTALL_ROOT=${install_dir} -DCMAKE_BUILD_TYPE=${BUILD_TYPE}
check_status_code $?

echo "[VIZ3D] -- Building Main Project"
cmake --build . --config Release --target install
check_status_code $?
