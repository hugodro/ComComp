/**************************************************
* File: dataStat.cc.
* Desc: Implementation of the SQL data statements classes.
* Module: AkraLog : SqlSupport.
* Rev: 25 mai 1998 : REV 0 : Hugo DesRosiers : Creation.
**************************************************/

#include "sqlOpcodes.h"
#include "sql3Support.h"
#include "sqlExpression.h"
#include "execFrame.h"
#include "textHelper.h"
#include "dataStat.h"


/**************************************************
* Implementation: SqlOpenStat.
**************************************************/

bool SqlOpenStat::parseTokens(SqlExecFrame *frame, unsigned char *tokens, unsigned int tokensLength, unsigned int &tokenRead)
{
    return false;
}

int SqlOpenStat::getMainAction(void)
{
    return OPEN_DATA;
}


/**************************************************
* Implementation: SqlFetchStat.
**************************************************/

bool SqlFetchStat::parseTokens(SqlExecFrame *frame, unsigned char *tokens, unsigned int tokensLength, unsigned int &tokenRead)
{
    return false;
}

int SqlFetchStat::getMainAction(void)
{
    return FETCH_DATA;
}


/**************************************************
* Implementation: SqlCloseStat.
**************************************************/

bool SqlCloseStat::parseTokens(SqlExecFrame *frame, unsigned char *tokens, unsigned int tokensLength, unsigned int &tokenRead)
{
    return false;
}

int SqlCloseStat::getMainAction(void)
{
    return CLOSE_DATA;
}


/**************************************************
* Implementation: SqlSelectStat.
**************************************************/

SqlSelectStat::SqlSelectStat(void)
{
    queryExpression= NULL;
}


bool SqlSelectStat::parseTokens(SqlExecFrame *frame, unsigned char *tokens, unsigned int tokensLength, unsigned int &tokenRead)
{
    SqlTokens::Tokens currentToken;
    int errorCode= 0;
    unsigned int section= 0;

    currentToken= (SqlTokens::Tokens)*tokens++;
    tokenRead++;
    if (currentToken != SqlTokens::queryExpr) {
	errorCode= -1;
    }
    else {
	queryExpression= new SqlQueryExpr();
	if (!queryExpression->parseTokens(frame, tokens, tokensLength - tokenRead, tokenRead)) {
	    errorCode= -1;
	}
    }

    return (errorCode > -1);		// Positive error codes are not failures.
}


int SqlSelectStat::getMainAction(void)
{
    return SELECT_SNG_DATA;
}


SqlQueryExpr *SqlSelectStat::getQuery(void)
{
    return queryExpression;
}


/**************************************************
* Implementation: SqlFreeLocatorStat.
**************************************************/

bool SqlFreeLocatorStat::parseTokens(SqlExecFrame *frame, unsigned char *tokens, unsigned int tokensLength, unsigned int &tokenRead)
{
    return false;
}

int SqlFreeLocatorStat::getMainAction(void)
{
    return FREELOCATOR_DATA;
}


/**************************************************
* Implementation: SqlInsertStat.
**************************************************/

