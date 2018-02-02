#yum install gcc
#apt-get install gcc
gcc -static -c sockets.c -o sockets.o
ar -rcs libsockets.a sockets.o

# DESCOMENTAR SI ES ¡CENTOS 6!
#gcc main.c -o main -std=gnu99 -L. -lsockets

#Descomentar si es debian o ubuntu
#gcc main.c -o main -std=gnu11 -L. -lsockets
