enum sock_type {
	STYPE_UNKNOWN,
	STYPE_DGRAM,
	STYPE_STREAM,
};

enum sock_domain {
	SDOM_UNKNOWN,
	SDOM_INET,
};

enum sock_proto {
	SPROT_UNKNOWN,
};

struct socket {
	int sock;
}


struct socket
socket(
