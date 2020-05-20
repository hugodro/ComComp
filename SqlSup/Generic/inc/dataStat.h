#ifndef _SQL_DATA_STATEMENTS_H_
#define _SQL_DATA_STATEMENTS_H_
/**************************************************
* File: dataStat.h.
* Desc: Definition of the SQL data statements classes.
* Module: AkraLog : SqlSupport.
* Rev: 25 mai 1998 : REV 0 : Hugo DesRosiers : Creation.
**************************************************/

#include "sqlStatement.h"

class SqlExecFrame;
class SqlQueryExpr;
class SqlIdentifier;


class SqlDataStatement : public SqlStatement {
  protected:
  public:
};


 class SqlOpenStat : public SqlDataStatement {
  protected:

  public:
    virtual bool parseTokens(SqlExecFrame *frame, unsigned char *tokens, unsigned int tokensLength, unsigned int &tokenRead);
    virtual int getMainAction(void);
};


class SqlFetchStat : public SqlDataStatement {
  protected:

  public:
    virtual bool parseTokens(SqlExecFrame *frame, unsigned char *tokens, unsigned int tokensLength, unsigned int &tokenRead);
    virtual int getMainAction(void);
};


class SqlCloseStat : public SqlDataStatement {
  protected:

  public:
    virtual bool parseTokens(SqlExecFrame *frame, unsigned char *tokens, unsigned int tokensLength, unsigned int &tokenRead);
    virtual int getMainAction(void);
};


class SqlSelectStat : public SqlDataStatement {
  protected:
    SqlQueryExpr *queryExpression;

  public:
    SqlSelectStat(void);
    virtual bool parseTokens(SqlExecFrame *frame, unsigned char *tokens, unsigned int tokensLength, unsigned int &tokenRead);
    virtual int getMainAction(void);
    virtual SqlQueryExpr *getQuery(void);
};


class SqlFreeLocatorStat : public SqlDataStatement {
  protected:

  public:
    virtual bool parseTokens(SqlExecFrame *frame, unsigned char *tokens, unsigned int tokensLength, unsigned int &tokenRead);
    virtual int getMainAction(void);
};


class SqlInsertStat : public SqlDataStatement {
  public:
    enum ValuesKind {
	defaultValues, tableName, valuesList, expression
    };

  protected:
    SqlIdentifier *destination;
    unsigned int nbrColumnNames;
    SqlIdentifier **columnNames;
    ValuesKind valueKind;
    unsigned int nbrValues;
    void *values;
    
  public:
    virtual bool parseTokens(SqlExecFrame *frame, unsigned char *tokens, unsigned int tokensLength, unsigned int &tokenRead);
    virtual int getMainAction(void);
    virtual SqlIdentifier *getDestination(void);
    virtual unsigned int getNbrColumns(void);
    virtual SqlIdentifier *getColumn(unsigned int index);
    virtual ValuesKind getValuesKind(void);
    virtual unsigned int getNbrValues(void);
    virtual void *getValues(void);
};


class SqlDeleteStat : public SqlDataStatement {
  protected:

  public:
    virtual bool parseTokens(SqlExecFrame *frame, unsigned char *tokens, unsigned int tokensLength, unsigned int &tokenRead);
    virtual int getMainAction(void);
};


class SqlUpdateStat : public SqlDataStatement {
  protected:

  public:
    virtual bool parseTokens(SqlExecFrame *frame, unsigned char *tokens, unsigned int tokensLength, unsigned int &tokenRead);
    virtual int getMainAction(void);
};



#endif		/* _SQL_DATA_STATEMENTS_H_ */
