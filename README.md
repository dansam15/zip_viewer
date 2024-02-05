# zip_viewer
Cross-platform zip viewer

After cloning, use (in project dir)
>git submodule update --init minizip-ng/

Buid (in project dir):
>mkdir dbg
>cd dbg
>/usr/bin/cmake .. -DCMAKE_INSTALL_PREFIX=./

>make install

Run:
>./zip_viewer ${PATH_TO_ZIP}
