/*
 *  SEOS ChanMUX Ethernet NIC Channel interface
 *
 *  Copyright (C) 2019, Hensoldt Cyber GmbH
 */

#pragma once

typedef enum
{
    CHANMUX_NIC_CMD_OPEN       =  0,   /*!< Open Channel */
    CHANMUX_NIC_RSP_OPEN       =  1,   /*!< Open Confirmation */
    CHANMUX_NIC_CMD_CLOSE      =  2,   /*!< Close Channel */
    CHANMUX_NIC_RSP_CLOSE_     =  3,   /*!< Close Confirmation */
    CHANMUX_NIC_CMD_GET_MAC    =  4,   /*!< GetMac */
    CHANMUX_NIC_RSP_GET_MAC    =  5,   /*!< GetMac Confirmation */
    CHANMUX_NIC_CMD_START_READ =  6,   /*!< Start reading */
    CHANMUX_NIC_RSP_START_READ =  7,   /*!< Start reading confirmation*/
    CHANMUX_NIC_CMD_STOP_READ  =  8,   /*!< Stop reading */
    CHANMUX_NIC_RSP_STOP_READ  =  9,   /*!< Stop reading confirmation*/
} seos_chanmux_nic_commands_t;
