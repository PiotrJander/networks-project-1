# zadanie 1

now:
*

todo:
* handle invalid message on both sides; done?
* handle more than one datagram from a client
* server needs to keep a list of clients who sent to it within last two minutes
* client limit 42
* poll itself might block, but writes and reads on the socket must be non-blocking
* Wysyłanie datagramów nie powinno blokować ich odbierania
* test that when client send sth again, its time is updated
    but entry not duplicated



* suppose we had a partial write; then we need to have a persistent state
    of sending the datagram to clients
    - which client we're sending to
    - how many bytes we've already sent
    - if we have sent all bytes, proceed to next client

* we always save the datagram to the buffer (circular queue) first
if datagram being sent or datagram in queue
    if POLLOUT
        send as much as we can


super simple:
* enable saving client addresses in server
* debug and test adding clients

