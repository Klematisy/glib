set BUILD_TYPE=%~1

if "%BUILD_TYPE%"=="Debug" (
  set BUILD_SYMBOL=d
)

set ZLIB_LIB=-DZLIB_LIBRARY=%cd%\extdeps\zlib\lib\zlibstatic%BUILD_SYMBOL%.lib -DZLIB_INCLUDE_DIR=%cd%\extdeps\zlib\include
set PNG_LIB=-DPNG_LIBRARY=%cd%\extdeps\libpng\lib\libpng16_static%BUILD_SYMBOL%.lib -DPNG_PNG_INCLUDE_DIR=%cd%\extdeps\libpng\include
set BZIP2_LIB=-DBZIP2_LIBRARY=%cd%\extdeps\BZip2\lib\bz2_static.lib -DBZIP2_INCLUDE_DIR=%cd%\extdeps\BZip2\include
set BROTLI_LIB=-DBROTLIDEC_LIBRARY=%cd%\extdeps\brotli\lib\brotlienc-static.lib -DBROTLIDEC_INCLUDE_DIRS=%cd%\extdeps\brotli\include
set HarfBuzz_LIB=-DHarfBuzz_LIBRARIES=%cd%\extdeps\HarfBuzz\lib\harfbuzz.lib -DHarfBuzz_INCLUDE_DIR=%cd%\extdeps\HarfBuzz\include
set FREETYPE_LIB=-DFREETYPE_LIBRARY=%cd%\extdeps\freetype\lib\freetype%BUILD_SYMBOL%.lib -DFREETYPE_INCLUDE_DIRS=%cd%\extdeps\freetype\include\freetype2

set BZIP2_LIB_TYPE=-DBZIP2_LIBRARY_RELEASE=%cd%\extdeps\BZip2\lib\bz2_static.lib

del extdeps
mkdir extdeps

cmake -S downloads\brotli -B downloads\brotli\build ^
    -DCMAKE_BUILD_TYPE=%BUILD_TYPE% ^
    -DCMAKE_INSTALL_PREFIX="%cd%\extdeps\brotli" ^
    -DBROTLI_BUILD_FOR_PACKAGE=ON

cmake --build downloads\brotli\build --parallel
cmake --install downloads\brotli\build --config %BUILD_TYPE%

cmake -S downloads\BZip2 -B downloads\BZip2\build ^
    -DCMAKE_BUILD_TYPE=%BUILD_TYPE% ^
    -DCMAKE_INSTALL_PREFIX="%cd%\extdeps\BZip2" ^
    -DENABLE_SHARED_LIB=OFF ^
    -DENABLE_STATIC_LIB=ON ^
    -DENABLE_STATIC_LIB_IS_PIC=ON ^
    -DENABLE_APP=OFF ^
    -DENABLE_TESTS=OFF ^
    -DENABLE_EXAMPLES=OFF ^
    -DENABLE_DOCS=OFF

cmake --build downloads\BZip2\build --parallel
cmake --install downloads\BZip2\build --config %BUILD_TYPE%

cmake -S downloads\zlib -B downloads\zlib\build ^
    -DCMAKE_BUILD_TYPE=%BUILD_TYPE% ^
    -DCMAKE_INSTALL_PREFIX="%cd%\extdeps\zlib"

cmake --build downloads\zlib\build --parallel
cmake --install downloads\zlib\build --config %BUILD_TYPE%

cmake -S downloads\libpng -B downloads\libpng\build ^
    -DCMAKE_BUILD_TYPE=%BUILD_TYPE% %ZLIB_LIB% ^
    -DCMAKE_INSTALL_PREFIX="%cd%\extdeps\libpng" ^
    -DPNG_SHARED=OFF -DPNG_STATIC=ON

cmake --build downloads\libpng\build --parallel
cmake --install downloads\libpng\build --config %BUILD_TYPE%

cmake -S downloads\HarfBuzz -B downloads\HarfBuzz\build ^
    -DCMAKE_BUILD_TYPE=%BUILD_TYPE% ^
    -DCMAKE_INSTALL_PREFIX="%cd%\extdeps\HarfBuzz" ^
    -DBUILD_SHARED_LIBS=OFF

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
    -DCMAKE_BUILD_TYPE=%BUILD_TYPE% ^
    -DBUILD_SHARED_LIBS=OFF -DFT_REQUIRE_BZIP2=ON

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
    -DMSDF_ATLAS_USE_VCPKG=OFF -DMSDF_ATLAS_USE_SKIA=OFF ^
    -DMSDF_ATLAS_DYNAMIC_RUNTIME=ON -DBUILD_SHARED_LIBS=OFF -DMSDF_ATLAS_BUILD_STANDALONE=OFF

cmake --build downloads\msdf-atlas-gen\build --parallel


xcopy downloads\msdf-atlas-gen extdeps\msdf-atlas-gen /E /I /H /Y
xcopy downloads\stb extdeps\stb /E /I /H /Y
xcopy downloads\glm extdeps\glm /E /I /H /Y
xcopy downloads\glfw extdeps\glfw /E /I /H /Y
xcopy downloads\glew extdeps\glew /E /I /H /Y