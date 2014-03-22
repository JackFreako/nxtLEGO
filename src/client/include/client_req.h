#ifndef __CLIENT_REQ_H__
#define __CLIENT_REQ_H__
#include <stdint.h>

#include "../../common/bt_packet.h"


extern uint32_t timestamp;
void bt_decode_incoming(bt_packet_t *incoming, bt_packet_t *outgoing);
#endif