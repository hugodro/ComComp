/**************************************************
* File: mysqlEngine.cc.
* Desc: Implementaiton of text executor for MySql.
* Module: AkraLog : SqlSupport.
* Rev: 14 juillet 1998 : REV 0 : Hugo DesRosiers : Creation.
**************************************************/

#include "textOutputBuffer.h"
#include "mysqlEngine.h"


MySqlEngine::MySqlEngine(void)
    : TextSqlEngine()
{
    cursorName= NULL;
}


MySqlEngine::MySqlEngine(TextOutputBuffer *outBuffer)
    : TextSqlEngine(outBuffer)
{
    cursorName= NULL;
}


void MySqlEngine::registerCursorName(char *aCursorName)
{
    cursorName= aCursorName;
}


int MySqlEngine::execSelectMult(SqlExecFrame *frame, SqlSelectStat *statement, void **suppValues)
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


int MySqlEngine::execSelectSng(SqlExecFrame *frame, SqlSelectStat *statement, void **suppValues)
{
    if (cursorName[7] == 'z') {
    	cursorName[6]++;
	cursorName[7]= 'a';
    }
    else {
    	cursorName[7]++;
    }
/*
    output->addKeyword(TextOutputBuffer:: kwDeclare);
    output->addText(cursorName);
    output->addKeyword(TextOutputBuffer:: kwCursor);
    output->addKeyword(TextOutputBuffer:: kwFor);
*/
    return TextSqlEngine::execSelectMult(frame, statement, suppValues);
}


