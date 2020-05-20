#ifndef _TEXTHELPER_H_
#define _TEXTHELPER_H_
/**************************************************
* File: textHelper.h.
* Desc: Some text manipulation routines to help parsing.
* Module: AkraLog : SqlSupport.
* Rev: 25 mai 1998 : REV 0 : Hugo DesRosiers : Creation.
**************************************************/

#define ReadUInt(a)	\
	    ((((unsigned int)*a) << 24) | (((unsigned int)*(a+1)) << 16) \
	    | (((unsigned int)*(a+2)) << 8) | (unsigned int)*(a+3))

#define ReadUShort(a)	\
	    ((((unsigned int)*a) << 8) | (unsigned int)*(a+1))

#define ReadUByte(a)	\
	    (((unsigned int)*a) & 0x0FF)

#endif		/* _TEXTHELPER_H_ */
