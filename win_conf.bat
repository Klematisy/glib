python -m venv venv
call .\venv\Scripts\activate

pip install requests

python install_script.py

cmake -S . -B build
cmake --build build --parallel