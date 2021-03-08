gcc -o lib/libvector_b.so -fPIC -shared -I./inc src/*_operator.c

gcc -o ./build/bin/dynamic -I./inc -L./lib ./test/test.c -lvector_b