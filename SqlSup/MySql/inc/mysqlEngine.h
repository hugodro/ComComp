#ifndef _MYSQL_ENGINE_H_
#define _MYSQL_ENGINE_H_
/**************************************************
* File: mysqlEngine.h.
* Desc: Definition of the statement to text executor.
* Module: AkraLog : SqlSupport.
* Rev: 14 juillet 1998 : REV 0 : Hugo DesRosiers : Creation.
**************************************************/

#include <sqlsup/textEngine.h>


class MySqlEngine : public TextSqlEngine {
  protected:	// Instance variables.
    char *cursorName;

  public:		// Instance methods.
    MySqlEngine(void);
    MySqlEngine(TextOutputBuffer *outBuffer);
    virtual void registerCursorName(char *aCursorName);
    virtual int execSelectMult(SqlExecFrame *frame, SqlSelectStat *statement, void **suppValues);
    virtual int execSelectSng(SqlExecFrame *frame, SqlSelectStat *statement, void **suppValues);

};


#endif		/* _MYSQL_ENGINE_H_ */
