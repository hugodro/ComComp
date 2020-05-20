#ifndef _POSTGRES_REPLYVALUE_H_
#define _POSTGRES_REPLYVALUE_H_
/**************************************************
* File: pgReplyValue.h.
* Desc: Definition of the PostgresReplyValue classes.
* Module: AkraLog : SqlSupport.
* Rev: 25 mai 1998 : REV 0 : Hugo DesRosiers.
**************************************************/

#include "replyValue.h"


class PostgresReplyValue : public SqlReplyValue {
    friend class PostgresPipe;
  protected:
    void *data;

  public:
    PostgresReplyValue(void);
    virtual ~PostgresReplyValue(void);
    virtual void load(int aType, int aSize, int aLength, char *aValue);
    virtual unsigned int getAsInteger(void);
    virtual char *getAsString(void);
};


#endif		/* _POSTGRES_REPLYVALUE_H_ */

