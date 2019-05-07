# mesh-net

Dissertation project on peer-to-peer network architectures for procedurally generated voxel worlds in C++.

When running on a headless server it must be run using a virtual frame buffer so that GLFW can initialise correctly.
This can be achieved using Xvfb.

Use of a program such as 'pngcheck -c [file]' is advised to ensure that all image assets pass CRC checks.

## dependencies

- GLFW3
- GLM
- GLAD (INCLUDED)
- LODEPNG (INCLUDED)

## progress

artefact  
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
- [x] resource creation : render textures
- [ ] basic sprite rendering
- [ ] gui interaction
- [ ] text rendering
- [x] basic mesh rendering
- [x] mesh texture application
- [ ] mesh dynamic lighting
- [x] basic component-entity system
- [x] base components : transform, camera, mesh renderer
- [ ] camera compositing
- [ ] post-processing effects
- [x] creation of server+client instance
- [x] connection to peers
- [x] protocol for data type differentiation
- [x] imaging of network structure
- [x] ip address grepping
- [ ] smart reconnect handling
- [ ] smart network balancing
- [ ] game network data parsing
- [ ] basic player interactions | chat functions
- [ ] persistent data loading from existing peers

dissertation  
- [x] abstract: scenario
- [x] abstract: proposition
- [x] introduction: host-clients structure
- [x] introduction: dedicated versus community
- [x] introduction: server authoritative
- [x] introduction: mesh-network architecture
- [x] introduction: mesh-network potential pitfalls
- [ ] introduction: network architecture justification
- [x] introduction: project goals & deliverable
- [ ] introduction: unattainable targets
- [ ] methodology: target device architecture
- [ ] methodology: posix versus win32
- [ ] methodology: required libraries
- [ ] methodology: sockets & file descriptors
- [ ] methodology: threading & blocking
- [ ] methodology: network structure
- [ ] methodology: mesh protocol
- [ ] methodology: network event breakdown
- [ ] methodology: basic network imaging
- [ ] methodology: connection handshake
- [ ] methodology: addressing peers
- [ ] methodology: ip address grepping
- [ ] methodology: reconnect events
- [ ] methodology: load balancing
- [ ] methodology: architecture cross-compatibility
- [ ] evaluation: data throughput
- [ ] evaluation: socket load
- [ ] evaluation: computational load
- [ ] evaluation: debugging process
- [ ] evaluation: example scenario
- [ ] evaluation: discussion on reliability
- [ ] evaluation: alternative potential use cases
- [ ] conclusion: evaluation of progress
- [ ] conclusion: target versus result
- [ ] conclusion: were I given a redo
- [ ] conclusion: viability of idea

# standard project

## front cover

// INSERT FRONT COVER & DISCLAIMER //

## acknowledgements

//

## abstract

### scenario

One of the primary costs associated with developing multi-player online video games is that of dedicated server hosting. Ranging from short-lived competitive or co-op game instances to massive multi-player multi-instanced virtual worlds, these servers require both large amounts of computational power and a high data throughput. The large sums of money set aside for these dedicated hosting devices can affect both the game's development and its lifetime significantly.

### proposition

This project aims to develop and test a method for enabling the distributed processing and hosting of virtual game worlds in which all clients act as equal nodes in the network. The result will be compared to a more traditional network architecture and the viability of the mesh network will be evaluated. An attempt will also be made to introduce data persistence and load balancing to the resulting mesh network. The result will be a module written in C++, integrated loosely with a simple, custom, 3D capable game engine. This module will abstract all network events and methods in a way that can be utilised by any program, rather than being tied to a particular tool.

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

### host-clients structure

Traditional video game networks are single-host many-client constructs. A server is created which can then be connected to by any number of clients. This server is often distributed and run as a separate tool to the client software. Not only does this result in more work for developers, in order to keep both server and client programs up to date and compatible, but also the server instance has to be hosted temporarily, or in some cases indefinitely, by either the developers of the game or by the players themselves. This latter option is often referred to as community hosted server solutions. Both options have advantages and disadvantages for both the developers and the users.

// INSERT DIAGRAM OF TRADITIONAL HOST-CLIENTS NETWORK STRUCTURE

### dedicated versus community

