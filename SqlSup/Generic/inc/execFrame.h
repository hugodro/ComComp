#ifndef _SQLEXECFRAME_H_
#define _SQLEXECFRAME_H_
/**************************************************
* File: execFrame.h.
* Desc: Definition of the SqlExecFrame classes.
* Module: AkraLog : SqlSupport.
* Rev: 25 mai 1998 : REV 0 : Hugo DesRosiers.
**************************************************/

#include <akra/AkObject.h>
#include <akra/portableDefs.h>

class SqlStatement;
class SqlReplyValue;
class SqlDataPipe;


class ClassExport SqlIdentifier : public AkObject {
  public:
    unsigned int charLength;
    unsigned char *value;

  public:
    SqlIdentifier(unsigned int aLength, unsigned char *aValue);
};


// ATTN-980525 [HD]: The SqlConstant currently plays the role of the
//	a CharStr constant.
class ClassExport SqlConstant : public AkObject {
  public:
    unsigned int charLength;
    unsigned char *value;

  public:
    SqlConstant(unsigned int aLength, unsigned char *aValue);
};


class ClassExport SqlExecFrame : public AkObject {
  protected:	// Instance variables.
    unsigned int nbrIdentifiers;
    SqlIdentifier **identifiers;
    unsigned int nbrConstants;
    SqlConstant **constants;
    SqlStatement *statement;
    unsigned int nbrHoles;
    unsigned int signatureLengths[2];
    unsigned char *signatures[2];
    unsigned int replyWidth, nbrReplyAllocated;
    SqlReplyValue **replyValues;

  public:		// Instance methods.
    SqlExecFrame(void);
    virtual ~SqlExecFrame(void);
    virtual unsigned int getVersion(void)= 0;
    virtual void setStatement(SqlStatement *aStatement);
    virtual void incHoleCount(void);
    virtual void incHoleCount(unsigned int aCount);
    virtual void allocIdentTable(unsigned int aSize);
    virtual void allocConstantTable(unsigned int aSize);
    virtual bool setIdentAt(unsigned int position, unsigned int length, unsigned char *data);
    virtual bool setConstantAt(unsigned int position, unsigned int length, unsigned char *data);
    virtual SqlIdentifier *getIdent(unsigned int anIndex);
    virtual void setSignatures(unsigned int inLength, unsigned char *signIn, unsigned int outLength, unsigned char *signOut);
    SqlStatement *getStatement(void);
    virtual SqlConstant *getConstant(unsigned int anIndex);
    virtual unsigned char getHoleType(unsigned int anIndex);
    virtual unsigned int getReplyWidth(void);
    virtual void allocForReply(unsigned int aSize, SqlDataPipe *aPipe);
    virtual void loadReply(unsigned int index, unsigned int type, unsigned int size, unsigned int length, char *strValue);
    virtual SqlReplyValue *getReply(unsigned int index);
};


class Sql92Frame : public SqlExecFrame {
  protected:	// Instance variables.

  public:		// Instance methods.
    virtual unsigned int getVersion(void);
};


class Sql3Frame : public SqlExecFrame {
  protected:	// Instance variables.

  public:		// Instance methods.
    virtual unsigned int getVersion(void);
};




#endif		/* _SQLEXECFRAME_H_ */
