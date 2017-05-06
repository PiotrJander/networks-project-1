ok now we have argument validation somewhere

but let me try to:

* server echoes to the client
* client listens in a loop and writes datagrams from the server to stdout
* client already listen for the server, but we would like to listen in infinite
    loop
* also we want some abstractions