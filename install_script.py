import os
import platform
import zipfile
import requests
import io

extdeps_folder = "extdeps/"

download_file = "install_scripts/"

if platform.system() == "Windows":
    download_file += "win_deps_"
elif platform.system() == "Linux":
    download_file += "linux_deps_"
elif platform.system() == "Darwin":
    download_file += "darwin_deps_"

if platform.machine() == "x86_64":
    download_file += "x86_64.txt"
elif platform.machine() == "arm64":
    download_file += "arm64.txt"


file = open(download_file, 'r')

for line in file:
    filled_name = True
    name = ""
    url = ""
    for ch in line:
        if ch == ' ':
            filled_name = False
            continue
        if not filled_name:
            url += ch
        else:
            name += ch

    if os.path.isdir(extdeps_folder):
        if os.path.isdir(extdeps_folder + name):
            print("Is-up-to-date: <" + name + ">")
            continue

    url = url[:-1]
    print("Downloading " + name + "...")
    r = requests.get(url)
    if r.status_code != 200:
        print("Unexcepted error! (" + r.status_code + ")")
        print("Installer haven't downloaded " + name)
        continue
    z = zipfile.ZipFile(io.BytesIO(r.content))
    z.extractall(extdeps_folder)
    os.rename(extdeps_folder + z.namelist()[0][:-1], extdeps_folder + name)
file.close()
