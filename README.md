SocketNcurses
=============

What started as a learning exercise for sockets and Ncurses is now turning into a mini game

Usage:
SocketNcurses takes the following flags when run from the command line, but use them differently depending
on whether it's a server or client

-debug - enable debugging mode

-server - enable server mode

-client - enable client mode

-ip 192.168.1.1 - set the ip to listen on if a server or set ip to connect to as a client

-port 34567 - port to listen on if server or port to connect to if client


Example:
The server MUST be running before the client or the client will fail to connect atm
Run as server with debug output
./SocketNcurses -server -ip 192.168.1.1 -port 34567 -debug

Run as client and connect to the server
./SocketNcurses -client -ip 192.168.1.1 -port 34567 

Moving around in the game:
On the clients terminal once connected you can move your character around using

w - move character up

a - move character left

s - move character down

d - move character right

Notes:
The debug flag is kind of userless on the client atm because ncurses doesn't show terminal output
Multiple clients can be connected to the server and can see each other move around.
AFter slight modification it doesn't use as much bandwidth on the network but it still uses tonnes
about ~2MB/s now instead of 10. This is because the server is now restricted to 60fps instead of no restriction
and it just stomping the network.
