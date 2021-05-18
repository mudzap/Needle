Too many dependencies! README comes later.

```bash
# Download the latest version (tested with 1.16), unzip and then:
cd src
make
# Then copy the AntTweakBar header file to the includes directory:
sudo cp ../include/AntTweakBar.h /usr/include/
# .. and copy the lib to the system lib dir (or add the path to LD_LIBRARY_PATH):
sudo cp ../lib/libAntTweakBar.so /usr/lib/i386-linux-gnu/ # For 32bit systems, or:
sudo cp ../lib/libAntTweakBar.so /usr/lib/x86_64-linux-gnu/ # for 64bit systems
```
```bash
sudo cp -r ./glm /usr/include/glm
```
