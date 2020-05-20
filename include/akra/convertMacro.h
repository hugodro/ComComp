#ifndef _CONVERTMACRO_H_
#define _CONVERTMACRO_H_
/**************************************************
* File: convertMacro.h.
* Desc: Macro for type conversions between 'disk' data and memory data.
* Module: AkraLog : AkraKit.
* Rev: 29 avril 1996 : REV 0 : Hugo DesRosiers : Creation.
**************************************************/

#define MakeUByte(x)		\
		((unsigned char)((x)[0]))

#if defined(__linux__)
#define MakeUShort(x)		\
		(((unsigned short)(unsigned char)(x)[1]) | (((unsigned short)(unsigned char)(x)[0]) << 8))


#define MakeUInteger(x)	\
			(((unsigned int)(unsigned char)(x)[3]) \
			| (((unsigned int)(unsigned char)(x)[2]) << 8) \
			| (((unsigned int)(unsigned char)(x)[1]) << 16) \
			| (((unsigned int)(unsigned char)(x)[0]) << 24))

#elif defined(__POWERPC__)
#define MakeUShort(x)		\
		(((unsigned short)(unsigned char)(x)[1]) | (((unsigned short)(unsigned char)(x)[0]) << 8))


#define MakeUInteger(x)	\
			(((unsigned int)(unsigned char)(x)[3]) \
			| (((unsigned int)(unsigned char)(x)[2]) << 8) \
			| (((unsigned int)(unsigned char)(x)[1]) << 16) \
			| (((unsigned int)(unsigned char)(x)[0]) << 24))
#else
#define MakeUShort(x)		\
		((unsigned short)((unsigned short)((x)[1]) + ((unsigned short)((x)[0]) << 8)))

#define MakeUInteger(x)	\
		((unsigned int)	 \
			(unsigned int)((x)[3]) + ((unsigned int)((x)[2]) << 8) + \
			((unsigned int)((x)[1]) << 16) + ((unsigned int)((x)[0]) << 24))
#endif


#endif	/* _CONVERTMACRO_H_ */
