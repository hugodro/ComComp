/**************************************************
* File: execFrame.cc.
* Desc: Implementation of the SqlExecFrame classes.
* Module: AkraLog : SqlSupport.
* Rev: 25 mai 1998 : REV 0 : Hugo DesRosiers.
**************************************************/

#if defined(_WIN32)
#include <memory.h>
#endif
#include "sqlStatement.h"
#include "replyValue.h"	// TMP.
#include "execFrame.h"


/**************************************************
* Implementation: SqlIdentifier.
**************************************************/

SqlIdentifier::SqlIdentifier(unsigned int aLength, unsigned char *aValue)
{
    charLength= aLength;
    value= new unsigned char[aLength];
    memcpy(value, aValue, aLength);
}


/**************************************************
* Implementation: SqlConstant.
**************************************************/

SqlConstant::SqlConstant(unsigned int aLength, unsigned char *aValue)
{
    charLength= aLength;
    value= new unsigned char[aLength];
    memcpy(value, aValue, aLength);
}


/**************************************************
* Implementation: SqlExecFrame.
**************************************************/

SqlExecFrame::SqlExecFrame(void)
{
    statement= NULL;
    nbrIdentifiers= 0;
    nbrConstants= 0;
    nbrHoles= 0;
    signatureLengths[0]= signatureLengths[1]= 0;
    replyWidth= nbrReplyAllocated= 0;
    replyValues= NULL;
}


SqlExecFrame::~SqlExecFrame(void)
{
    unsigned int i;

    if (statement != NULL) delete statement;
    if (nbrConstants > 0) {
	for (i= 0; i < nbrConstants; i++) {
	    delete constants[i];
	}
	delete[] constants;
    }
    if (nbrIdentifiers > 0) {
	for (i= 0; i < nbrIdentifiers; i++) {
	    delete identifiers[i];
	}
	delete[] identifiers;
    }
    if ((signatureLengths[0] != 0) || (signatureLengths[1] != 0)) delete[] signatures[0];
}


void SqlExecFrame::setStatement(SqlStatement *aStatement)
{
    statement= aStatement;
}


void SqlExecFrame::incHoleCount(void)
{
    nbrHoles++;
}


void SqlExecFrame::incHoleCount(unsigned int aCount)
{
    nbrHoles+= aCount;
}


void SqlExecFrame::allocIdentTable(unsigned int aSize)
{
    if (aSize != 0) identifiers= new SqlIdentifier *[aSize];
    nbrIdentifiers= aSize;
}


void SqlExecFrame::allocConstantTable(unsigned int aSize)
{
    if (aSize != 0) constants= new SqlConstant *[aSize];
    nbrConstants= aSize;
}


bool SqlExecFrame::setIdentAt(unsigned int position, unsigned int length, unsigned char *data)
{
    identifiers[position]= new SqlIdentifier(length, data);
    return true;    // TMPTMP: Always succeed for now.
}


bool SqlExecFrame::setConstantAt(unsigned int position, unsigned int length, unsigned char *data)
{
    constants[position]= new SqlConstant(length, data);
    return true;    // TMPTMP: Always succeed for now.
}


SqlIdentifier *SqlExecFrame::getIdent(unsigned int anIndex)
{
    return identifiers[anIndex];
}


void SqlExecFrame::setSignatures(unsigned int inLength, unsigned char *signIn, unsigned int outLength, unsigned char *signOut)
{
    signatureLengths[0]= inLength;
    signatureLengths[1]= outLength;
    signatures[0]= new unsigned char[inLength + outLength];
    signatures[1]= signatures[0] + inLength;
    memcpy(signatures[0], signIn, inLength);
    memcpy(signatures[1], signOut, outLength);
}


SqlStatement *SqlExecFrame::getStatement(void)
{
    return statement;
}


SqlConstant *SqlExecFrame::getConstant(unsigned int anIndex)
{
    return constants[anIndex];
}


unsigned char SqlExecFrame:: getHoleType(unsigned int anIndex)
{
    if (anIndex < signatureLengths[0]) {
	return signatures[0][anIndex];
    }
    return 'V';
}


unsigned int SqlExecFrame::getReplyWidth(void)
{
    return replyWidth;
}


void SqlExecFrame::allocForReply(unsigned int aSize, SqlDataPipe *aPipe)
{
    if (nbrReplyAllocated < aSize) {
	if (replyValues != NULL) {
	    delete[] replyValues[0];
	    delete[] replyValues;
	}
	replyValues= aPipe->makeReplyArray(aSize);
	nbrReplyAllocated= aSize;
    }
    replyWidth= aSize;
}


// TMP: The SqlExecFrame can not load the replies, since they will be in database-specific format.
// This method has to be implemented in the pipe.
void SqlExecFrame::loadReply(unsigned int index, unsigned int type, unsigned int size, unsigned int length, char *strValue)
{
    if (index < replyWidth) {
	replyValues[index]->load(type, size, length, strValue);
    }
}


SqlReplyValue *SqlExecFrame::getReply(unsigned int index)
{
    if (index < replyWidth) return replyValues[index];
    else return NULL;
}


/**************************************************
* Implementation: Sql92Frame.
**************************************************/

unsigned int Sql92Frame::getVersion(void)
{
    return SqlDataPipe::sql92;
}


/**************************************************
* Implementation: Sql3Frame.
**************************************************/

unsigned int Sql3Frame::getVersion(void)
{
    return SqlDataPipe::sql3;
}

