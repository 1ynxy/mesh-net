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

## abstract

One of the primary costs associated with developing large scale online video games is that of continued hosting of non-community run game servers. Often, these are the only type of server available, as it allows the company more authority over how the servers are interacted with by the players.

This project aims to develop and test a method for enabling the distributed processing and hosting of virtual game worlds in which all clients act as equal nodes in the network. An attempt will also be made to introduce data persistence and load balancing to the resulting mesh network. The result will be a module written in C++, integrated loosely with a simple, custom, 3D capable game engine. This module will abstract all network events and methods in a way that can be utilised by any program, rather than being tied to a particular game engine.

## contents

- abstract
- contents
- introduction
- literature review
- methodology
- conclusion
- references 

## introduction

// put introduction here

## literature review

// put literature review here

## methodology

representing network

tree: 
- each peer is a node
- a node has one host & many children
- most accurate view of network relationships
- innefficient to traverse : depth first techniques:
  - INORDER
  - PREORDER
  - POSTORDER
- every operation requires a traversal
list: 
- simple list of nodes
- simpler algorithmically
- no indication of relationships : disconnects will not be accurate
- much easier to traverse
both: 
- easy to traverse
- accurate view of network relationships
- modification of peers requires multiple operations
- list can be used for traversal, then acted upon as a tree node

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

## conclusion

// put conclusion here

## references

// put references here
