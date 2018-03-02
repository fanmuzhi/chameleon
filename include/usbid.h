/* -*- mode: c; tab-width: 4 -*- */
/* $Header$ */

/*
 *******************************************************************************
 **
 **                           NDA AND NEED-TO-KNOW REQUIRED
 **
 *****************************************************************************
 **
 **  Copyright (c) 2005-2018 Synaptics Incorporated. All rights reserved.
 **
 **
 ** This file contains information that is proprietary to Synaptics
 ** Incorporated ("Synaptics"). The holder of this file shall treat all
 ** information contained herein as confidential, shall use the
 ** information only for its intended purpose, and shall not duplicate,
 ** disclose, or disseminate any of this information in any manner unless
 ** Synaptics has otherwise provided express, written permission.
 ** Use of the materials may require a license of intellectual property
 ** from a third party or from Synaptics. Receipt or possession of this
 ** file conveys no express or implied licenses to any intellectual
 ** property rights belonging to Synaptics.
 **
 **
 ** INFORMATION CONTAINED IN THIS DOCUMENT IS PROVIDED "AS-IS," AND
 ** SYNAPTICS EXPRESSLY DISCLAIMS ALL EXPRESS AND IMPLIED WARRANTIES,
 ** INCLUDING ANY IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
 ** PARTICULAR PURPOSE, AND ANY WARRANTIES OF NON-INFRINGEMENT OF ANY
 ** INTELLECTUAL PROPERTY RIGHTS. IN NO EVENT SHALL SYNAPTICS BE LIABLE
 ** FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, PUNITIVE, OR
 ** CONSEQUENTIAL DAMAGES ARISING OUT OF OR IN CONNECTION WITH THE USE OF
 ** THE INFORMATION CONTAINED IN THIS DOCUMENT, HOWEVER CAUSED AND BASED
 ** ON ANY THEORY OF LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 ** NEGLIGENCE OR OTHER TORTIOUS ACTION, AND EVEN IF SYNAPTICS WAS ADVISED
 ** OF THE POSSIBILITY OF SUCH DAMAGE. IF A TRIBUNAL OF COMPETENT
 ** JURISDICTION DOES NOT PERMIT THE DISCLAIMER OF DIRECT DAMAGES OR ANY
 ** OTHER DAMAGES, SYNAPTICS' TOTAL CUMULATIVE LIABILITY TO ANY PARTY
 ** SHALL NOT EXCEED ONE HUNDRED U.S. DOLLARS.
 */

/*
 * Note that as of September 1, 2015, this file is controlled
 *  authoritatively in the git repository
 *    ssh://gitms@git.synaptics.com/git/biometrics/include.git.
 * Updates will continue to be copied into the CVSNT repository
 *  in /test/engineering/private/impl/micro/falcon/shared/usbid.h.
 * The last authoritative version of this file in CVSNT was
 *  /test/engineering/private/impl/micro/falcon/shared/usbid.h,v 1.34 2015/03/20 23:48:20 synaptics-inc\bchen
 * DO NOT EDIT THIS FILE IN THE CVSNT REPOSITORY.  Your changes will
 *  be overwritten.
 */

#ifndef VCSFW_USBID_H
#define VCSFW_USBID_H

/* -------------------------------------------------------------------------- */
/* -------------------------------------------------------------------------- */
/* Validity Vendor ID (0x138a) */
/* -------------------------------------------------------------------------- */
/* -------------------------------------------------------------------------- */

/* This file defines the validity vendor id as well as the various product   */
/* ids we use                                                                */

/* Standard Vendor ID (VID)                                                  */
#define USB_VALIDITY_VENDOR 0x138a

/* Known Product IDs (PID)                                                   */
#define USB_VFS101          0x0001  /* Includes vfs101,vfs131,vfs161,vfs201  */
#define USB_DP02A           0x0002  /* Obsolete, never shipped               */
#define USB_DP02B           0x0003  /* Obsolete, never shipped               */
#define USB_VFS111          0x0004  /* Obsolete, never shipped               */

#define USB_VFS301          0x0005  /* Falcon-based, no security             */
#define USB_VFS351          0x0006  /* Falcon-based, AES Security            */
#define USB_VFS451          0x0007  /* Falcon-based, AES with SecureMatch    */
#define USB_VFS300          0x0008  /* Falcon-based, downgraded 301          */
#define USB_VFS421          0x0009  /* Falcon-based, 4.21 FW, no security    */

/* NOTE: 4.30 Raptor firmware internally supports only PIDs 0x0010-0x001f    */
#define USB_VFS501          0x0010  /* Raptor-based, no security; "Bermuda", */
                                    /* Was originally "USB_RAPTOR"           */
#define USB_RAPTOR          USB_VFS501  /* useful alias                      */
#define USB_EASTER          0x0011  /* Raptor-based, 160 pixel;    "Easter"  */
#define USB_SAMOA           0x0012  /* Raptor-based, BGA           "Samoa"   */
#define USB_RAPTOR_STANDARD 0x0013  /* Raptor-based, Standard part for small */
                                    /* OEMs.  Expected use is WBF            */
