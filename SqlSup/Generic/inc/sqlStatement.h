#ifndef _SQL_STATEMENT_
#define _SQL_STATEMENT_
/**************************************************
* File: sqlStatement.h.
* Desc: Definition of the SqlStatement.
* Module: AkraLog : SqlSupport.
* Rev: 25 mai 1998 : REV 0 : Hugo DesRosiers : Creation.
**************************************************/

#include <akra/AkObject.h>
#include <akra/portableDefs.h>

class SqlExecFrame;


class ClassExport SqlStatement : public AkObject {
  protected:

  public:
    virtual bool parseTokens(SqlExecFrame *frame, unsigned char *tokens, unsigned int tokensLength, unsigned int &tokenRead)= 0;
    virtual int getMainAction(void)= 0;
};



#endif		/* _SQL_STATEMENT_ */
