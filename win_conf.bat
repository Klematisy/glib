set MSDFGEN_DIR=extdeps\msdf-atlas-gen
set FREETYPE_DIR=extdeps\freetype

set BUILD_TYPE=%~1

python -m venv venv
call .\venv\Scripts\activate

pip install requests

python install_script.py

cmake -S %FREETYPE_DIR% -B %FREETYPE_DIR%\build -DCMAKE_BUILD_TYPE=%BUILD_TYPE% -DCMAKE_POLICY_VERSION_MINIMUM="3.5"
cmake -S . -B build -DCMAKE_BUILD_TYPE=%BUILD_TYPE%
