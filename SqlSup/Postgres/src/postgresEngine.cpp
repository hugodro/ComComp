/**************************************************
* File: postgresEngine.cc.
* Desc: Implementaiton of the statement to text executor.
* Module: AkraLog : SqlSupport.
* Rev: 28 mai 1998 : REV 0 : Hugo DesRosiers : Creation.
**************************************************/

#include "textOutputBuffer.h"
#include "postgresEngine.h"


PostgresEngine::PostgresEngine(void)
    : TextSqlEngine()
{
    cursorName= NULL;
}


PostgresEngine::PostgresEngine(TextOutputBuffer *outBuffer)
    : TextSqlEngine(outBuffer)
{
    cursorName= NULL;
}


void PostgresEngine::registerCursorName(char *aCursorName)
{
    cursorName= aCursorName;
}


int PostgresEngine::execSelectMult(SqlExecFrame *frame, SqlSelectStat *statement, void **suppValues)
{
    if (cursorName[7] == 'z') {
    	cursorName[6]++;
	cursorName[7]= 'a';
    }
    else {
    	cursorName[7]++;
    }
    return TextSqlEngine::execSelectMult(frame, statement, suppValues);
}


int PostgresEngine::execSelectSng(SqlExecFrame *frame, SqlSelectStat *statement, void **suppValues)
{
    if (cursorName[7] == 'z') {
    	cursorName[6]++;
	cursorName[7]= 'a';
    }
    else {
    	cursorName[7]++;
    }
    output->addKeyword(TextOutputBuffer:: kwDeclare);
    output->addText(cursorName);
    output->addKeyword(TextOutputBuffer:: kwCursor);
    output->addKeyword(TextOutputBuffer:: kwFor);
    return TextSqlEngine::execSelectMult(frame, statement, suppValues);
}