Many developers do not opt to offer these server programs to the community, such as in Grand Theft Auto Five, because the design of the game requires more control over how the game is interacted with. Widely distributed server software that allows any user to host a game often is quickly reverse-engineered or modified in order to influence the gameplay or virtual world. In situations in which advantages can be gained through micro-transactions such modifications can negate the need to pay, resulting in a loss of profit, but server software modification by users can also be advantageous. In the case of games such as Minecraft, especially the Java based client, user modification is a major selling point offering many times more hours of gameplay than the vanilla client can. User modifications can add missing features, fix bugs, improve performance, or substitute gameplay modes entirely.

### server authoritative

Whether developer hosted or community hosted, in this form of network the server is nearly always authoritative. This means that information received from a client is not regarded as truth, but checked against peer inputs and other factors. If a client is found to be sending inputs which do not seem legitimate and might give the player an unfair advantage over other players they can be singled out and either reprimanded or removed from the network. The server is much less likely to be modified in order to give false information than a client is, which improves the security of this network structure significantly over alternatives.

### mesh-network architecture

An alternative network structure is that of a mesh network. In a mesh network there is no central host; all peers are connected directly to each other or to another peer that is connected to the network. In a full mesh network each and every peer is connected to each other, creating a large number of redundant connections, but ensuring connection at all times to every other member of the network. In a partial mesh network some clients may be connected to other peers directly, but not to all. All data must then be forwarded around the network until every member has received it. Although this latter option reduces the number of connections that each peer has to handle at any one time, the amount of time processing the data transmitted is increased as they have to both receive and forward packets. A single dropped connection can also isolate a single peer, or even a number of peers, if no redundant connections are made. With the right information a reconnect attempt can be made, but this can still result in missed packets or downtime for the disconnected peer.

// INSERT DIAGRAM OF MESH-NETWORK BOTH FULL & PARTIAL

### mesh-network potential pitfalls

There are multiple major pitfalls with both this network architecture and the method used that will be addressed but not all will solved in the demonstration.

The first is the issue of authority. As there is no one central device that is not a user, there is no viable peer that can be selected as an authoritative member of the network. Every peer has the same level of trust, and so the network is open to exploitation. One potential solution to this problem is to provide versions of the client which do not act as in-game entities, but continue to interact with the network as is normal. These clients will be able to compare and analyse inputs from all peers connected to the network and will be able to single out potential malicious devices. Other solutions include industry standard methods of anti-tampering, such as hashing game files and comparing results with expected hashes or those of peers in the network. Unfortunately a number of these methods are no longer applicable with no central authority.

A second hurdle which will not be solved is a problem with handling connections between devices that are on different networks and devices that are on the same network, or devices on Local Area Network (LAN) versus devices on Wide Area Network (WAN). Any users that wish to be accessible from outside of their local network must port-forward the connection on their router. The only way to test if this port forward is active is to connect to the device from outside of the network, which complicates the process considerably. Listing device accessibility in the network image is a must, as this information is required when picking a new target for reconnect. A number of complex scenarios can arise from this problem, one of which is described below.

// INSERT DIAGRAM & EXPLANATION OF PORT-FORWARDED ROOT-HOST|CHILD RECONNECT

Another major problem occurs when the root, or first node disconnects from the network. As this is the first node to join the network it should be at the top of the hierarchy. If it has multiple children, a race condition of sorts will be created, as each of the children search for a new host. In particularly unfortunate circumstances each peer might attempt to connect to a node that is lower in the hierarchy of another, creating a loop in which messages might get caught indefinitely. The first solution that comes to mind is for each host to elect a child to take its position should it disconnect, or to rank children by the order in which position inheritance should occur.

// INSERT DIAGRAM OF PEER RECONNECT RACE CONDITION

### network architecture justification

The target is to create a mesh network which can easily be implemented into any game or library without much prior knowledge of network structures, socket handling and data transfer. The focus of the project is primarily on that of a partial mesh network over a full mesh network. This is due to the need to keep the program light weight, which would be compromised by the creation of a connection for each peer in the network, and the simplicity of a tree structured network. A full mesh network design would introduce a number of complexities which would reduce the amount of time allotted for testing the network capabilities.

// REWORD / REWRITE ABOVE

### project goals & deliverable

The target of this project is to construct a partial mesh network without redundancies, instead including network imaging that allows for reconnect attempts to be made. This will reduce the number of connections each peer has to handle, but might introduce unreliable aspects to the network. As this is only a demonstration the aim is not to be completely reliable, but to prove that this structure is fast enough, yet light enough, to run on user machines without impacting performance unacceptably.

