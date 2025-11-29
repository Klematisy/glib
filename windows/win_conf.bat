

set ZLIB_LIB=-DZLIB_LIBRARY=%cd%\extdeps\zlib\lib\zlibstatic%BUILD_SYMBOL%.lib -DZLIB_INCLUDE_DIR=%cd%\extdeps\zlib\include
set PNG_LIB=-DPNG_LIBRARY=%cd%\extdeps\libpng\lib\libpng16_static%BUILD_SYMBOL%.lib -DPNG_PNG_INCLUDE_DIR=%cd%\extdeps\libpng\include
set BZIP2_LIB=-DBZIP2_LIBRARY=%cd%\extdeps\BZip2\lib\bz2_static.lib -DBZIP2_INCLUDE_DIR=%cd%\extdeps\BZip2\include
set BROTLI_LIB=-DBROTLIDEC_LIBRARY=%cd%\extdeps\brotli\lib\brotlienc-static.lib -DBROTLIDEC_INCLUDE_DIRS=%cd%\extdeps\brotli\include
set HarfBuzz_LIB=-DHarfBuzz_LIBRARIES=%cd%\extdeps\HarfBuzz\lib\harfbuzz.lib -DHarfBuzz_INCLUDE_DIR=%cd%\extdeps\HarfBuzz\include
set FREETYPE_LIB=-DFREETYPE_LIBRARY=%cd%\extdeps\freetype\lib\freetype%BUILD_SYMBOL%.lib -DFREETYPE_INCLUDE_DIRS=%cd%\extdeps\freetype\include\freetype2


set BUILD_TYPE=%~1

cmake -S . -B build ^
    -DCMAKE_BUILD_TYPE=%BUILD_TYPE% ^
    -DCMAKE_PREFIX_PATH=extdeps\freetype2\lib\cmake\freetype ^
     ZLIB_LIB PNG_LIB BZIP2_LIB BROTLI_LIB HarfBuzz_LIB ^
     FREETYPE_LIB