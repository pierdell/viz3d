BUILD_TYPE=$1

if [ -z "$BUILD_TYPE" ]
then
   BUILD_TYPE=Release
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

echo "[CT_ICP] -- Generating External Dependencies";
cmake -G "${CMakeGenerator}" -S ${src_dir}/external -DINSTALL_DIR=${install_dir}
check_status_code $?

echo "[CT_ICP] -- Building External Dependencies"
cmake --build . --config Release
check_status_code $?

echo "[CT_ICP] -- Generating CMake File"
cd ${build_dir}
cmake -G "${CMakeGenerator}" -S ${src_dir} -DINSTALL_DIR=${install_dir}
check_status_code $?

echo "[CT_ICP] -- Building Main Project"
cmake --build . --config Release --target install
check_status_code $?
