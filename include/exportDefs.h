#ifndef _XTN_EXPORTDEFS_H_
#define _XTN_EXPORTDEFS_H_
/*******************************************
 * File: exportDefs.h.
 * Desc: Definitions of export system for putting classes into libraries.
 * History: 2000/07/27 : REV 0 : [HD] : Creation.
******************************************/

#if defined(_WIN32)
#if defined(DLL)
#define ClExp __declspec(dllexport)
#else
#define ClExp __declspec(dllimport)
#endif		/* DLL */
#else		/* _WIN32 */
#define ClExp
#endif		/* !_WIN32 */


#endif		/* _XTN_EXPORTDEFS_H_ */
