set TYPE=%~1
set BUILD_TYPE=%~2
set LINKING_TYPE=%~3

if "%TYPE%"=="install" (
    windows\install.bat %BUILD_TYPE%
) else if "%TYPE%"=="build_dependencies" (
    windows\build_dependencies.bat %BUILD_TYPE%
) else if "%TYPE%"=="configure" (
    windows\win_conf.bat %BUILD_TYPE% %LINKING_TYPE%
) else if "%TYPE%"=="clear" (
    del %cd%\downloads\brotli\build
    del %cd%\downloads\BZip2\build
    del %cd%\downloads\freetype\build
    del %cd%\downloads\libpng\build
    del %cd%\downloads\msdf-atlas-gen\build
)