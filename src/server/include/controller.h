#ifndef __CONTROLLER_H__
#define __CONTROLLER_H__
#include <stdint.h>
#include "../../common/bt_packet.h"

/*********DEFINES*********/
#define SERVER_PATH		"/tmp/nxtLEGO/"
#define MAX_CONNECTIONS 	1
/************GLOBALS*********/

void init_controller(int *server_sock);
int controller_accept_conn(int *server_sock, int *client_sock);
int controller_process_req(bt_packet_t *in, bt_packet_t *out,int *client_sock, int *bt_sock);
int controller_stop(int server_sock, int client_sock);
#endif