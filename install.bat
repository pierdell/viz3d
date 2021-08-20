set CMakeGenerator="Visual Studio 16 2019"
set src_dir=%cd%
cd ..
set build_dir=%cd%\viz3d-build

mkdir %build_dir%
mkdir %build-dir%\external

cd %build_dir%\external

@echo "[CT_ICP] -- Generating External Dependencies"
cmake -G %CMakeGenerator% -S %src_dir%\external
@echo "[CT_ICP] -- Building External Dependencies"
cmake --build . --config Release

@echo "[CT_ICP] -- Generating CMake File"
cd %build_dir%
cmake -G %CMakeGenerator% -S %src_dir%
@echo "[CT_ICP] -- Building Main Project"
cmake --build . --config Release --target install

