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
- literature review
- methodology
- testing & evaluation
- conclusion
- references
- appendices

## introduction, aims & objectives

// 

## literature review, theory & design

//

## methodology

In order to create a fully decentralised mesh network all members of the network must be treated the same. Each node in the network will have to be as self-sufficient as possible. There is no one device that allocates identification numbers or target nodes to new connections. There is no one device that handles user input from each peer. There is no one source of information on the network structure. Instead, these tasks will have to be accomplished collaboratively, using a range of handshaking techniques designed to communicate information in the correct order.

The complexity of a mesh network is largely due to the need for all data to be synced between all peers that are connected, including data that was sent prior to a peer's connection. When a new device connects to the network it must somehow receive all of the game state changes so far so that it can catch up. The same method can be used to keep all members of the network up to date on the network structure itself. A range of methods for storing and transmitting this data present themselves, each with pros and cons.

The first, and most intuitive, method for storing this network image during runtime is that of a tree data structure. As each member of the network is a node with a single parent and multiple children this method seems ideal, but it does present complications when attempting to quickly navigate the structure. Although use of pointers and references in the C++ language improves memory management, as, unlike a std::vector data type, when the structure is modified the memory does not necessarily have to be reallocated, the structure itself can be navigated bidirectionally. Unlike with a list, which would only have to be navigated in a single direction, a tree can be traversed in multiple ways, which both complicates algorithms and decreases efficiency of even simple tasks such as searching for a node with a certain identifying feature. Tree traversal methods include depth-first techniques, such as inorder, preorder and postorder.

One counter-argument to the memory management advantages that such a data structure provides is the use of linked lists, which work in a similar fashion, but reduces the number of child nodes to one, eliminating the need for complex traversal algorithms. Unfortunately, using only a list type structure does not allow for an accurate indication of network relationships, resulting in a list of peers without a discernable hierarchy.

A potential compromise comprises of a hybrid structure. A tree can be created, and each element of the tree can be also stored in a list for efficient information grepping. Duplicate data can be eliminated through the use of C++ pointers, although this can introduce problems with dangling pointers if not handled cautiously. A dangling pointer is a pointer which no longer point to a valid object of the appropriate type, most likely because the target object has been deleted. This method does introduce complexities in that a single modification of the network would require multiple operations to be correctly reflected in the data structures.

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

peer structure

integer unique user identification [UUID]  
integer socket number  
string global ip  
string local ip  
enum port status { PORT_FORWARD, PORT_BLOCK, PORT_UNDEF }  
peer* host  
list peer* children  

## testing & evaluation

//

## conclusion

//

## references

//

## appendices

//
