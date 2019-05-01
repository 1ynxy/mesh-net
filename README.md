# mesh-net

Dissertation project on peer-to-peer network architectures for procedurally generated voxel worlds in C++.

When running on a headless server it must be run using a virtual framebuffer so that GLFW can initialise correctly.
This can be achieved using Xvfb.

Use of a program such as 'pngcheck -c [file]' is advised to ensure that all image assets pass CRC checks.

## dependencies

- GLFW3
- GLM
- GLAD (INCLUDED)
- LODEPNG (INCLUDED)

## progress

- [x] file loading & saving
- [x] configuration loading
- [x] thread safe debug logging
- [x] timer and fps cap handling
- [x] basic window creation & management
- [x] user input capture : mouse & keyboard
- [x] shared threaded resource handler
- [x] resource loading : sprites
- [x] resource loading : shaders
- [x] resource loading : meshes
- [ ] resource loading : samples
- [x] resource creation : sprites
- [x] resource creation : rendertextures
- [ ] basic sprite rendering
- [ ] gui interaction
- [ ] text rendering
- [x] basic mesh rendering
- [x] mesh texture application
- [ ] mesh dynamic lighting
- [x] basic component-entity system
- [x] base components : transform, camera, meshrenderer
- [ ] camera compositing
- [ ] post-processing effects
- [x] creation of server+client instance
- [x] connection to peers
- [x] protocol for data type differentiation
- [x] imaging of network structure
- [ ] ip address grepping
- [ ] smart reconnection handling
- [ ] smart network balancing
- [ ] game network data parsing
- [ ] basic player interactions | chat functions
- [ ] persistent data loading from existing peers

# standard project

## front cover

// INSERT FRONT COVER & DISCLAIMER //

## acknowledgements

//

## abstract

One of the primary costs associated with developing large scale online video games is that of continued hosting of non-community run game servers. Often, these are the only type of server available, as it allows the company more authority over how the servers are interacted with by the players.

This project aims to develop and test a method for enabling the distributed processing and hosting of virtual game worlds in which all clients act as equal nodes in the network. An attempt will also be made to introduce data persistence and load balancing to the resulting mesh network. The result will be a module written in C++, integrated loosely with a simple, custom, 3D capable game engine. This module will abstract all network events and methods in a way that can be utilised by any program, rather than being tied to a particular game engine.

## contents

- front cover
- disclaimer
- abstract
- contents
- introduction
- methodology
- testing & evaluation
- conclusion
- references
- appendices

## introduction, aims & objectives

The library will provide a "plug & play" capable solution to handling dynamic mesh networks for communicating game state data. In this instance a mesh network is that which does not contain any central device for data collation or connection handling. All peers are equal, and might be connected to the network through any existing, accessible node.

In order to provide a library that is as easy as possible to use for programmers who might not have studied network programming certain aspects will be abstracted. Sockets, for example, will be hidden, and peers will instead be addressed using Unique User Identification Numbers (UUIDs) which are consistent across the network. Events will be reduced to connections and disconnections. Reconnections will be handled as autonomously as possible, requiring no, or little, intervention by the user of the library. The primary protocol that differentiates network events and game data, and communicates message targets and sources,  will be hidden. Although this has the potential to reduce the capabilities of the library as a whole, it will hopefully open up use to programmers with less experience in the lower level handling of such systems.

## theory, design, & methodology

In order to create a fully decentralised mesh network all members of the network must be treated the same. Each node in the network will have to be as self-sufficient as possible. There is no one device that allocates identification numbers or target nodes to new connections. There is no one device that handles user input from each peer. There is no one source of information on the network structure. Instead, these tasks will have to be accomplished collaboratively, using a range of handshaking techniques designed to communicate information in the correct order.

The complexity of a mesh network is largely due to the need for all data to be synced between all peers that are connected, including data that was sent prior to a peer's connection. When a new device connects to the network it must somehow receive all of the game state changes so far so that it can catch up. The same method can be used to keep all members of the network up to date on the network structure itself. A range of methods for storing and transmitting this data present themselves, each with pros and cons.

