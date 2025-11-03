set MSDFGEN_DIR=extdeps\msdf-atlas-gen
set FREETYPE_DIR=extdeps\freetype

set BUILD_TYPE=%~1

python -m venv venv
call .\venv\Scripts\activate

pip install requests

python install_script.py


build_dependencies.bat %BUILD_TYPE%


cmake -S . -B build -DCMAKE_BUILD_TYPE=%BUILD_TYPE% -DCMAKE_PREFIX_PATH=extdeps\freetype2\lib\cmake\freetype\