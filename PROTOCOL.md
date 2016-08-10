# Comsci Wire Protocol #
Comsci is based on the TCP transport with a custom wire protocol.

## General Information and Value Types ##
When values are given in hexadecimal format, e.g. ```0x03```, the given value
also indicates the width of the field. In the previous example, the field
would have a width of 1 byte.

The type POSITION_TYPE is to be a field 32 bits (4 bytes) in width.

The type POSITIONDIFF_TYPE is to be a field 32 bits (4 bytes) in width.

The type SPACER1_TYPE is to be a field 8 bits (1 byte) in width. It is defined
to have the value ```0x00```.

The type LAYER_TYPE is to be a field 8 bits (1 byte) in width.

The type ENTITY_TYPE is to be a field 16 bits (2 bytes) in width.

The type STRLEN_TYPE is to be a field 16 bits (2 bytes) in width.

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

When the server needs to change the state held by the clients, it will send a
ServerStateChanged message, containing the changes to be made. The
ServerStateChanged message contains a subtype, indicating what kind of change
is to be made.

When the client needs to send input to the server, the client will send a
ClientSendInput message containing the input. The server MUST respond with a
ServerInputRecieved message indicating that the message was recieved. If the
client does not recieve this confirmation, it should alert the user to
potential network connectivity problems. Proper handling for dropped input
is not currently implemented.

## General Packet Description ##
(numbers given above the diagram are byte offsets from the beginning of the packet)
```    
byte
0   1   2   3   4   5   6   7
+---+---+---+---+---+---+---+---+
|    Header     |Ver|Typ|  Len  |
+---+---+---+---+---+---+---+---+
| Data......
+---+---+---+---+
| Terminator    |
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
```
0x00 - Reserved
0x01 - ClientHello
0x02 - ServerHello
0x03 - ClientListGames
0x04 - ServerAvailableGames
0x05 - ClientJoinGame
0x06 - ServerJoinSuccess
0x07 - ServerJoinFailure
0x08 - ServerStateChanged
0x09 - ClientSendInput
0x0A - ServerInputRecieved
0x0B - ClientLeaveGame
0x0C - ClientGoodbye
0x0D - ServerGoodbye
0x0E - Heartbeat
```

### Terminator ###
Terminates a particular packet.
This field is 4 bytes in width and always contains the reserved value
```0xFFEEDD3E```.
The Terminator field is always aligned on a 4-byte boundary. If necessary,
SPACER1_TYPE characters shall be added before this field to ensure alignment.

### ServerStateChanged subtypes ###
```
0x00 FieldChangeRect
0x01 FieldChangeSparseMultiType
0x02 FieldChangeSparseSingleType
0x03 DisplayPrompt
0x04 InputPrompt
0x05 InputTimerUpdate
0x06 ActorResourceChange
0x07 ActorItemChange
0x08 ActorExistenceChange
```

#### ServerStateChanged Additional Specifications ####

##### 0x00 FieldChangeRect #####
The data should consist of a POSITION_TYPE indicating the upper left position
of the rectangle, followed by a POSITIONDIFF_TYPE indicating the horizontal
dimension of the rectangle, followed by a POSITIONDIFF_TYPE indicating the
vertical dimension of the rectangle, followed by a LAYER_TYPE indicating the
layer, followed by an array of 3 SPACER1_TYPE to ensure ENTITY_TYPE alignment,
followed by an array of size ```horizontal * vertical``` of type ENTITY_TYPE
that contains the actual data for the rectangle being updated. Each horizontal
line of entities will be sent, then the next line of entities, and so on,
until all data for the rectangle has been sent.

##### 0x01 FieldChangeSparseMultiType #####
Not yet defined.

##### 0x02 FieldChangeSparseSingleType #####
Not yet defined.

##### 0x03 DisplayPrompt #####
The data shall consist of a STRLEN_TYPE containing the number of *bytes* in
the message, followed by the message encoded in UTF-8 format, with a null
terminator.

##### 0x04 InputPrompt #####
Not yet defined.

##### 0x05 InputTimerUpdate #####
Not yet defined.