#define USB_RAPTOR_LENOVO_CLASSIC 0x0014    /* EASTER part for Lenovo Classic, 2013 */
#define USB_RAPTOR_LENOVO_KEYPAD 0x0015     /* EASTER part for Lenovo Keyboard, 2013 */
                                    /* 0x0016-0x0017 currently unassigned    */
#define USB_SECURE_RAPTOR   0x0018  /* Raptor-based, SecureMatch enabled     */
                                    /* 19-1f (SM enabled) unassigned         */

/* Note: Raptor SPI firmware internally supports only PIDs 0x0020-0x002f     */
#define SPI_BERMUDA         0x0020  /* Raptor-based, SPI, no security        */
#define SPI_BUTTON_HTC      0x0022  /* Raptor-based, SPI button for HTC      */
#define SPI_RAPTOR2         0x0023  /* Raptor2-based, SPI, no security       */
#define SPI_OTHER           0x0024  /* Raptor-based, for non-Samsung/HTC customers */

/* 0x0030 to 0x003f reserved for predefined V4.40 firmware parts.            */
#define USB_KEYWEST         0x0030  /* Falcon Based, no security             */
#define USB_VFS471          0x003c  /* Falcon-based, RSA-2048                */
/* Sensors with V4.45 firmware.                                              */
#define USB_CUDA            0x0031  /* Falcon Based, no security             */
/* Sensors with V4.60 firmware                                               */
#define USB_VFS491          0x003d  /* Falcon-based, RSA-2048, FP buffering  */
#define USB_VFS495_DELL     0x003e  /* Same sensor as VFS495                 */
#define USB_VFS495          0x003f  /* Same sensor as VFS491, used for HP    */

/* 0x0050 to 0x006f reserved for Eagle-derived products                      */
#define USB_FIJI            0x0050  /* Eagle-based initial device            */
#define USB_EAGLE_STANDARD  0x0051  /* Eagle standard part for small OEMs    */
#define USB_EAGLE_DELL      0x0052  /* Eagle standard part for Dell Consumer */
#define USB_EAGLE_HP        0x0053  /* Eagle standard part for HP Consumer   */

/* 0x0070 to 0x008f reserved for Windsor-derived products                    */
#define USB_WINDSOR         0x0070  /* Windsor-based initial device          */

/* 0x0090 to 0x00af reserved for Metallica-derived products                  */
#define USB_METALLICA       0x0090  /* Metallica-based initial device        */

/* 0x00b0 to 0x00cf reserved for Viper-derived products                      */
#define USB_VIPER           0x00b0  /* Viper-based initial device            */

/* 0x00d0 to 0x00df reserved for Raptor2 based, FW 4.53 sensors              */
#define SPI_RAPTOR_453      0x00d0

/* PIDs of the form 0xFXXX are experimental devices */
#define USB_EXPERIMENTAL    0xf000  /* Base value.  Range=0xf000 - 0xffff    */
#define USB_EXPERIMENTAL_MASK  0xf000   /* Only look at the high 4 bits      */

#define USB_OSPREY_TA       0xf000  /* Osprey the first ASIC revision        */
#define USB_OSPREY_TB       0xf001  /* Osprey the second ASIC revision       */

#define SPI_OSPREY         0x002f /* Viswanadh : Add The windsor SPI device ID to the usb ids */
/* Special PID used for internal manufacturing tests                         */
#define USB_RELIABILITY_TEST  0xfff0  /* Reliability Test Unit PID           */


/* -------------------------------------------------------------------------- */
/* -------------------------------------------------------------------------- */
/* Synaptics Vendor ID (0x06cb) */
/* -------------------------------------------------------------------------- */
/* -------------------------------------------------------------------------- */

#define USBID_SYNA_VID      0x06cb

/*
 * Viper2: 0x40-0x4f.  Email from Dmitriy Kiryashov <dkiryashov@synaptics.com>
 *  Subject: "RE: New USB VID and block of PIDs for BPD's Viper2",
 *  Fri 8/1/2014 8:56 PM EDT.
 *
 * NOTE THAT ONLY 0x0040 WAS EVER USED, so we've reclaimed this range
 *  for newer BPD products.
 */
#define USBID_SYNA_VIPER2       0x0040
/* Hayes (b1212): */
#define USBID_SYNA_HAYES        0x0041
/* Viper3 (b1218): */
#define USBID_SYNA_VIPER3       0x0042
/* Prometheus (b1422): */
#define USBID_SYNA_PROMETHEUS   0x00a9


/*
 * ST Micro USB to SPI bridge: 0x0050 - 0x005f.
 * Email from Dmitriy Kiryashov <Dmitriy.Kiryashov@synaptics.com>
 *  to Bo Chen <bo.chen@synaptics.com>
 *  Subject: "RE: USB descriptor for STMicro", Fri 3/20/2015 2:32 PM EDT.
 */
#define USBID_SYNA_STMICRO      0x0050


#endif  /* VCSFW_USBID_H    */