This network imaging will require the development of a protocol that will run on top of TCP or UDP that can differentiate game data and network packets. These network packets will be utilised in the construction of a network tree on each peer that will provide the required information for message targeting and autonomous reconnect handling. This network tree will have to be persistent across the network, meaning that new connections must be able to receive the serialised network image, parse it, and then add themselves to the network, broadcasting this event to the rest of the network. This is a basic form of data persistence in a mesh network; a concept which will also be employed in the game state syncing itself. This network image could potentially also be used to reduce the amount of data bouncing around the network. If each packet includes target and source identification it can be forwarded only to peers which are in the direct path to the target peer. In this way an advantage can be gained over traditional network structures.

// DIAGRAM OF OSI MODEL / SOMETHING DISPLAYING NETWORK LAYERING

Using this constructed network image, the library will handle reconnect and load balancing autonomously. Information on an active peer's address, port forward status, and current load can be used to pick an appropriate target to attempt reconnect. Any children that the disconnected peer owns will have to be serialised and sent to the new host if connection is regained, eliminating the need for all disconnected peers in the hierarchy to reconnect. The amount of time that this operation takes is important, as it will determine the number of packets missed. It might instead be possible for disconnected peers to drop persistent data and request a new game state image upon reconnect to the network. This will negate the potential issues that reconnect presents, but will increase the load upon the new host and the network as a whole as more data is transmitted. Another approach might be to connect to multiple hosts and pick a single host to communicate through, switching if that host is lost. This approach will reduce the chance of complete disconnections, but not eliminate them entirely.

This project aims to provide a "plug & play" solution to handling dynamic mesh networks for communicating game state data. In order to provide a library that is as accessible as possible certain aspects will be abstracted. Sockets, for example, will be hidden, and peers will instead be addressed using Unique User Identification Numbers (UUIDs) which are consistent across the network. Events will be reduced to connections and disconnections. Reconnecting will be handled as autonomously as possible, requiring little or no intervention by the user. The primary protocol that differentiates network events and game data and communicates message targets and sources will be hidden. Although this has the potential to reduce the flexibility of the library it will hopefully open up use to programmers with less experience in the lower level handling of such systems.

### unattainable targets

The time given for this project is limited and existing expertise when it comes to network handling is minimal. Because of this there are a number of tasks which might not be completed by the deadline. These tasks may compromise the reliability of the network but should not prevent a basic demonstration of the concept from running. Hurdles that might not be overcome include handling of the disparity between WAN and LAN networks, as this requires a larger amount of IP address handling and comparison, and the persistence of game state data itself, as this utilises the same principles as persistent network imaging does, only on a different layer of the network. Anti-cheat and exploitation methods will also very likely not be implemented, as they are not necessary for the operation of the network and involve multiple other disciplines in programming that have not been explored.

## theory, design, & methodology

### target device architecture

The eventual goal is to write the library to be as portable as possible, but due to the difference in the way sockets behave between architectures there will always be some outliers. The most popular systems include Windows, Linux, and Mac, but these can provide overlap with alternative, similarly structured systems. Programs that will build for Linux will often also build for Mac and OpenBSD, for example. Because of this, Linux seems to be the best target architecture that will allow for the largest number of potential devices. A choice should be made between the different CPU architectures, such as x86-x64, x86, and ARM, with the potential inclusion of newer architectures such as Risc-V. Because x86-x64 based machines often are more capable than the alternatives it might be easiest to develop solely for this architecture, especially as most development machines will also be compatible, but this does not mean that the library can't be test built for a lower-power architecture such as ARM. The addition of ARM devices decreases the cost of testing, as they often are cheap to purchase and set up, allowing for a small debugging network to be constructed easily. Windows is a lot less easily available on ARM devices than Linux or OpenBSD is, which also negatively effects the cost of development for Windows systems.

To aide in multi-platform development the language used will be C++. Unlike a lot of languages which compile to byte-code and which must be interpreted by the system or by a virtual environment, such as with C# or Java, C++ is compiled into machine code which can run anywhere (or at least anywhere it is compiled to work on). C++ also provides good implementations of the Posix compliant and WinSock libraries for network construction, as well as a range of tools and data structures which will be useful. As the platform chosen is Linux and the language is C++ it becomes obvious that the build system will be GCC. The construction of Makefiles can be simplified using tools such as CMake.

### posix versus win32

// compatibility with architectures & availability  
// differences in implementation  
// 