### network structure

The first, and most intuitive, method for storing this network image during run-time is that of a tree data structure. As each member of the network is a node with a single parent and multiple children this method seems ideal, but it does present complications when attempting to quickly navigate the structure. Although use of pointers and references in the C++ language improves memory management, as, unlike a std::vector data type, when the structure is modified the memory does not necessarily have to be reallocated, the structure itself can be navigated bidirectionally. Unlike with a list, which would only have to be navigated in a single direction, a tree can be traversed in multiple ways, which both complicates algorithms and decreases efficiency of even simple tasks such as searching for a node with a certain identifying feature. Tree traversal methods include depth-first techniques, such as inorder, preorder and postorder.

One counter-argument to the memory management advantages that such a data structure provides is the use of linked lists, which work in a similar fashion, but reduces the number of child nodes to one, eliminating the need for complex traversal algorithms. Unfortunately, using only a list type structure does not allow for an accurate indication of network relationships, resulting in a list of peers without a discernible hierarchy.

A potential compromise comprises of a hybrid structure. A tree can be created, and each element of the tree can be also stored in a list for efficient information grepping. Duplicate data can be eliminated through the use of C++ pointers, although this can introduce problems with dangling pointers if not handled cautiously. A dangling pointer is a pointer which no longer point to a valid object of the appropriate type, most likely because the target object has been deleted. This method does introduce complexities in that a single modification of the network would require multiple operations to be correctly reflected in the data structures.

A peer in this network structure will have to contain all of the information required for identification and connection. It might also contain meta-data such as the peer name. An association must be made between the socket of the peer, if the peer is a direct connection, and the unique user identification number of the peer.

    Peer {
        int uuid;				// unique user identification number
        int sock;				// socket of connection

        string name;				// name of peer
        string addr;				// ip address of peer

        Peer* host;				// the peer that this peer is connected to
        vector<Peer*> children;			// the peers that are connected to this peer
    };

### protocol

In order to keep the network structure up to date a distinction must be made between packets which contain game state data, and packets which contain network event data. This is achieved using a protocol, or a layer in which the data is wrapped with a header which describes it. Examples of protocols include Transfer Control Protocol (TCP) and User Datagram Protocol (UDP). Both of these protocols act upon the same layer, and each have their advantages and disadvantages.

The User Datagram Protocol is a protocol for communicating datagrams between devices. A datagram is a set of information, potentially split into multiple packets, which is broadcast at the target device. No response is expected, meaning that this protocol does not open and maintain a connection between two devices. The sender does not know if the receiver has received the information, and as such, packet loss can occur. Packets can be received out-of-order, or not at all, but they will always contain the correct information. More light weight than TCP, and quicker due to the communication not requiring multiple steps, this protocol is often used when packet loss is acceptable and speed is important. The Transfer Control Protocol is a protocol for reliable two-way communication of data between multiple devices. A connection can be made and maintained, and while this connection is open any data can be sent and received through it. No packet loss will occur, and packets will arrive in the correct order.

In this use-case it would be best to create a protocol one layer under TCP. Reliability of communication is important, as a single lost packet could result in a desynced network image which could cause problems further into development. When it comes to developing a protocol there are a number of considerations to be made when designing the header, such as whether it is human readable or not, or whether it is a dynamic or fixed length. These options can influence network reliability, extensibility, speed, and efficiency, so it is important to find the correct balance.

protocol

fixed header:
- simpler to parse
- smaller header

dynamic header:
- more metadata
- can support multiple layers
- easier to implement changes later

examples of dynamic header

NEWCONN PEERID HOSTID
[7 NEWCONN 005 002]

SETIP PEERID PEERIP
[5 SETIP 005 192.168.0.15]

CONNLOST PEERID
[8 CONNLOST 005]

human readable vs machine readable

human readable:
- text in headers to indicate event types
- easier to debug

machine readable:
- replaces text in header with event ids
- smaller headers
- easier to parse
- requires more documentation 

## testing & evaluation

//

## conclusion

//

## references

//

## appendices

//
