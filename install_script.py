import os
import platform
import zipfile
import tarfile
import requests
import shutil
import io

downloads_folder = "downloads/"

download_file = "install_scripts/"

if platform.system() == "Windows":
    download_file += "win_deps_"
elif platform.system() == "Linux":
    download_file += "linux_deps_"
elif platform.system() == "Darwin":
    download_file += "darwin_deps_"

if platform.machine() == "x86_64" or platform.machine() == "AMD64":
    download_file += "x86_64.txt"
elif platform.machine() == "arm64" or platform.machine() == "ARM64":
    download_file += "arm64.txt"

file = open(download_file, 'r')


def unpack_file(url, r):
    l = len(url)
    name = ""
    if (url[l - 3] + url[l - 2] + url[l - 1] == "zip"):
        z = zipfile.ZipFile(io.BytesIO(r.content))
        z.extractall(downloads_folder)
        name = z.namelist()[0][:-1]
        z.close()
    elif (url[l - 2] + url[l - 1] == "gz"):
        tar_bytes = io.BytesIO(r.content)
        tar = tarfile.open(fileobj=tar_bytes, mode="r:gz")
        tar.extractall(downloads_folder)
        name = tar.getnames()[0]

        tar.close()

    return name


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

    if os.path.isdir(downloads_folder):
        if os.path.isdir(downloads_folder + name):
            print("Is-up-to-date: <" + name + ">")
            continue
    if url[len(url) - 1] == '\n':
        url = url[:-1]

    print("Downloading " + name + "...")
    r = requests.get(url)
    if r.status_code != 200:
        print("Unexcepted error! (", r.status_code, ")")
        print("Installer haven't downloaded " + name)
        continue

    package_name = unpack_file(url, r)
    os.rename(downloads_folder + package_name, downloads_folder + name)
file.close()

shutil.rmtree("downloads/msdf-atlas-gen/msdfgen")
shutil.move("downloads/msdfgen", "downloads/msdf-atlas-gen")
shutil.move("downloads/artery-font/artery-font", "downloads/msdf-atlas-gen/artery-font-format")
shutil.rmtree("downloads/artery-font")

temp = open("downloads/msdf-atlas-gen/msdfgen/msdfgen-config.h", "w", encoding="utf-8")
temp.close()