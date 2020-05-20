#ifndef _MYSQL_REPLYVALUE_H_
#define _MYSQL_REPLYVALUE_H_
/**************************************************
* File: mysqlReplyValue.h.
* Desc: Definition of the MySqlReplyValue classes.
* Module: AkraLog : SqlSupport.
* Rev: 19 juillet 1998 : REV 0 : Hugo DesRosiers.
**************************************************/

#include <sqlsup/replyValue.h>


class MySqlReplyValue : public SqlReplyValue {
    friend class MySqlPipe;
  protected:
    void *data;

  public:
    MySqlReplyValue(void);
    virtual ~MySqlReplyValue(void);
    virtual void load(int aType, int aSize, int aLength, char *aValue);
    virtual unsigned int getAsInteger(void);
    virtual char *getAsString(void);
};


#endif		/* _MYSQL_REPLYVALUE_H_ */

