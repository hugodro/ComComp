/**************************************************
* File: sqlDataPipe.cc.
* Desc: Implementation of the SqlDataPipe class.
* Module: AkraLog : SqlSupport.
* Rev: 23 mai 1998 : REV 0 : Hugo DesRosiers : Creation.
**************************************************/

#include <Compiler/sql3Support.h>
#include "textHelper.h"
#include "execFrame.h"
#include "sqlStatement.h"
#include "dataStat.h"
#include "replyValue.h"
#include "sqlDataPipe.h"


/**************************************************
* Implementation: SqlDataPipe.
**************************************************/

SqlStatement *SqlDataPipe::sql92StatementCreator(unsigned char aToken)
{

    switch(aToken) {
	case SELECT_MULT_DATA:
	    return new SqlSelectStat();
	    break;
	case SELECT_SNG_DATA:
	    return new SqlSelectStat();
	    break;
	case INSERT_DATA:
	    return new SqlInsertStat();
	    break;
	case DELETE_SEARCH_DATA:
	    return new SqlDeleteStat();
	    break;
	case DELETE_POS_DATA:
	    return new SqlDeleteStat();
	    break;
	case UPDATE_SEARCH_DATA:
	    return new SqlUpdateStat();
	    break;
	case UPDATE_POS_DATA:
	    return new SqlUpdateStat();
	    break;
	// TODO: implement the other statements.
    }

    return NULL;		// Unknown statement.
}


SqlStatement *SqlDataPipe::sql3StatementCreator(unsigned char aToken)
{
// TODO.

    return NULL;		// Unknown statement.
}


SqlDataPipe::SqlDataPipe(JVInstance *anInstance)
{
    dual= anInstance;
    tmpKnownCount= 0;
}


SqlExecFrame  *SqlDataPipe::parseTokens(unsigned char *charArray, unsigned int dataLength, JVInstance *source)
{
    SqlExecFrame *execFrame;
    unsigned int codeStart;
    unsigned int i, sqlVersion;
    unsigned char *tokens;

    for (i= 0; i < tmpKnownCount; i++) {
	if (tmpKnownStrings[i] == source) return tmpKnownFrames[i];
    }

    codeStart= ReadUInt(charArray);
    charArray+= 4;
    tokens= charArray + codeStart;
    sqlVersion= (unsigned int)*charArray++;
    if (sqlVersion < 128) {
	execFrame= new Sql92Frame();

	parseSql92((Sql92Frame *)execFrame, charArray, tokens, sqlVersion, dataLength-5);
    }
    else {
	execFrame= new Sql3Frame();

	parseSql3((Sql3Frame *)execFrame, charArray, tokens, sqlVersion-128, dataLength-5);
    }

    tmpKnownFrames[tmpKnownCount]= execFrame;
    tmpKnownStrings[tmpKnownCount++]= source;
    return execFrame;
}


SqlStatement *SqlDataPipe::parseSql92(Sql92Frame *frame, unsigned char *constantBytes, unsigned char *tokenBytes, unsigned int version, unsigned int dataLength)
{
    SqlStatement *statement;
    unsigned int nbrIdent, nbrConstants, identLength, tokensLength, tokensRead= 0;
    unsigned int i;

    nbrIdent= ReadUShort(constantBytes);
    constantBytes+= 2;
    nbrConstants= ReadUShort(constantBytes);
    constantBytes+= 2;

    if (nbrIdent > 0) {
	frame->allocIdentTable(nbrIdent);
	for (i= 0; i < nbrIdent; i++) {
	    identLength= ReadUShort(constantBytes);
	    constantBytes+= 2;
	    frame->setIdentAt(i, identLength, constantBytes);
	    constantBytes+= identLength;
	}
    }

    if (nbrConstants > 0) {
	frame->allocConstantTable(nbrConstants);
	for (i= 0; i < nbrConstants; i++) {
	    identLength= ReadUShort(constantBytes);
	    constantBytes+= 2;
	    frame->setConstantAt(i, identLength, constantBytes);
	    constantBytes+= identLength;
	}
    }

    if (constantBytes != tokenBytes) {
	// We missed something in the ident/constants.  For now, break away.
	return NULL;
    }

    tokensLength= ReadUShort(tokenBytes);
    tokenBytes+= 2;
    statement= newStatementForToken(*tokenBytes++, sql92);

    if (statement->parseTokens(frame, tokenBytes, tokensLength, tokensRead)) {
	unsigned int inLength, outLength;
	unsigned char *signIn, *signOut;

	frame->setStatement(statement);
	tokenBytes+= tokensRead;
	inLength= ReadUShort(tokenBytes);
	tokenBytes+= 2; tokensRead+= 2;
	signIn= tokenBytes;
	tokenBytes+= inLength;
	outLength= ReadUShort(tokenBytes);
	tokenBytes+= 2; tokensRead+= 2;
	signOut= tokenBytes;
	tokenBytes+= outLength;
	frame->setSignatures(inLength, signIn, outLength, signOut);
    }
    else {
	delete statement;
	statement= NULL;
    }

    return statement;
}


SqlStatement *SqlDataPipe::parseSql3(Sql3Frame *frame, unsigned char *constantBytes, unsigned char *tokenBytes, unsigned int version, unsigned int dataLength)
{
// TODO.
    return NULL;
}


SqlStatement *SqlDataPipe::newStatementForToken(unsigned char aToken, SqlConstant sqlVersion)
{
    if (sqlVersion == sql92) {
	return sql92StatementCreator(aToken);
    }
    else if (sqlVersion == sql3) {
	return sql3StatementCreator(aToken);
    }

    return NULL;		// This is an error situation (can't match required sql version).
}


int SqlDataPipe::execute(SqlExecFrame *aFrame, void **values, unsigned int nbrValues)
{
    // Does nothing.
    return 1;
}


bool SqlDataPipe::putReplyOnStack(JVOperStack *operStack, unsigned int nbrValues)
{
    if (nbrValues > 0) return false;
    return true;
}


SqlReplyValue **SqlDataPipe::makeReplyArray(unsigned int aSize)
{
    SqlReplyValue **tmpHolder;
    unsigned int i;

    tmpHolder= new SqlReplyValue *[aSize];
    tmpHolder[0]= new SqlReplyValue[aSize];
    for (i= 1; i < aSize; i++) {
	tmpHolder[i]= tmpHolder[i-1] + 1;
    }
    return tmpHolder;
}

