#ifndef __SHAPE_PACKET_H__
#define __SHAPE_PACKET_H__

/* just stub now */
#define DATA_SZ 32

#pragma pack(push,1)
typedef struct {
	struct ether_header ether;
	struct iphdr ip;
	char data[DATA_SZ];
} my_packet_t;
#pragma pack(pop)

void shape_packet(my_packet_t *packet, int max_len);
void set_payload(my_packet_t *packet, int max_len);

#endif
