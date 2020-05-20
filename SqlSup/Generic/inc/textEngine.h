#ifndef _TEXTSQLENGINE_H_
#define _TEXTSQLENGINE_H_
/**************************************************
* File: textEngine.h.
* Desc: Definition of the statement to text executor.
* Module: AkraLog : SqlSupport.
* Rev: 27 mai 1998 : REV 0 : Hugo DesRosiers : Creation.
**************************************************/

#include "sqlEngine.h"

class TextOutputBuffer;
class SqlSelectStat;
class SqlInsertStat;
class SqlDeleteStat;
class SqlUpdateStat;
class SqlQueryExpr;
class SqlSelectItem;
class SqlTableRef;
class SqlExpression;
class SqlOperExpr;
class SqlConstantExpr;


class TextSqlEngine : public SqlEngine {
  protected:
    TextOutputBuffer *output;

  public:
    TextSqlEngine(void);
    TextSqlEngine(TextOutputBuffer *outBuffer);
    virtual ~TextSqlEngine(void);
    virtual int execute(SqlExecFrame *frame, void **suppValues);
    virtual int execSelectMult(SqlExecFrame *frame, SqlSelectStat *statement, void **suppValues);
    virtual int execSelectSng(SqlExecFrame *frame, SqlSelectStat *statement, void **suppValues);
    virtual int execInsert(SqlExecFrame *frame, SqlInsertStat *statement, void **suppValues);
    virtual int execDeleteSrch(SqlExecFrame *frame, SqlDeleteStat *statement, void **suppValues);
    virtual int execDeletePos(SqlExecFrame *frame, SqlDeleteStat *statement, void **suppValues);
    virtual int execUpdateSrch(SqlExecFrame *frame, SqlUpdateStat *statement, void **suppValues);
    virtual int execUpdatePos(SqlExecFrame *frame, SqlUpdateStat *statement, void **suppValues);

    virtual int execQueryExpr(SqlExecFrame *frame, SqlQueryExpr *query, void **suppValues);
    virtual int execExpression(SqlExecFrame *frame, SqlExpression *expression, void **suppValues);
    virtual int execOperation(SqlExecFrame *frame, SqlOperExpr *expression, void **suppValues);
    virtual int execConstant(SqlExecFrame *frame, SqlConstantExpr *aConstant);

    virtual int execSelectItem(SqlExecFrame *frame, SqlSelectItem *anItem, void **suppValues);
    virtual int execTableRefItem(SqlExecFrame *frame, SqlTableRef *anItem, void **suppValues);
};



#endif	/* _TEXTSQLENGINE_H_ */
