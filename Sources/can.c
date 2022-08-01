/*
 * can.c
 *
 *  Created on: 09-Jun-2022
 *      Author: bhaar
 */

#include "can.h"





can_message_t  can_rx_tx_msg_st;

U8 last_can_command_ack_pending = 0;
volatile U8 can_rx_msg_received_vu8 = 0;

void CAN_eventHandler(U8 instance, flexcan_event_type_t eventType,
		 U32 buffIdx, flexcan_state_t *flexcanState);


can_message_t message_can;
/*#2 Configure the Tx buffer using MailBox 0 & 1*/
can_buff_config_t can_tx_buff_config;
can_buff_config_t can_tx_buff_config1;
void can_init(void)
{
	uint8_t can_bank_counter_u8 = 0;
	CAN_Init(&can_pal1_instance, &can_pal1_Config0);


		CAN_InstallEventCallback(&can_pal1_instance,
		    		CAN_eventHandler,
		                                        NULL);

		can_tx_buff_config.enableBRS = false;
		can_tx_buff_config.enableFD = false;
		can_tx_buff_config.fdPadding = false;
		can_tx_buff_config.idType = CAN_MSG_ID_STD;
		can_tx_buff_config.isRemote = false;

		CAN_ConfigTxBuff(&can_pal1_instance,
		CAN_TX_MAILBOX0, &can_tx_buff_config);


			can_tx_buff_config1.enableBRS = false;
			can_tx_buff_config1.enableFD = false;
			can_tx_buff_config1.fdPadding = false;
			can_tx_buff_config1.idType = CAN_MSG_ID_STD;
			can_tx_buff_config1.isRemote = false;

			CAN_ConfigTxBuff(&can_pal1_instance,
			CAN_TX_MAILBOX1, &can_tx_buff_config1);

			/*#3 Configure the Rx buffer using MailBox 2 */

			can_buff_config_t can_rx_buff_config0;
			can_rx_buff_config0.enableBRS = false;
			can_rx_buff_config0.enableFD = false;
			can_rx_buff_config0.fdPadding = false;
			can_rx_buff_config0.idType = CAN_MSG_ID_STD;
			can_rx_buff_config0.isRemote = false;

			//CAN_ConfigRxBuff(&can_pal1_instance, CAN_RX_MAILBOX0, &can_rx_buff_config0,
					//0);
			//CAN_SetRxFilter(&can_pal1_instance, CAN_MSG_ID_STD, CAN_RX_MAILBOX0, 0);

			can_buff_config_t can_rx_buff_config1;
			can_rx_buff_config1.enableBRS = false;
			can_rx_buff_config1.enableFD = false;
			can_rx_buff_config1.fdPadding = false;
			can_rx_buff_config1.idType = CAN_MSG_ID_STD;
			can_rx_buff_config1.isRemote = false;

			//CAN_ConfigRxBuff(&can_pal1_instance, CAN_RX_MAILBOX1, &can_rx_buff_config1,0);
			//CAN_SetRxFilter(&can_pal1_instance, CAN_MSG_ID_STD, CAN_RX_MAILBOX1, 0);

			can_buff_config_t can_rx_buff_config2;
			can_rx_buff_config2.enableBRS = false;
			can_rx_buff_config2.enableFD = false;
			can_rx_buff_config2.fdPadding = false;
			can_rx_buff_config2.idType = CAN_MSG_ID_STD;
			can_rx_buff_config2.isRemote = false;

			for(can_bank_counter_u8 = 2;can_bank_counter_u8<12;can_bank_counter_u8++)
			{
				CAN_ConfigRxBuff(&can_pal1_instance, can_bank_counter_u8, &can_rx_buff_config1,0);
				CAN_SetRxFilter(&can_pal1_instance, CAN_MSG_ID_STD, CAN_RX_MAILBOX1, 0);
				CAN_Receive(&can_pal1_instance, can_bank_counter_u8, &message_can);
			}
			can_queue_init();

}

void CAN_eventHandler(uint8_t instance, flexcan_event_type_t eventType,
		 uint32_t buffIdx, flexcan_state_t *flexcanState)
 {
	/* Unused warning removal */
	(void) (instance);
	/* Unused warning removal */
	(void)(flexcanState);

	switch (eventType)
	{
/**********************************************************************************************************************/
		case FLEXCAN_EVENT_RX_COMPLETE:
		{
			can_rx_array_queue_put(message_can);
			CAN_Receive(&can_pal1_instance, buffIdx, &message_can);
		}
		break;
/**********************************************************************************************************************/
		case FLEXCAN_EVENT_RXFIFO_COMPLETE:
		{

		}
		break;
/**********************************************************************************************************************/
		case FLEXCAN_EVENT_WAKEUP_MATCH:
		{
			flexcan_msgbuff_t flexcan_msgbuff_st;
			FLEXCAN_DRV_GetWMB(instance, 0, &flexcan_msgbuff_st);
		}
		break;
/**********************************************************************************************************************/
		case FLEXCAN_EVENT_WAKEUP_TIMEOUT:
		{

		}
		break;
/**********************************************************************************************************************/
		case FLEXCAN_EVENT_SELF_WAKEUP:
		{

		}
		break;
/**********************************************************************************************************************/
		case FLEXCAN_EVENT_DMA_COMPLETE:
		{

		}
		break;
/**********************************************************************************************************************/
		case FLEXCAN_EVENT_TX_COMPLETE:
		{

		}
		break;
/**********************************************************************************************************************/
		default:
		{

		}
		break;
/**********************************************************************************************************************/
	}
 }

