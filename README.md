# zadanie 1

now:
* refactor into functions

todo:
* handle invalid message on both sides

maybe:
* error on longer messages from client
* error on too long message from server in client

udp datagram sizes and fragmentation:
* what should buffer sizes be?
* udp: what happens if message is greater than the datagram?
* was that the point of the inner loop? and value 0?
* how do we get the datagram size?
* udp datagram size: max 65507, practical 534
* let's assume the practical limit for now


but msg in both can be shorter than 9 bytes; in this case report error