bool SqlInsertStat::parseTokens(SqlExecFrame *frame, unsigned char *tokens, unsigned int tokensLength, unsigned int &tokenRead)
{
    SqlExpression **exprValues;
    int errorCode= 0, readDiff;
    unsigned int section= 0, i, j;
    unsigned char formatFlag;
    SqlTokens::Tokens currentToken;
    bool readMore= true, skipRead= false;

	// Sections are: 0: init, 1: get statement format, 2: get column names.
    nbrValues= 0;
    do {
	if (skipRead) {
	    skipRead= false;
	}
	else {
	    currentToken= (SqlTokens::Tokens)*tokens++;
	    tokenRead++;
	}
	switch(section) {
	    case 0:
		if (currentToken != SqlTokens::identReference) {
		    readMore= false;
		    errorCode= -1;
		}
		else {
		    j= (unsigned int)ReadUShort(tokens);
		    tokens+= 2;
		    tokenRead+= 2;	    
		    destination= frame->getIdent(j);
		    section= 1;
		}
		break;
	    case 1:
		if ((currentToken & 0x0E0)  != SqlTokens::immediateConstant) {
		    readMore= false;
		    errorCode= -2;
		}
		else {
		    formatFlag= currentToken & 0x1F;
		    if (formatFlag & 1) {
			valueKind= ((formatFlag & 2) ?
						((formatFlag & 4) ? valuesList : tableName) : expression);
			if (formatFlag & 8) {
			    section= 2;
			}
			else {
			    nbrColumnNames= 0;
			    if (valueKind == valuesList) {
				section= 4;
			    }
			    else if (valueKind == tableName) {
				// TODO.
			    }
			    else if (valueKind == expression) {
				// TODO.
			    }
			}
		    }
		    else {
			valueKind= defaultValues;
			readMore= false;
		    }
		}
		break;
	    case 2:	// Read column names count.
		if (currentToken == SqlTokens::list) {
		    nbrColumnNames= (unsigned int)ReadUShort(tokens);
		    tokens+= 2;
		    tokenRead+= 2;
		}
		else {
		    nbrColumnNames= 1;
		    skipRead= true;
		}
		columnNames= new SqlIdentifier*[nbrColumnNames];
		i= 0;
		section= 3;
		break;
	    case 3:	// Read column names
		if (currentToken != SqlTokens::identReference) {
		    readMore= false;
		    errorCode= -3;
		}
		else {
		    j= (unsigned int)ReadUShort(tokens);
		    tokens+= 2;
		    tokenRead+= 2;	    
		    columnNames[i++]= frame->getIdent(j);
		    if (i == nbrColumnNames) {
			if (valueKind == valuesList) {
			    section= 4;
			}
			else if (valueKind == tableName) {
			    // TODO.
			}
			else if (valueKind == expression) {
			    // TODO.
			}
		    }
		}
		break;
	    case 4:	// Read value list.
		if (currentToken == SqlTokens::list) {
		    nbrValues= (unsigned int)ReadUShort(tokens);
		    tokens+= 2;
		    tokenRead+= 2;
		    exprValues= new SqlExpression*[nbrValues];
		    values= (void *)exprValues;
		    i= 0;
		    section= 5;
		}
		else {
		    readMore= false;
		    errorCode= -4;
		}
		break;
	    case 5:
		if ((exprValues[i]= SqlExpression::newExpressionFor(currentToken)) != NULL) {
		    readDiff= tokenRead;
		    if (!exprValues[i]->parseTokens(frame, tokens, tokensLength, tokenRead)) {
			for (j= 0; j < i; j++) delete exprValues[j];
			delete[] exprValues;
			readMore= false;
			errorCode= -5;
		    }
		    else {
			tokens= tokens + tokenRead - readDiff;
			i++;
			if (i == nbrValues) {
			    readMore= false;
			}
		    }
		}
		else {
		    readMore= false;
		    errorCode= -2;
		}
		break;
	}
    } while (readMore && (tokenRead < tokensLength));

    return (errorCode > -1);
}


int SqlInsertStat::getMainAction(void)
{
    return INSERT_DATA;
}


SqlIdentifier *SqlInsertStat::getDestination(void)
{
    return destination;
}


unsigned int SqlInsertStat::getNbrColumns(void)
{
    return nbrColumnNames;
}


SqlIdentifier *SqlInsertStat::getColumn(unsigned int index)
{
    if (index < nbrColumnNames) return columnNames[index];
    else return NULL;
}


SqlInsertStat::ValuesKind SqlInsertStat::getValuesKind(void)
{
    return valueKind;
}


unsigned int SqlInsertStat::getNbrValues(void)
{
    return nbrValues;
}


void *SqlInsertStat::getValues(void)
{
    return values;
}


/**************************************************
* Implementation: SqlDeleteStat.
**************************************************/

bool SqlDeleteStat::parseTokens(SqlExecFrame *frame, unsigned char *tokens, unsigned int tokensLength, unsigned int &tokenRead)
{
    return false;
}


int SqlDeleteStat::getMainAction(void)
{
    return DELETE_SEARCH_DATA;
}


/**************************************************
* Implementation: SqlUpdateStat.
**************************************************/

bool SqlUpdateStat::parseTokens(SqlExecFrame *frame, unsigned char *tokens, unsigned int tokensLength, unsigned int &tokenRead)
{
    return false;
}

int SqlUpdateStat::getMainAction(void)
{
    return UPDATE_SEARCH_DATA;
}


