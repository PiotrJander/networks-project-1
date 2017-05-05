# zadanie1

TODO
* ./client timestamp c host [port]
* let client take the args
* client sends the datagram, and writes datagrams from server server in loop
* how do we send a 64 bit number over network?
* Jeżeli rok zawarty w znaczniku czasu jest mniejszy niż 1717 lub większy niż
  4242
* error messages to stderr, datagrams from server to stdout
* bad args -> stderr and exit code 1


timestamp: number in range
c: string of length one

timestamp: we want to use strlol or even ll
to have bigger ints, and also report