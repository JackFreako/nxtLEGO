#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "include/client_lego.h"
#include "include/client_req.h"
#include "../common/include/bt_packet.h"

#define unlikely(x)     __builtin_expect((x),0)

/*************GLOBAL VARIABLES**********/
uint32_t timestamp;
uint32_t bt_conn_status;
bt_packet_t incoming_packet[1];
bt_packet_t outgoing_packet[1];

#define DEVICE_PWD "1234"

/******OSEK Declarations******/

DeclareTask(BtComm);
DeclareTask(DisplayTask);
DeclareResource(RES_LCD);
DeclareAlarm(BT_COMM_ALARM);
DeclareAlarm(LCD_UPDATE_ALARM);
DeclareCounter(SysTimerCnt);


static void reset_data_structs()
{
    memset(incoming_packet, 0, sizeof(bt_packet_t));
    memset(outgoing_packet, 0, sizeof(bt_packet_t));
}

static void reset_motor_power()
{
    nxt_motor_set_speed(NXT_PORT_A, 0, 1);
    nxt_motor_set_speed(NXT_PORT_B, 0, 1);
    nxt_motor_set_speed(NXT_PORT_C, 0, 1);
}


/****Initialize DEVICE*****/
void ecrobot_device_initialize()
{
    timestamp = 0;
    bt_conn_status = 0;
    reset_data_structs();
    reset_motor_power();
    ecrobot_init_bt_slave(DEVICE_PWD);
}


void ecrobot_device_terminate()
{
    reset_motor_power();
    reset_data_structs();
    bt_reset();
    ecrobot_term_bt_connection();
}


void user_1ms_isr_type2(void)
{
    StatusType ercd;
    /*check value of timestamp to prevent an overflow
     * if maximum value is reached reset it to 0*/
    if(unlikely (timestamp == (uintmax_t) UINT32_MAX))
        timestamp = 0;
    else
        ++timestamp;

    ercd = SignalCounter(SysTimerCnt);
    if(ercd != E_OK)
        ShutdownOS(ercd);
}



TASK(BtComm)
{

    bt_conn_status = ecrobot_read_bt_packet((U8*)incoming_packet, sizeof(bt_packet_t));

    if( bt_conn_status > 0)
    {
        bt_decode_incoming(incoming_packet, outgoing_packet);
        ecrobot_send_bt_packet((void *)outgoing_packet, (U32)sizeof(bt_packet_t));
    }
    TerminateTask();
}


TASK(DisplayTask)
{
    ecrobot_status_monitor("nxtLEGO client");
    TerminateTask();
}
