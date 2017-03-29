cd ../examples/
make
cd ../userprog/build/
rm filesys.dsk
pintos-mkdisk filesys.dsk --filesys-size=2
pintos -f -q
pintos -p ../../examples/cat -a cat -- -q
pintos -p ../../examples/echo -a echo -- -q
pintos -p ../../examples/rm -a rm -- -q
pintos -p ../../examples/insult -a insult -- -q
pintos -p ../../examples/recursor -a recursor -- -q
pintos -p ../../examples/lineup -a lineup -- -q
pintos -p ../../examples/halt -a halt -- -q
pintos -p ../../examples/cp -a cp -- -q
pintos -p ../notes.txt -a notes.txt -- -q
cd ..
make
pintos run "echo hello world!"
pintos run "cat notes.txt"
pintos run "lineup notes.txt"
pintos run "cat notes.txt"
pintos run "cp notes.txt new.txt"
pintos ls
pintos run "cat new.txt"
pintos run "rm notes.txt"
pintos ls
pintos run "insult"
pintos run "recursor hello 2 2"
pintos run "halt"
