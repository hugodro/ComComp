/**************************************************
* File: replyValue.cc.
* Desc: Implementation of the SqlReplyValue classes.
* Module: AkraLog : SqlSupport.
* Rev: 25 mai 1998 : REV 0 : Hugo DesRosiers.
**************************************************/

#include "replyValue.h"


/**************************************************
* Implementation: SqlReplyValue.
**************************************************/

SqlReplyValue::SqlReplyValue(void)
{
    dataLength= 0;
    type= unknown;
}


SqlReplyValue::~SqlReplyValue(void)
{
}


void SqlReplyValue::load(int aType, int aSize, int aLength, char *aValue)
{
// TODO: Decide if there is some operation that is occuring at this level.
}


SqlReplyValue::Type SqlReplyValue::getType(void)
{
    return type;
}


unsigned int SqlReplyValue::getAsInteger(void)
{
    return 0;
}


char *SqlReplyValue::getAsString(void)
{
    return NULL;
}

