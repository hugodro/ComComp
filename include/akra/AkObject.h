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


#ifndef _AKOBJECT_H_
#define _AKOBJECT_H_
/**************************************************
* File: AkObject.h.
* Desc: Definition of the base object class for AkraKits.
* Module: AkraLog : AkraKit.
* Rev: 30 avril 1996 : REV 0 : Hugo DesRosiers : Creation.
**************************************************/

#define true 1
#define false 0


#if defined(_WIN32)
#if defined(DLL)
#define ClassExport __declspec(dllimport)
#else
#define ClassExport __declspec(dllexport)
#endif		/* DLL. */
#else
#define ClassExport
#endif		/* _WIN32. */

class ClassExport AkObject {
    int hashID;
 };


#endif	/* _AKOBJECT_H_ */
