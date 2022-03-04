set CMakeGenerator="Visual Studio 16 2019"
set src_dir=%cd%
set install_dir=%src_dir%\install
cd ..
set build_dir=%cd%\viz3d-build

mkdir %build_dir%
mkdir %build-dir%\external

cd %build_dir%\external

@echo "[VIZ3D] -- Generating External Dependencies"
cmake -G %CMakeGenerator% -S %src_dir%\external -DINSTALL_DIR=%install_dir%
@echo "[VIZ3D] -- Building External Dependencies"
cmake --build . --config Release

@echo "[VIZ3D] -- Generating CMake File"
cd %build_dir%
cmake -G %CMakeGenerator% -S %src_dir% -DINSTALL_DIR=%install_dir%
@echo "[VIZ3D] -- Building Main Project"
cmake --build . --config Release --target install

