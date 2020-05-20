#ifndef _SQL_EXPRESSION_H_
#define _SQL_EXPRESSION_H_
/**************************************************
* File: sqlExpression.h.
* Desc: Definition of the SqlExpression class and subclasses.
* Module: AkraLog : SqlSupport.
* Rev: 25 mai 1998 : REV 0 : Hugo DesRosiers : Creation.
**************************************************/

#include <akra/AkObject.h>
#include <akra/portableDefs.h>
#include "sqlOpcodes.h"

class SqlSelectItem;
class SqlTableRef;
class SqlExecFrame;
class SqlIdentifier;


class ClassExport SqlExpression : public AkObject {
  public:
    enum Categories {
	cOperation, cIdentifier, cHole,  cQuery, cConstant, cFunction
    };

  public:    // Class methods.
    static SqlExpression *newExpressionFor(SqlTokens::Tokens aToken);

  protected:

  public:
    virtual bool parseTokens(SqlExecFrame *frame, unsigned char *tokens, unsigned int tokensLength, unsigned int &tokenRead)= 0;
    virtual Categories getCategory(void)= 0;
};


class ClassExport SqlQueryExpr : public SqlExpression {
  protected:
    unsigned char selectMode;
    unsigned int nbrSelectItems;
    SqlSelectItem *selectArray;
    unsigned int nbrTableRefs;
    SqlTableRef *tableRefArray;
    SqlExpression *where;
	// TODO: Add groubBy, having, orderBy.

  public:
    SqlQueryExpr(void);
    virtual bool parseTokens(SqlExecFrame *frame, unsigned char *tokens, unsigned int tokensLength, unsigned int &tokenRead);
    virtual unsigned int getSelectMode(void);
    virtual unsigned int getNbrSelectItems(void);
    virtual SqlSelectItem *getSelectItems(void);
    virtual unsigned int getNbrTableRef(void);
    virtual SqlTableRef *getTableRefs(void);
    virtual SqlExpression *getWherePart(void);
    virtual Categories getCategory(void);
};


class ClassExport SqlOperExpr : public SqlExpression {
  protected:
    SqlTokens::Tokens operation;
    SqlExpression *left, *right;

  public:		// Instance methods.
    SqlOperExpr(void);
    SqlOperExpr(SqlTokens::Tokens aToken);
    virtual bool parseTokens(SqlExecFrame *frame, unsigned char *tokens, unsigned int tokensLength, unsigned int &tokenRead);
    virtual Categories getCategory(void);
    virtual SqlTokens::Tokens getOperation(void);
    virtual SqlExpression *getLeftOperand(void);
    virtual SqlExpression *getRightOperand(void);
};


class ClassExport SqlIdentifierExpr : public SqlExpression {
  protected:
    void *value;

  public:
    virtual bool parseTokens(SqlExecFrame *frame, unsigned char *tokens, unsigned int tokensLength, unsigned int &tokenRead);
    virtual Categories getCategory(void);
    virtual SqlIdentifier *getValue(void);
};


class ClassExport SqlHoleExpr : public SqlExpression {
  protected:
    unsigned int index;

  public:
    virtual bool parseTokens(SqlExecFrame *frame, unsigned char *tokens, unsigned int tokensLength, unsigned int &tokenRead);
    virtual Categories getCategory(void);
    virtual unsigned int getIndex(void);
};



class ClassExport SqlFunctionExpr : public SqlExpression {
  protected:
    SqlTokens::Tokens operation;
    SqlExpression *left, *right;

  public:		// Instance methods.
    SqlFunctionExpr(void);
    SqlFunctionExpr(SqlTokens::Tokens aToken);
    virtual bool parseTokens(SqlExecFrame *frame, unsigned char *tokens, unsigned int tokensLength, unsigned int &tokenRead);
    virtual Categories getCategory(void);
    virtual SqlTokens::Tokens getOperation(void);
    virtual SqlExpression *getLeftOperand(void);
    virtual SqlExpression *getRightOperand(void);
};

class ClassExport SqlConstantExpr : public SqlExpression {
  public:
    enum CteType {
	ctBoolean, ctByte, ctShort, ctInteger, ctReal, ctCharStr, ctSpecial, ctEnum, ctNull
    };

  protected:
    CteType type;

  public:
    SqlConstantExpr(void);
    SqlConstantExpr(CteType aType);
    CteType getType(void);
    virtual bool parseTokens(SqlExecFrame *frame, unsigned char *tokens, unsigned int tokensLength, unsigned int &tokenRead);
    virtual Categories getCategory(void);
};


/********************************************************
* Class: SqlCompactCte.
* Desc: Constant value of fixed length, using a single word of memory.
* Class variables:
* Instance variables:
* Super-Class: SqlExpression.
* Module: NativeLib : SqlSupport.
********************************************************/

class ClassExport SqlCompactCte : public SqlConstantExpr {
  protected:
    unsigned int value;

  public:
    SqlCompactCte(void);
    SqlCompactCte(CteType aType);
    SqlCompactCte(CteType aType, unsigned int aValue);
    virtual bool parseTokens(SqlExecFrame *frame, unsigned char *tokens, unsigned int tokensLength, unsigned int &tokenRead);
    unsigned int getValue(void);
};


class ClassExport SqlCharStrCte : public SqlConstantExpr {
  protected:
    unsigned int reference;

  public:
    SqlCharStrCte(void);
    virtual bool parseTokens(SqlExecFrame *frame, unsigned char *tokens, unsigned int tokensLength, unsigned int &tokenRead);
    virtual unsigned int getValue(void);
};


#endif		/* _SQL_EXPRESSION_H_ */
