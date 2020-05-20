/**************************************************
* File: pgPipe.cc.
* Desc: Implementation of the PostgresPipe class.
* Module: AkraLog : SqlSupport.
* Rev: 26 mai 1998 : REV 0 : Hugo DesRosiers : Creation.
**************************************************/

extern "C" {
#include <stdio.h>
#include <libpq-fe.h>
}

#include <iostream.h>
#include <Compiler/sql3Support.h>
#include "pgTerminal.h"
#include "sqlDataPipe.h"
#include "postgresEngine.h"
#include "textOutputBuffer.h"
#include "execFrame.h"
#include "sqlStatement.h"
#include "pgReplyValue.h"
#include "pgPipe.h"


/**************************************************
* Implementation: PostgresPipe.
**************************************************/

PostgresPipe::PostgresPipe(JVInstance *anInstance)
    : SqlDataPipe(anInstance)
{
    hasTransaction= false;
    strcpy(cursorName, "aaaaaaaa");
    outputBuffer= new TextOutputBuffer();
    engine= new PostgresEngine(outputBuffer);
    engine->registerCursorName(cursorName);
}


PostgresPipe::~PostgresPipe(void)
{
    delete engine;
    delete outputBuffer;
}


int PostgresPipe::execute(SqlExecFrame *aFrame, void **values, unsigned int nbrValues)
{
    int result= -1;
    char *tmpResult;

    engine->execute(aFrame, values);
    tmpResult= outputBuffer->getCompactOutput();
    if (terminal) {
	PGconn *conn= terminal->getConnection();
	PGresult* res;
	if (aFrame->getStatement()->getMainAction() == SELECT_SNG_DATA) {
	    boolean createdTransaction= false;

	    // Must go through a builtin fetch command.
	    if (!hasTransaction) {
		res= PQexec(conn,"BEGIN");
		if (PQresultStatus(res) == PGRES_COMMAND_OK) {
		    hasTransaction= true;
		    createdTransaction= true;
		}
		PQclear(res);
	    }
	    if (hasTransaction) {
		res= PQexec(conn, tmpResult);
		if (PQresultStatus(res) == PGRES_COMMAND_OK) {
		    char tmpFetchCommand[32]= "fetch 2 in ";

		    PQclear(res);
		    strcpy(&tmpFetchCommand[11], cursorName);
		    res= PQexec(conn, tmpFetchCommand);
		    if (PQresultStatus(res) == PGRES_TUPLES_OK) {
			if (PQntuples(res) == 1) {
			    int j, fieldSize, fieldLength;
			    unsigned int replyWidth;
			    Oid fieldType;

			    if ((replyWidth= PQnfields(res)) > 0) {
//				printf("debug: pgPipe select sng row returns %d values:\n", replyWidth);
				aFrame->allocForReply(replyWidth, this);
				for (j=0  ; j < replyWidth; j++) {
				    fieldType= PQftype(res, j);
				    fieldSize= PQfsize(res, j);
				    fieldLength= PQgetlength(res, 0, j);
//				    printf("\t%d: type %d, size %d, length %d: %s\n", j, fieldType, fieldSize, fieldLength, PQgetvalue(res, 0, j));
				    aFrame->loadReply(j, fieldType, fieldSize, fieldLength, PQgetvalue(res, 0, j));
				}
				result= 0;
			    }
			    PQclear(res);
			}
			else {
			    // Error: not one row returned for a select sng row.
			    result= -2;
			    PQclear(res);
			}
		    }
		    else {
			// Error: no row returned for a select sng row.
			result= -2;
			PQclear(res);
		    }
		    strcpy(tmpFetchCommand, "close ");
		    strcpy(&tmpFetchCommand[6], cursorName);
		}
		else {
		    // Error: query did not work.
		    result= -3;
		    PQclear(res);
		}
		if (createdTransaction) {
		    res= PQexec(conn, "END");
		    hasTransaction= false;
		    PQclear(res);
		}
	    }
	}
	else {
	    res= PQexec(conn, tmpResult);
	    if (PQresultStatus(res) != PGRES_COMMAND_OK) {
	    }
	    else {
		result= 0;
	    }
	    PQclear(res);
	}
    }

    return result;
}


void PostgresPipe::installOnTerminal(PostgresTerminal *aTerminal)
{
    terminal= aTerminal;
}


SqlReplyValue **PostgresPipe::makeReplyArray(unsigned int aSize)
{
    PostgresReplyValue **tmpHolder;
    unsigned int i;

    tmpHolder= new PostgresReplyValue *[aSize];
    tmpHolder[0]= new PostgresReplyValue[aSize];
    for (i= 1; i < aSize; i++) {
	tmpHolder[i]= tmpHolder[i-1] + 1;
    }
    return (SqlReplyValue **)tmpHolder;
}

