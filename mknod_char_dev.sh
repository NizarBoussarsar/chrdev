rm -f /dev/cbstat
rm -f /dev/cutbuf1
rm -f /dev/cutbuf2
rm -f /dev/cutbuf3
rm -f /dev/cutbuf4
mknod /dev/cbstat c 100 0
mknod /dev/cutbuf1 c 100 1
mknod /dev/cutbuf2 c 100 2
mknod /dev/cutbuf3 c 100 3
mknod /dev/cutbuf4 c 100 4
