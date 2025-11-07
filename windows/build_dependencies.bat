set BUILD_TYPE=%~1

if "%BUILD_TYPE%"=="Debug" (
	set BUILD_SYMBOL=d
)

set ZLIB_LIB=-DZLIB_LIBRARY=%cd%\extdeps\zlib\lib\zlib%BUILD_SYMBOL%.lib -DZLIB_INCLUDE_DIR=%cd%\extdeps\zlib\include
set PNG_LIB=-DPNG_LIBRARY=%cd%\extdeps\libpng\lib\libpng16%BUILD_SYMBOL%.lib -DPNG_PNG_INCLUDE_DIR=%cd%\extdeps\libpng\include
set BZIP2_LIB=-DBZIP2_LIBRARIES=%cd%\extdeps\BZip2\lib\bz2.dll -DBZIP2_INCLUDE_DIR=%cd%\extdeps\BZip2\include
set BROTLI_LIB=-DBROTLIDEC_LIBRARIES=%cd%\extdeps\brotli\lib\brotlidec.lib -DBROTLIDEC_INCLUDE_DIRS=%cd%\extdeps\brotli\include
set HarfBuzz_LIB=-DHarfBuzz_LIBRARIES=%cd%\extdeps\HarfBuzz\lib\harfbuzz.lib -DHarfBuzz_INCLUDE_DIR=%cd%\extdeps\HarfBuzz\include
set FREETYPE_LIB=-DFREETYPE_LIBRARY=%cd%\extdeps\freetype\lib\freetype$(BUILD_SYMBOL).lib -DFREETYPE_INCLUDE_DIRS=%cd%\extdeps\freetype\include

set BZIP2_LIB_TYPE=-DBZIP2_LIBRARY_RELEASE=%cd%\extdeps\BZip2\lib\bz2.lib

del extdeps
mkdir extdeps

cmake -S downloads\brotli -B downloads\brotli\build ^
    -DCMAKE_BUILD_TYPE=%BUILD_TYPE% ^
    -DCMAKE_INSTALL_PREFIX="%cd%\extdeps\brotli"

cmake --build downloads\brotli\build --parallel
cmake --install downloads\brotli\build --config %BUILD_TYPE%

cmake -S downloads\BZip2 -B downloads\BZip2\build ^
    -DCMAKE_BUILD_TYPE=%BUILD_TYPE% ^
    -DCMAKE_INSTALL_PREFIX="%cd%\extdeps\BZip2"

cmake --build downloads\BZip2\build --parallel
cmake --install downloads\BZip2\build --config %BUILD_TYPE%

cmake -S downloads\zlib -B downloads\zlib\build ^
    -DCMAKE_BUILD_TYPE=%BUILD_TYPE% ^
    -DCMAKE_INSTALL_PREFIX="%cd%\extdeps\zlib"

cmake --build downloads\zlib\build --parallel
cmake --install downloads\zlib\build --config %BUILD_TYPE%

cmake -S downloads\libpng -B downloads\libpng\build ^
    -DCMAKE_BUILD_TYPE=%BUILD_TYPE% %ZLIB_LIB% ^
    -DCMAKE_INSTALL_PREFIX="%cd%\extdeps\libpng"

cmake --build downloads\libpng\build --parallel
cmake --install downloads\libpng\build --config %BUILD_TYPE%

cmake -S downloads\HarfBuzz -B downloads\HarfBuzz\build ^
    -DCMAKE_BUILD_TYPE=%BUILD_TYPE% ^
    -DCMAKE_INSTALL_PREFIX="%cd%\extdeps\HarfBuzz"

cmake --build downloads\HarfBuzz\build --parallel
cmake --install downloads\HarfBuzz\build --config %BUILD_TYPE%

cmake -S downloads\freetype -B downloads\freetype\build ^
    -DCMAKE_POLICY_VERSION_MINIMUM=3.5 ^
     %ZLIB_LIB% ^
     %PNG_LIB% ^
     %BROTLI_LIB% ^
     %BZIP2_LIB% ^
     %BZIP2_LIB_TYPE% ^
    -DCMAKE_INSTALL_PREFIX="%cd%\extdeps\freetype" ^
    -DCMAKE_BUILD_TYPE=%BUILD_TYPE% -DBUILD_SHARED_LIBS=ON

cmake --build downloads\freetype\build --parallel
cmake --install downloads\freetype\build --config %BUILD_TYPE%

cmake -S downloads\freetype -B downloads\freetype\build ^
    -DCMAKE_POLICY_VERSION_MINIMUM=3.5 ^
     %ZLIB_LIB% ^
     %PNG_LIB% ^
     %BROTLI_LIB% ^
     %BZIP2_LIB% ^
     %BZIP2_LIB_TYPE% ^
    -DCMAKE_INSTALL_PREFIX="%cd%\extdeps\freetype" ^
    -DCMAKE_BUILD_TYPE=%BUILD_TYPE% -DBUILD_SHARED_LIBS=ON

cmake --build downloads\freetype\build --parallel
cmake --install downloads\freetype\build --config %BUILD_TYPE%


cmake -S downloads\msdf-atlas-gen -B downloads\msdf-atlas-gen\build ^
    -DCMAKE_POLICY_VERSION_MINIMUM=3.5 ^
     %ZLIB_LIB% ^
     %PNG_LIB% ^
     %BROTLI_LIB% ^
     %FREETYPE_LIB% ^
     %BZIP2_LIB% ^
    -DCMAKE_BUILD_TYPE=%BUILD_TYPE% ^
    -DMSDF_ATLAS_USE_VCPKG=OFF -DMSDF_ATLAS_USE_SKIA=OFF

cmake --build downloads\msdf-atlas-gen\build --parallel


xcopy downloads\msdf-atlas-gen extdeps\msdf-atlas-gen /E /I /H /Y
xcopy downloads\stb extdeps\stb /E /I /H /Y
xcopy downloads\glm extdeps\glm /E /I /H /Y
xcopy downloads\glfw extdeps\glfw /E /I /H /Y
xcopy downloads\glew extdeps\glew /E /I /H /Y