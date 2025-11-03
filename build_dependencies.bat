set BUILD_TYPE=%~1

set ZLIB_LIB=-DZLIB_LIBRARY=%cd%/extdeps/zlib/lib/zlibd.lib -DZLIB_INCLUDE_DIR=%cd%/extdeps/zlib/include
set PNG_LIB=-DPNG_LIBRARY=%cd%/extdeps/libpng/lib/libpng16d.lib -DPNG_INCLUDE_DIR=%cd%/extdeps/libpng/include
set BZIP2_LIB=-DBZIP2_LIBRARIES=%cd%/extdeps/BZip2/lib/bz2.lib -DBZIP2_INCLUDE_DIR=%cd%/extdeps/BZip2/include
set BROTLI_LIB=-DBROTLIDEC_LIBRARIES=%cd%extdeps/brotli/lib/brotlidec.lib -DBROTLIDEC_INCLUDE_DIRS=%cd%extdeps/brotli/include
set HarfBuzz_LIB=-DHarfBuzz_LIBRARIES=%cd%/extdeps/HarfBuzz/lib/harfbuzz.lib -DHarfBuzz_INCLUDE_DIR=%cd%/extdeps/HarfBuzz/include


del extdeps
mkdir extdeps

cmake -S downloads\brotli -B downloads\brotli\build -DCMAKE_BUILD_TYPE=%BUILD_TYPE% -DCMAKE_INSTALL_PREFIX=extdeps\brotli
cmake --build downloads\brotli\build --parallel
cmake --install downloads\brotli\build --config %BUILD_TYPE%

sudo cmake -S downloads/BZip2 -B downloads/BZip2/build -DCMAKE_BUILD_TYPE=$(BUILD_TYPE)
sudo cmake --build downloads/BZip2/build --parallel
sudo cmake --install downloads/BZip2/build --prefix extdeps/BZip2 --config $(BUILD_TYPE)

cmake -S downloads\zlib -B downloads\zlib\build -DCMAKE_BUILD_TYPE=%BUILD_TYPE% -DCMAKE_INSTALL_PREFIX="extdeps\zlib"
cmake --build downloads\zlib\build --parallel
cmake --install downloads\zlib\build --config %BUILD_TYPE%

cmake -S downloads\libpng -B downloads\libpng\build -DCMAKE_BUILD_TYPE=%BUILD_TYPE% %ZLIB_LIB%
cmake --build downloads\libpng\build --parallel
cmake --install downloads\libpng\build --prefix extdeps\libpng --config %BUILD_TYPE%

cmake -S downloads\HarfBuzz -B downloads\HarfBuzz\build -DCMAKE_BUILD_TYPE=%BUILD_TYPE%
cmake --build downloads\HarfBuzz\build --parallel
cmake --install downloads\HarfBuzz\build --prefix extdeps\HarfBuzz --config %BUILD_TYPE%

cmake -S downloads\freetype -B downloads\freetype\build -DCMAKE_POLICY_VERSION_MINIMUM=3.5 ^
%ZLIB_LIB% %PNG_LIB% %BROTLI_LIB% %BZIP2_LIB% %HarfBuzz_LIB% -DCMAKE_INSTALL_PREFIX=extdeps\freetype ^
-DCMAKE_BUILD_TYPE=%BUILD_TYPE%

cmake --build downloads\freetype\build --parallel
cmake --install downloads\freetype\build --config %BUILD_TYPE%

xcopy downloads\msdf-atlas-gen extdeps\msdf-atlas-gen /E /I /H /Y
xcopy downloads\stb extdeps\stb /E /I /H /Y
xcopy downloads\glm extdeps\glm /E /I /H /Y
xcopy downloads\glfw extdeps\glfw /E /I /H /Y
xcopy downloads\glew extdeps\glew /E /I /H /Y