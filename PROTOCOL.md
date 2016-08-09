# Comsci Wire Protocol #
Comsci is based on the TCP transport with a custom wire protocol.

## General Packet Description ##
(numbers given above the diagram are byte offsets from the beginning of the packet)
```
0      78       1516
|------||--------||
 HEADER   LEN
 (0-7)    (8-15)

```

## Packet Fields ##
### HEADER ###
Identifies the packet as being created by Comsci.
This field always is the literal ASCII string ```Comsci```, without a null-terminator.

### LEN ###
The total size, in bytes, of the entire packet, headers and all.

###
