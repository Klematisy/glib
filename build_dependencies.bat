set BUILD_TYPE=%~1

set ZLIB_LIB="-DZLIB_LIBRARY=^"extdeps/zlib/lib/libz.dylib^" -DZLIB_INCLUDE_DIR=^"extdeps/zlib^""
set PNG_LIB="-DPNG_LIBRARY=^"extdeps/libpng/lib/libpng16.dylib^" -DPNG_INCLUDE_DIR=^"extdeps/libpng/include^""
set BZIP2_LIB="-DBZIP2_LIBRARY=^"extdeps/brotli/lib/libbz2.dylib^" -DBZIP2_INCLUDE_DIR=^"extdeps/brotli/include^""
set HarfBuzz_LIB="-DHarfBuzz_LIBRARY=^"extdeps/HarfBuzz/lib/libharfbuzz.a^" -DHarfBuzz_INCLUDE_DIR=^"extdeps/HarfBuzz/include^""

del extdeps
Y
mkdir extdeps

cmake -S downloads\brotli -B downloads\brotli\build -DCMAKE_BUILD_TYPE=%BUILD_TYPE%
cmake --build downloads\brotli\build --parallel
cmake --install downloads\brotli\build --prefix extdeps\brotli --config %BUILD_TYPE%

cmake -S downloads\libpng -B downloads\libpng\build -DCMAKE_BUILD_TYPE=%BUILD_TYPE%
cmake --build downloads\libpng\build --parallel
cmake --install downloads\libpng\build --prefix extdeps\libpng --config %BUILD_TYPE%

cmake -S downloads\HarfBuzz -B downloads\HarfBuzz\build -DCMAKE_BUILD_TYPE=%BUILD_TYPE%
cmake --build downloads\HarfBuzz\build --parallel
cmake --install downloads\HarfBuzz\build --prefix extdeps\HarfBuzz --config %BUILD_TYPE%

cmake -S downloads\zlib -B downloads\zlib\build -DCMAKE_BUILD_TYPE=%BUILD_TYPE% -DCMAKE_INSTALL_PREFIX="extdeps\zlib"
cmake --build downloads\zlib\build --parallel
cmake --install downloads\zlib\build --config %BUILD_TYPE%

cmake -S downloads\freetype -B downloads\freetype\build 	    \
%ZLIB_LIB% %PNG_LIB% %BZIP2_LIB% %HarfBuzz_LIB% 		 	    \
-DCMAKE_INSTALL_PREFIX=extdeps\freetype -DCMAKE_BUILD_TYPE=%BUILD_TYPE%

cmake --build downloads\freetype\build --parallel
cmake --install downloads\freetype\build

xcopy downloads\msdf-atlas-gen extdeps\msdf-atlas-gen /E /I /H /Y
xcopy downloads\stb extdeps\stb /E /I /H /Y
xcopy downloads\glm extdeps\glm /E /I /H /Y
xcopy downloads\glfw extdeps\glfw /E /I /H /Y
xcopy downloads\glew extdeps\glew /E /I /H /Y