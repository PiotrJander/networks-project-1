ok now we have argument validation somewhere

but let me try to:

* server echoes to the client
* client listens in a loop and writes datagrams from the server to stdout
* client already listen for the server, but we would like to listen in infinite
    loop

* now refactor server as well
* ok now really go to server

* what does it mean that server receives message of length 0?
* one exchange = one datagram
*