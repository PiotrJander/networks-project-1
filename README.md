# zadanie 1

now:
*

todo:
* handle invalid message on both sides; done?
* handle more than one datagram from a client (ignore? update last sent time?)




* server needs to keep a list of clients who sent to it within last two minutes
* client limit 42

* Wysyłanie datagramów nie powinno blokować ich odbierania
*
Jeżeli datagramy
nadsyłane są szybciej, niż udaje się wysyłać odpowiedzi, to powinny być
umieszczane w buforze o rozmiarze 4096 datagramów, a jeżeli zabraknie miejsca
w buforze, to należy nadpisać najstarszy datagram



* mamy tylko jeden deskryptor! how not to lose incoming datagrams??

* so the buffer will be a circular queue
* the datagram queue will be 4096 x 534 = about twenty megabytes
* but we could also allocate that dynamically

* do we need to zero revents every time? yes

* we could produce a bug and then fix it
* flood server with datagrams
*


* the POLLOUT flag means that the write buffer for the socket is not full
* if no POLLOUT flag, store datagram in buffer instead
* still, if not enough space in buffer, write can block
* if process blocks, it can't handle the other connections

* server always only saves first 9 bytes from datagrams - malloc anyway
* suppose we start sending a response datagram
* we send it to up to 42 clients
* for each write / send

* suppose we had a partial write; then we need to have a persistent state
    of sending the datagram to clients
    - which client we're sending to
    - how many bytes we've already sent
    - if we have sent all bytes, proceed to next client

* we always save the datagram to the buffer (circular queue) first
if datagram being sent or datagram in queue
    if POLLOUT
        send as much as we can


small steps: still single client case, but use poll now

create poll struct
set revents to 0
poll with inf timeout
when poll returns, check flags
if POLLIN, read datagram to cqueue
if cqueue not empty or datagram being sent, and POLLOUT, do a write

even simpler:

add the poll, but simply echo the message
simple! socket is always ready for writing
this means space is buffer after all!









