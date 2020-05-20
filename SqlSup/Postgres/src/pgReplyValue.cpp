/**************************************************
* File: pgReplyValue.cc.
* Desc: Implementation of the PostgresReplyValue classes.
* Module: AkraLog : SqlSupport.
* Rev: 25 mai 1998 : REV 0 : Hugo DesRosiers.
**************************************************/

#include <stdlib.h>
#include "pgReplyValue.h"


/**************************************************
* Implementation: PostgresReplyValue.
**************************************************/

PostgresReplyValue::PostgresReplyValue(void)
    : SqlReplyValue()
{
    data= NULL;
}


PostgresReplyValue::~PostgresReplyValue(void)
{
    if ((type == tVarchar) || (type == tChar)) {
	delete data;
    }
}


void PostgresReplyValue::load(int aType, int aSize, int aLength, char *aValue)
{
    float tmpFloat, *tmpFloatPtr;
    tmpFloatPtr= &tmpFloat;

    switch(aType) {
	case 23:		// int.
	    type= tInt;
	    data= (void *)atoi(aValue);
	    dataLength= 4;
	    break;
	case 700:	// float.
	    type= tReal;
	    tmpFloat= (float)atof(aValue);
	    data= (void *)*(int *)tmpFloatPtr;
	    dataLength= 4;
	    break;
	case 1042:	// char.
	    type= tChar;
	    data= new char[aLength+1];
	    memcpy(data, aValue, aLength);
	    *(((char *)data)+aLength)= '\0';
	    dataLength= aLength+1;
	    break;
	case 1043:	// varchar.
	    type= tVarchar;
	    data= new char[aLength+1];
	    memcpy(data, aValue, aLength);
	    *(((char *)data)+aLength)= '\0';
	    dataLength= aLength+1;
	    break;
	case 1082:	// date.
	    type= tDate;
	    // TODO.
	    break;
    }
}


unsigned int PostgresReplyValue::getAsInteger(void)
{
    return (unsigned int)data;
}


char *PostgresReplyValue::getAsString(void)
{
    return (char *)data;
}

