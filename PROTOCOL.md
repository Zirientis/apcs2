# Comsci Wire Protocol #
Comsci is based on the TCP transport with a custom wire protocol.

## General Packet Description ##
(numbers given above the diagram are byte offsets from the beginning of the packet)
```    
byte
0   1   2   3   4   5   6   7
+---+---+---+---+---+---+---+---+
|    Header     |Version|  Len  |
+---+---+---+---+---+---+---+---+
| Type  |
```

## Packet Fields ##
### Header ###
Identifies the packet as being created by Comsci.
This field always is the literal ASCII string ```CSWP```, without a null-terminator.

### Version ###
Identifies protocol version.
The current value of this field is 0x00.

### Len ###
The total size, in bytes, of the entire packet, headers and all.

### Type ###
Packet type. The following types are defined:
0x00 - Reserved
0x01 - ClientHello
0x02 - ServerHello
0x03 - ClientListGames
0x04 - ServerAvailableGames
0x05 - ClientJoinGame
0x06 -
