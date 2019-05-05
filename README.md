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

Traditional video game networks are single-host many-client constructs. This requires the host to act differently to the clients in that it accepts connections, collates information, and handles client inputs. Not only does this result in more work, in order to keep both server and client programs up to date and compatible, but also the server instance has to be hosted temporarily, or in some cases indefinitely, by either the developers of the game or by the players themselves. This latter option is often referred to as community hosted server solutions. Both options have advantages and disadvantages for both the developers and the users.

Many developers do not opt to offer these server programs to the community, such as in Grand Theft Auto Five, because the design of the game requires more control over how the game is interacted with. Widely distributed server software that allows any user to host a game often is quickly reverse-engineered or modified in order to influence the gameplay or virtual world. In situations in which advantages can be gained through microtransactions such modifications can negate the need to pay, resulting in a loss of profit. Server software modification by users can also be advantageous. In the case of games such as Minecraft, especially the Java based client, user modification is a major selling point offering many times more hours of gameplay than vanilla gameplay can. User modifications can add missing features, fix bugs, improve performance, or substitute gameplay modes entirely.

Whether developer hosted or community hosted, in this form of network the server is nearly always authoritive. This means that information received from a client is not regarded as truth, but checked against peer inputs and other factors. If a client is found to be sending inputs which do not seem legitimate and might give the player an unfair advantage over other players they can be singled out and either reprimanded or removed from the network. The server is much less likely to be modified in order to give false information than a client is.

// describe traditional network structure  
// describe types of mesh network structure  
// justify choice of network structure  
// explain idea of data persistence in a mesh network  
// describe uses of data persistence  
// outline general methodology for data persistence  
// explain autonomous net events - reconnection, load balancing  
// point out issues with client based authority over server authority  

The library will provide a "plug & play" capable solution to handling dynamic mesh networks for communicating game state data. In this instance a mesh network is that which does not contain any central device for data collation or connection handling. All peers are equal, and might be connected to the network through any existing, accessible node.

In order to provide a library that is as easy as possible to use for programmers who might not have studied network programming certain aspects will be abstracted. Sockets, for example, will be hidden, and peers will instead be addressed using Unique User Identification Numbers (UUIDs) which are consistent across the network. Events will be reduced to connections and disconnections. Reconnections will be handled as autonomously as possible, requiring no, or little, intervention by the user of the library. The primary protocol that differentiates network events and game data, and communicates message targets and sources,  will be hidden. Although this has the potential to reduce the capabilities of the library as a whole, it will hopefully open up use to programmers with less experience in the lower level handling of such systems.

## theory, design, & methodology

In order to create a fully decentralised mesh network all members of the network must be treated the same. Each node in the network will have to be as self-sufficient as possible. There is no one device that allocates identification numbers or target nodes to new connections. There is no one device that handles user input from each peer. There is no one source of information on the network structure. Instead, these tasks will have to be accomplished collaboratively, using a range of handshaking techniques designed to communicate information in the correct order.

The complexity of a mesh network is largely due to the need for all data to be synced between all peers that are connected, including data that was sent prior to a peer's connection. When a new device connects to the network it must somehow receive all of the game state changes so far so that it can catch up. The same method can be used to keep all members of the network up to date on the network structure itself. A range of methods for storing and transmitting this data present themselves, each with pros and cons.

### network structure

The first, and most intuitive, method for storing this network image during run-time is that of a tree data structure. As each member of the network is a node with a single parent and multiple children this method seems ideal, but it does present complications when attempting to quickly navigate the structure. Although use of pointers and references in the C++ language improves memory management, as, unlike a std::vector data type, when the structure is modified the memory does not necessarily have to be reallocated, the structure itself can be navigated bidirectionally. Unlike with a list, which would only have to be navigated in a single direction, a tree can be traversed in multiple ways, which both complicates algorithms and decreases efficiency of even simple tasks such as searching for a node with a certain identifying feature. Tree traversal methods include depth-first techniques, such as inorder, preorder and postorder.

One counter-argument to the memory management advantages that such a data structure provides is the use of linked lists, which work in a similar fashion, but reduces the number of child nodes to one, eliminating the need for complex traversal algorithms. Unfortunately, using only a list type structure does not allow for an accurate indication of network relationships, resulting in a list of peers without a discernible hierarchy.

A potential compromise comprises of a hybrid structure. A tree can be created, and each element of the tree can be also stored in a list for efficient information grepping. Duplicate data can be eliminated through the use of C++ pointers, although this can introduce problems with dangling pointers if not handled cautiously. A dangling pointer is a pointer which no longer point to a valid object of the appropriate type, most likely because the target object has been deleted. This method does introduce complexities in that a single modification of the network would require multiple operations to be correctly reflected in the data structures.

A peer in this network structure will have to contain all of the information required for identification and connection. It might also contain meta-data such as the peer name and client version number. An association must be made between the socket of the peer, if the peer is a direct connection, and the unique user identification number of the peer.

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

A fixed header length simplifies the process and improves parsing speed for each packet transmitted. Less data is required to transmit the same header information, improving efficiency, but later modifications to the header structure can cause complications. If more header space is required at any point the result will no longer be compatible with older versions of the program. This header structure must be modified in every instance of code which either serialises or parses a packet, which can become time consuming.

A dynamic header, however, is easily extensible. If maintaining compatibility between versions of the program is important, as it might well be when it comes to persistent networks, then a dynamic header might be the better choice. The possibility of incompatibilities opens up a discussion into the value of allowing multiple iterations of the tool to connect to the same network, which will be saved for later.

The most sensible choice for development of a protocol, and a tool which implements said protocol, appears to be that of human readable headers over the alternative. This will greatly simplify debugging, as flaws in packet transmission or handling are immediately obvious, but the decision also ties into the choice between fixed and dynamic headers. Human readable information may differ in length, making fixed headers a limitation rather than an advantage. Instead, information such as data type can be stored as enumerable values, so that they can be parsed from machine readable data to human readable values more easily. Enumerating data also greatly improves efficiency, as assigning a single number to what would otherwise consist of multiple characters reduces the space the data takes up considerably. One disadvantage of this approach is that packet header structure must be documented thoroughly so that tools which capture packets for debugging purposes are not rendered useless, or at the very least more difficult to use.

One of the main goals of this project is to reduce the cost of server hosting. The main way in which this will be accomplished is by delegating the task of hosting to the users of the network, but that does not mean the program can then be inefficient when it comes to data usage. Not only will reducing the data transmission improve performance, but considering the user's situation is also important. Users might be subject to data caps or limits and so reducing the burden on them as much as possible is also a priority. Because of this, a fixed header with non-human readable data will be used, as this offers the greatest efficiency and speed when it comes to transmitting and parsing packets.

## testing & evaluation

- difference in data throughput compared to a normal network structure
- difference in number of connections handled compared to a normal network structure
- robustness of network image and autonomous reconnection handling
- difficulties arising from interaction between LAN & WAN networks
- estimated load and connection capacities
- possible techniques for improving efficiency provided by new network structure
- discussion of handling simultaneous network disconnection events
- outlier scenarios, such as root node disconnecting

## conclusion

//

## references

//

## appendices

//
