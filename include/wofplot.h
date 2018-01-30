/* -*- mode: c; tab-width: 4 -*- */
/*
 * INTERNAL USE ONLY
 *
 * Copyright (c) 2015-2016 Synaptics Incorporated.  All rights reserved.
 *
 * This file contains information that is proprietary to Synaptics
 * Incorporated ("Synaptics"). The holder of this file shall treat all
 * information contained herein as confidential, shall use the
 * information only for its intended purpose, and shall not duplicate,
 * disclose, or disseminate any of this information in any manner unless
 * Synaptics has otherwise provided express, written permission.
 */

/*
 * Implement the VCSFW_CMD_WOF_BAELINE VCSFW_CMD_WOF_SIGNAL command
 *
 */

#ifndef __WOFPLOT_H
#define __WOFPLOT_H


#define VCSFW_CMD_WOF_BASELINE 254
#define VCSFW_CMD_WOF_SIGNAL   255

/* set how many gain will scan */
#define WOF_GAIN_MAX_IDX 5 

#define  WOF_BASELINE   80


/* command structure for VCSFW_CMD_WOFBAELINE */
typedef struct vcsfw_cmd_wofbaseline_s
{
    uint16_t  gain_value;

}vcsfw_cmd_wofbaseline_t;


/* replay structure for VCSFW_CMD_WOFBAELINE */
typedef struct vcsfw_reply_wofbaseline_s
{
    uint16_t    gain_value;
    uint16_t    offsets;
}vcsfw_reply_wofbaseline_t;


/* command structure for VCSFW_CMD_WOFSIGNAL */
typedef struct vcsfw_cmd_wofsignal_s
{
    uint16_t    gain_value;
    uint16_t    offsets;
}vcsfw_cmd_wofsignal_t;

/* reply structure for VCSFW_CMD_WOFSIGNAL */
typedef struct vcsfw_reply_wofbsignal_s
{
    uint16_t    gain_value;
    uint16_t    offsets;
	uint16_t    signal;
}vcsfw_reply_wofsignal_t;

#endif /* !__WOFPLOT_H */
