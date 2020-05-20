#ifndef _REPLYVALUE_H_
#define _REPLYVALUE_H_
/**************************************************
* File: replyValue.h.
* Desc: Definition of the SqlReplyValue classes.
* Module: AkraLog : SqlSupport.
* Rev: 31 mai 1998 : REV 0 : Hugo DesRosiers.
**************************************************/

#include <akra/AkObject.h>
#include <akra/portableDefs.h>


class ClassExport SqlReplyValue : public AkObject {
  public:
    enum Type {
	tChar, tInt, tReal, tVarchar, tDate
	, unknown
    };

  protected:
    Type type;
    unsigned int dataLength;

  public:
    SqlReplyValue(void);
    virtual ~SqlReplyValue(void);
    virtual void load(int aType, int aSize, int aLength, char *aValue);
    virtual Type getType(void);
    virtual unsigned int getAsInteger(void);
    virtual char *getAsString(void);
};


#endif		/* _REPLYVALUE_H_ */

