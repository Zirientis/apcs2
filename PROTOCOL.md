# Comsci Wire Protocol #
Comsci is based on the TCP transport with a custom wire protocol.

## High-Level Overview ##
### Connecting ###
Upon connecting to a Comsci server, the client MUST send the ClientHello
message. Upon recieving the ClientHello message, the server MUST send the
ServerHello message. When the client wishes to find out about games on the
server, it MUST send the ClientListGames message. Upon recieving aforesaid
message, the server MUST respond with a ServerAvailableGames message. 

### Joining a game ###
To join a game, the sequence of events is as follows:
0. Client sends ClientJoinGame request, containing the ID of the game it that
   it wishes to join.
0. Server sends either ServerJoinSuccess or ServerJoinFailure.
0. If the client recieves ServerJoinSuccess, continue to the next step.
   If it recieves ServerJoinFailure, the request is considered to have failed.
0. The client should use the information provided in the server's response to
   construct the state required for rendering. The server will send the
   position of the client's character(s), the map tiles as seen by said
   character(s), and other general information, including, but not limited to,
   the number of other characters, information describing time limits, if any,
   and information about other players and the game itself.
0. At this point, the client has joined the game.

### Recieving game updates and sending user input ###
*This section does **not** refer to software updates of the client itself.*

As the game is played, it will frequently become necessary for the clients'
representation of the game to be updated, and for clients to transmit
information about player actions to the server.



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
