cd ../examples/
make
cd ../userprog/build/
rm filesys.dsk
pintos-mkdisk filesys.dsk --filesys-size=2
pintos -f -q
pintos -p ../../examples/cat -a cat -- -q
pintos -p ../../examples/echo -a echo -- -q
pintos -p ../notes.txt -a notes.txt -- -q
cd ..
make
pintos run "cat notes.txt"
