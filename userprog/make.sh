cd ././../examples/
if [[ $(make) != "make: Nothing to be done for 'all'." ]]; then
    cd ././../userprog/build/
    cd build
    rm filesys.dsk
    pintos-mkdisk filesys.dsk --filesys-size=2
    pintos -f -q
    pintos -p ../../examples/echo -a echo -- -q
    pintos -p ../../examples/ls -a ls -- -q
    pintos -p ../../examples/pwd -a pwd -- -q
    cd ..
else
    cd ././../userprog/
fi
make