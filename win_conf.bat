set MSDFGEN_DIR=extdeps\msdfgen
set FREETYPE_DIR=extdeps\freetype

python -m venv venv
call .\venv\Scripts\activate

pip install requests

python install_script.py

cmake -S %FREETYPE_DIR% -B %FREETYPE_DIR%\build -DCMAKE_PREFIX_PATH="extdeps/freetype_install" -DCMAKE_POLICY_VERSION_MINIMUM="3.5"
cmake --build %FREETYPE_DIR%\build --parallel
cmake --install %(FREETYPE_DIR)%\build --prefix "extdeps\freetype_install" --config Debug

cmake -S %MSDFGEN_DIR% -B %MSDFGEN_DIR%\build -DCMAKE_PREFIX_PATH="extdeps/freetype_install/lib/cmake/freetype"
cmake --build %MSDFGEN_DIR%\build --parallel

cmake -S . -B build