### required libraries

// window handling & graphics / opengl  
// sockets & file descriptors  
// architecture agnostic  
// 

### socket handling & file descriptors

// file descriptor sets  
// server & client merger  
// select versus poll  
// 

### threading & blocking

// non-blocking calls  
// multi-threaded using conditional variables  
// mutex protections  
// 

### network structure

The first, and most intuitive, method for storing a network image during run-time is that of a tree data structure. As each member of the network is a node with a single parent and multiple children this method seems ideal, but it does present complications when attempting to quickly navigate the structure. Although use of pointers and references in the C++ language improves memory management as, unlike with a vector data type, when the structure is modified the memory does not necessarily have to be reallocated, the structure itself can be navigated bidirectionally. Unlike with a list, which would only have to be navigated in a single direction, a tree can be traversed in multiple ways, which both complicates algorithms and decreases efficiency of even simple tasks such as searching for a node with a certain identifying feature. Tree traversal methods include depth-first techniques, such as inorder, preorder and postorder.

One counter-argument to the memory management advantages that such a data structure provides is the use of linked lists, which work in a similar fashion, but reduces the number of child nodes to one, eliminating the need for complex traversal algorithms. Unfortunately, using only a list type structure does not allow for an accurate indication of network relationships, resulting in a list of peers without a discernible hierarchy.

// DIAGRAM OF NETWORK TREE AND LIST COMBINATION

A potential compromise comprises of a hybrid structure. A tree can be created, and each element of the tree can be also stored in a list for efficient information grepping. Duplicate data can be eliminated through the use of C++ pointers, although this can introduce problems with dangling pointers if not handled cautiously. A dangling pointer is a pointer which no longer point to a valid object of the appropriate type, most likely because the target object has been deleted. This method does introduce complexities in that a single modification of the network would require multiple operations to be correctly reflected in the data structures.

A peer in this network structure will have to contain all of the information required for identification and connection. It might also contain meta-data such as the peer name and client version number. An association must be made between the socket of the peer, if the peer is a direct connection, and the unique user identification number of the peer.

// DIAGRAM OF PEER STRUCTURE

    Peer {
        int uuid;						// unique user identification number
        int sock;						// socket of connection

        string name;					// name of peer
        string addr;					// ip address of peer

        Peer* host;						// the peer that this peer is connected to
        vector<Peer*> children;			// the peers that are connected to this peer
    };

### mesh protocol

In order to keep this network structure up to date a distinction must be made between packets which contain game state data, and packets which contain network event data. This is achieved using a protocol, or a layer in which the data is wrapped with a header which describes it. Examples of protocols include Transfer Control Protocol (TCP) and User Datagram Protocol (UDP). Both of these protocols act upon the same layer, and each have their advantages and disadvantages.

The User Datagram Protocol is a protocol for communicating datagrams between devices. A datagram is a set of information, potentially split into multiple packets, which is broadcast at the target device. No response is expected, meaning that this protocol does not usually open and maintain a connection between two devices, and is known as a connectionless protocol. The sender does not know if the receiver has received the information, and as such, packet loss can occur. Packets can be received out-of-order, or not at all, but they will always contain the correct information. More light weight than TCP, and quicker due to the communication not requiring multiple steps, this protocol is often used when packet loss is acceptable and speed is important. The Transfer Control Protocol is a protocol for reliable two-way communication of data between multiple devices. A connection can be made and maintained, and while this connection is open any data can be sent and received through it. No packet loss will occur, and packets will arrive in the correct order.

In this use-case it would be best to create a protocol one layer under TCP. Reliability of communication is important, as a single lost packet could result in a de-synced network image which could cause problems further into development. Speed of transmission is not so important in this demonstration, although in a full commercial product it would be a significant discussion point. Many fast paced first person shooter games have picked UDP over TCP in order to reduce latency of packets, such as Quake 3. When it comes to developing a protocol there are a number of considerations to be made when designing the header, such as whether it is human readable or not, or whether it is a dynamic or fixed length. These options can influence network reliability, extensibility, speed, and efficiency so it is important to find the correct balance.

A fixed header length simplifies the process and improves parsing speed for each packet transmitted. Less data is required to transmit the same header information, improving efficiency, but later modifications to the header structure can cause complications. If more header space is required at any point the result will no longer be compatible with older versions of the program. This header structure must be modified in every instance of code which either serialises or parses a packet, which can become time consuming.

