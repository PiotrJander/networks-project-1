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
*
Jeżeli datagramy
nadsyłane są szybciej, niż udaje się wysyłać odpowiedzi, to powinny być
umieszczane w buforze o rozmiarze 4096 datagramów, a jeżeli zabraknie miejsca
w buforze, to należy nadpisać najstarszy datagram

* so the buffer will be a circular queue
* the datagram queue will be 4096 x 534 = about twenty megabytes
* but we could also allocate that dynamically
* server always only saves first 9 bytes from datagrams

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
* add the message buffer without alloc
* when we read a message, save to buffer
* don't send anything for now


for test, debug with small queue like 3

ok, looks ready, try with debug now

ok now on enqueue we need to malloc, on dequeue free

