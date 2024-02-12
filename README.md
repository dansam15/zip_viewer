# zip_viewer
Cross-platform zip viewer

After cloning, use (in project dir)
>git submodule update --init minizip-ng/

Buid (in project dir):
>mkdir dbg
>cd dbg
>cmake ../ -DCMAKE_INSTALL_PREFIX=./ -DMZ_ZLIB=FALSE -DMZ_LZMA=FALSE -DMZBZIP2=FALSE -DMZ_ZSTD=FALSE -DMZ_BZIP2=FALSE

>cmake --build ./

Run:
>./zip_viewer ${PATH_TO_ZIP}