// DIAGRAM OF PACKET CONSTRUCTED WITH FIXED VERSUS DYNAMIC HEADER SIZE

A dynamic header, however, is easily extensible. If maintaining compatibility between versions of the program is important, as it might well be when it comes to persistent networks, then a dynamic header might be the better choice. The possibility of incompatibilities opens up a discussion into the value of allowing multiple iterations of the tool to connect to the same network, which will be saved for later.

The most sensible choice for development of a protocol, and a tool which implements said protocol, appears to be that of human readable headers over the alternative. This will greatly simplify debugging, as flaws in packet transmission or handling are immediately obvious, but the decision also ties into the choice between fixed and dynamic headers. Human readable information may differ in length, making fixed headers a limitation rather than an advantage. Instead, information such as data type can be stored as enumerable values, so that they can be parsed from machine readable data to human readable values more easily. Enumerating data also greatly improves efficiency, as assigning a single number to what would otherwise consist of multiple characters reduces the space the data takes up considerably. One disadvantage of this approach is that packet header structure must be documented thoroughly so that tools which capture packets for debugging purposes are not rendered useless, or at the very least more difficult to use.

// DIAGRAM OF PACKET CONSTRUCTED FROM HUMAN-READABLE VERSUS MACHINE-READABLE DATA

One of the main goals of this project is to reduce the cost of server hosting. The main way in which this will be accomplished is by delegating the task of hosting to the users of the network, but that does not mean the program can then be inefficient when it comes to data usage. Not only will reducing the amount of data per packet improve performance, but considering the user's situation is also important. Users might be subject to data caps or limits and so reducing the burden on them as much as possible is also a priority. Because of this, a fixed header with non-human readable data will be used, as this offers the greatest efficiency and speed when it comes to transmitting and parsing packets.

### network event breakdown

// list of network events and ids  
// description of each  
// 

### basic network imaging

The complexity of a mesh network is largely due to the need for all data to be synced between all peers that are connected, including data that was sent prior to a peer's connection. When a new device connects to the network it must somehow receive all of the game state changes so far so that it can catch up. The same method can be used to keep all members of the network up to date on the network structure itself. A range of methods for storing and transmitting this data present themselves, each with pros and cons.

// serialisation & parsing  
// 

### connection handshake

In order to create a fully decentralised mesh network all members of the network must be treated the same. Each node in the network will have to be as self-sufficient as possible. There is no one device that allocates identification numbers or target nodes to new connections. There is no one device that handles user input from each peer. There is no one source of information on the network structure. Instead, these tasks will have to be accomplished collaboratively, using a range of handshaking techniques designed to communicate information in the correct order.

One consideration to make is to decide which device is the initiator when a new client joins the network. Either the host or the client could opt to inform the network, but only one can correctly assign a new, unused user identification number due to the availability of the information on which are already in use. This means that the host has to make the first move: to inform the new client of the current network structure.

- Client attempts connection to the network
- Host accepts new connection
- Host serialises and sends the network image directly to the new client
- Client receives and parses this network image, making a note of the socket it was received from
- Client 

// packet order in back and forth upon new connection  
// initiator  
// 

### addressing peers

// local sockets to unique user identification numbers  
// 

### ip address grepping

// new connection informing network of used target address  
// range of methods for getting an ip address  
// 

### reconnect events

// picking appropriate targets  
// managing skipped packets  
// options for handling children  
// 

### load balancing

// picking appropriate targets  
// similarities to handling reconnects  
// advantages over losing and regaining connection  
// 

## testing & evaluation

### data throughput

// difference in data throughput compared to a normal network structure  
// possible techniques for improving efficiency provided by new network structure  
// 

### socket load

// difference in number of connections handled compared to a normal network structure  
// 

### computational load

// discussion of increased computational load trade-off  
// 

### debugging process

// use of netcat & other tools  
// 

### example scenario

// evaluation of health of test setup  
// 

### discussion of reliability

// robustness of network image and autonomous reconnect handling  
// difficulties arising from interaction between LAN & WAN networks  
// estimated load and connection capacities  
// outlier scenarios, such as root node disconnecting  
// discussion of handling simultaneous network disconnection events  
// 

### alternative potential use cases

// other uses for mesh network architecture  
// 

## conclusion

### evaluation of progress

// 

### target versus result

// 

### were I given a redo

// 

### viability of idea

// 

## references

//

## appendices

//
