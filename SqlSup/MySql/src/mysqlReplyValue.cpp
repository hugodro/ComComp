/**************************************************
* File: mysqlReplyValue.cc.
* Desc: Implementation of the MySqlReplyValue classes.
* Module: AkraLog : SqlSupport.
* Rev: 19 juillet 1998 : REV 0 : Hugo DesRosiers.
**************************************************/

#include <stdlib.h>
#if defined(_WIN32)
#include <string.h>
#endif
#include <mysql/mysql.h>
#include "mysqlReplyValue.h"


/**************************************************
* Implementation: MySqlReplyValue.
**************************************************/

MySqlReplyValue::MySqlReplyValue(void)
    : SqlReplyValue()
{
    data= NULL;
}


MySqlReplyValue::~MySqlReplyValue(void)
{
    if ((type == tVarchar) || (type == tChar)) {
	delete data;
    }
}


void MySqlReplyValue::load(int aType, int aSize, int aLength, char *aValue)
{
    float tmpFloat, *tmpFloatPtr;
    tmpFloatPtr= &tmpFloat;

    switch(aType) {
	    // int.
	case FIELD_TYPE_DECIMAL:
	case FIELD_TYPE_TINY:
	case FIELD_TYPE_SHORT:
	case FIELD_TYPE_LONG:
	    type= tInt;
	    if (aValue != NULL) {
		data= (void *)atoi(aValue);
	    }
	    else {
		// Got a sql-null.
		data= 0;
	    }
	    dataLength= 4;
	    break;
	    // Real.
	case FIELD_TYPE_FLOAT:
	    type= tReal;
	    if (aValue != NULL) {
		tmpFloat= (float)atof(aValue);
    	    }
	    else {
		// Got a sql-null.
		tmpFloat= 0.0;
	    }

	    data= (void *)*(int *)tmpFloatPtr;
	    dataLength= 4;
	    break;
	case FIELD_TYPE_STRING:	// char.
	    type= tChar;
	    data= new char[aLength+1];
	    memcpy(data, aValue, aLength);
	    *(((char *)data)+aLength)= '\0';
	    dataLength= aLength+1;
	    break;
	case FIELD_TYPE_VAR_STRING:	// varchar.
	    type= tVarchar;
	    data= new char[aLength+1];
	    memcpy(data, aValue, aLength);
	    *(((char *)data)+aLength)= '\0';
	    dataLength= aLength+1;
	    break;
	case FIELD_TYPE_DATE:	// date.
	    type= tDate;
	    // TODO.
	    break;
/* TODO:
	      FIELD_TYPE_NULL,
	      FIELD_TYPE_TIMESTAMP,
	      FIELD_TYPE_LONGLONG,
	      FIELD_TYPE_INT24,
	      FIELD_TYPE_TIME,
	      FIELD_TYPE_DATETIME,
	      FIELD_TYPE_YEAR,
	      FIELD_TYPE_NEWDATE,
	      FIELD_TYPE_ENUM=247,
	      FIELD_TYPE_SET=248,
	      FIELD_TYPE_TINY_BLOB=249,
	      FIELD_TYPE_MEDIUM_BLOB=250,
	      FIELD_TYPE_LONG_BLOB=251,
	      FIELD_TYPE_BLOB=252,
	      FIELD_TYPE_VAR_STRING=253,
	      FIELD_TYPE_STRING=254
*/
    }
}


unsigned int MySqlReplyValue::getAsInteger(void)
{
    return (unsigned int)data;
}


char *MySqlReplyValue::getAsString(void)
{
    return (char *)data;
}

