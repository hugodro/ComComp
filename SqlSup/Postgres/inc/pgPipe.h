#ifndef _POSTGRES_DATAPIPE_H_
#define _POSTGRES_DATAPIPE_H_
/**************************************************
* File: pgPipe.h.
* Desc: Definition of the PostgresPipe class.
* Module: AkraLog : SqlSupport.
* Rev: 26 mai 1998 : REV 0 : Hugo DesRosiers : Creation.
**************************************************/

#include "sqlDataPipe.h"

class TextSqlEngine;
class PostgresTerminal;
class TextOutputBuffer;
class PostgresEngine;

class PostgresPipe : public SqlDataPipe {
  protected:
    boolean hasTransaction;
    char cursorName[9];
    PostgresEngine *engine;
    TextOutputBuffer *outputBuffer;
    PostgresTerminal *terminal;

  public:
    PostgresPipe(JVInstance *anInstance);
    virtual ~PostgresPipe(void);
    virtual int execute(SqlExecFrame *aFrame, void **values, unsigned int nbrValues);
    virtual void installOnTerminal(PostgresTerminal *aTerminal);
    virtual SqlReplyValue **makeReplyArray(unsigned int aSize);
};


#endif		/* _POSTGRES_DATAPIPE_H_ */
