#ifndef _POSTGRES_ENGINE_H_
#define _POSTGRES_ENGINE_H_
/**************************************************
* File: postgresEngine.h.
* Desc: Definition of the statement to text executor.
* Module: AkraLog : SqlSupport.
* Rev: 28 mai 1998 : REV 0 : Hugo DesRosiers : Creation.
**************************************************/

#include "textEngine.h"


class PostgresEngine : public TextSqlEngine {
  protected:	// Instance variables.
    char *cursorName;

  public:		// Instance methods.
    PostgresEngine(void);
    PostgresEngine(TextOutputBuffer *outBuffer);
    virtual void registerCursorName(char *aCursorName);
    virtual int execSelectMult(SqlExecFrame *frame, SqlSelectStat *statement, void **suppValues);
    virtual int execSelectSng(SqlExecFrame *frame, SqlSelectStat *statement, void **suppValues);

};


#endif		/* _POSTGRES_ENGINE_H_ */
