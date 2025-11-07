set TYPE=%~1
set BUILD_TYPE=%~2

if "%TYPE%"=="install" (
    windows\install.bat %BUILD_TYPE%
) else if "%TYPE%"=="build_dependencies" (
    windows\build_dependencies.bat %BUILD_TYPE%
) else if "%TYPE%"=="configure" (
    windows\win_conf.bat %BUILD_TYPE%
)