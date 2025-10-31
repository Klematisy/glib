set MSDFGEN_DIR=extdeps\msdf-atlas-gen
set FREETYPE_DIR=extdeps\freetype

set BUILD_TYPE=%~1

python -m venv venv
call .\venv\Scripts\activate

pip install requests

python install_script.py

mkdir extdeps\freetype2

cmake -S %FREETYPE_DIR% -B %FREETYPE_DIR%\build -DCMAKE_BUILD_TYPE=%BUILD_TYPE% -DCMAKE_POLICY_VERSION_MINIMUM="3.5"
cmake --build %FREETYPE_DIR%\build --parallel
cmake --install %FREETYPE_DIR%\build --prefix extdeps\freetype2 --config %BUILD_TYPE%

cmake -S . -B build -DCMAKE_BUILD_TYPE=%BUILD_TYPE% -DCMAKE_PREFIX_PATH=extdeps\freetype2\lib\cmake\freetype\