#ifndef _XTN_GLOBALDEFS_H_
#define _XTN_GLOBALDEFS_H_
/******************************************************************************
 * File: globDefs.h.
 * Desc: Definitions of constants, types and other usefull all-over-the place
 *       things.
 * History: 2000/03/17 : REV 0 : [HD] : Creation.
 * $Log: globDefs.h,v $
 * Revision 1.7  2000/10/12 01:40:52  igor
 * XTN_SID_T and other global types.
 *
 * Revision 1.6  2000/10/06 22:15:23  hugo
 * Increased the length of FILE_NAME to 1024.
 *
 * Revision 1.5  2000/10/06 22:12:10  hugo
 * Fixed the Log CVS entry.
 *
 *****************************************************************************/

/*---------------------------------------------------------------------------*/
/*                               CONSTANTS                                   */
/*---------------------------------------------------------------------------*/

#if !defined(NULL)
#define	NULL	(0L)
#endif

#define XTN_FILENAME_LEN          1024        // File name length
#define XTN_USERNAME_LEN            32        // User name length
/*---------------------------------------------------------------------------*/
/*                                    TYPES                                  */
/*---------------------------------------------------------------------------*/

typedef unsigned char    xtnUchar;
typedef char             XTN_FILENAME_T[XTN_FILENAME_LEN];
typedef char             XTN_USERNAME_T[XTN_USERNAME_LEN];
typedef unsigned long    XTN_SID_T;

/*---------------------------------------------------------------------------*/
/*                                   CLASSES                                 */
/*---------------------------------------------------------------------------*/

/*---------------------------------- ByteUtil -------------------------------*/
class ByteUtil {
 public:
  static inline unsigned short makeUInt16(char *&aStr)
  {
    unsigned short result;

        result= (unsigned short)(xtnUchar)aStr[1] + 
	  (unsigned short)((xtnUchar)aStr[0] << 8);
        aStr+= 2;
        return result;
  }
  static inline unsigned int makeUInt32(char *&aStr)
  {
    unsigned int result;

    result= ((unsigned int)(xtnUchar)aStr[3]) + 
      (((unsigned int)(xtnUchar)aStr[2]) << 8)
      + (((unsigned int)(xtnUchar)aStr[1]) << 16) + 
      (((unsigned int)(xtnUchar)aStr[0]) << 24);
    aStr+= 4;
    return result;
  }

  static inline void writeUInt32(char * &aStr, unsigned int value)
  {
    *aStr++= (char)((value >> 24) & 0x0FF);
    *aStr++= (char)((value >> 16) & 0x0FF);
    *aStr++= (char)((value >> 8) & 0x0FF);
    *aStr++= (char)(value & 0x0FF);
  };

  static inline void writeUInt16(char * &aStr, unsigned short value) 
  {
    *aStr++= (char)((value >> 8) & 0x0FF);
    *aStr++= (char)(value & 0x0FF);
  };

}; // class ByteUtil

#endif		/* _XTN_GLOBALDEFS_H_ */
