/*
 * Copyright (c) 1996-1997, Akra Logique s.r.o.
 * All Rights Reserved.
 *
 * The C++ source code is the confidential and proprietary information
 * of Akra Logique s.r.o. (refered to as "Proprietary Information" in the Standard
 * Non-Disclausure Agreement).  You shall not disclose such Proprietary Information
 * and shall use it only in accordance with the terms of the agreement you entered into
 * with Akra Logique.
*
 * AKRA LOGIQUE MAKES NO REPRESENTATIONS OR WARRANTIES ABOUT
 * THE SUITABILITY OF THE SOFTWARE, EITHER EXPRESS OR IMPLIED, INCLUDING
 * BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR
 * A PARTICULAR PURPOSE, OR NON-INFRINGEMENT. AKRA LOGIQUE SHALL NOT BE
 * LIABLE FOR ANY DAMAGES SUFFERED BY USER AS A RESULT OF USING,
 * MODIFYING OR DISTRIBUTING THIS SOFTWARE OR ITS DERIVATIVES.
 */

#ifndef _PORTABLEDEFS_H_
#define _PORTABLEDEFS_H_
/**************************************************
* File: portableDefs.h.
* Desc: Definitions to make things portable.
* Module: AkraLog : AkraKit.
* Rev: 2 mars 1997 : REV 0 : Hugo DesRosiers : Creation.
**************************************************/

#ifndef NULL
#define NULL		(0L)
#endif

#if !defined(__GNUC__) && !defined(MSDOS) && !defined(_WIN32)
typedef unsigned char bool;
#endif

#ifndef true
#define true	1
#endif

#ifndef false
#define false	0
#endif

typedef unsigned char ubyte;
typedef unsigned short ushort;
typedef unsigned int uint;

#endif	/* _PORTABLEDEFS_H_ */
