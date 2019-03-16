#include "sockets.h"

// Socket //

// Constructors & Destructors

Socket::Socket(int sock) : sock(sock) {
	
}

// Member Functions

Socket* Socket::get_host() {
	return host;
}

void Socket::set_host(int sock) {
	if (host) return;

	host = new Socket(sock);
}

void Socket::clear_host() {
	if (!host) return;

	delete(host);

	host = nullptr;
}

Socket* Socket::get_child(int sock) {
	for (Socket* child : children) if (child->sock == sock) return child;

	return nullptr;
}

void Socket::add_child(int sock) {
	Socket* socket = get_child(sock);

	if (socket) return;

	children.push_back(new Socket(sock));
}

void Socket::remove_child(int sock) {
	Socket* socket = get_child(sock);

	if (!socket) return;

	children.erase(std::find(children.begin(), children.end(), socket));
}

// Mesh //

// Member Functions

bool MeshNet::is_set(int sock) {
	return FD_ISSET(sock, &set);
}

bool MeshNet::is_self(int sock) {
	if (!peers) return false;

	if (peers->sock != sock) return false;
	
	return true;
}

bool MeshNet::is_host(int sock) {
	if (!peers) return false;

	if (!peers->get_host()) return false;

	if (!peers->get_host()->sock != sock) return false;

	return true;
}

bool MeshNet::select(fd_set* sel) {
	*sel = set;

	if (::select(count + 1, sel, NULL, NULL, NULL) == -1) return false;

	return true;
}

void MeshNet::add(int sock) {
	FD_SET(sock, &set);

	if (sock > count) count = sock;
}

void MeshNet::remove(int sock) {
	FD_CLR(sock, &set);

	close(sock);
}
