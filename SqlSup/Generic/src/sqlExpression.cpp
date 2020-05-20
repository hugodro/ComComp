/**************************************************
* File: sqlExpression.cc.
* Desc: Implementation of the SqlExpression class and subclasses.
* Module: AkraLog : SqlSupport.
* Rev: 25 mai 1998 : REV 0 : Hugo DesRosiers : Creation.
**************************************************/

#include <Compiler/sqlSpitters.h>
#include "textHelper.h"
#include "sqlClause.h"
#include "execFrame.h"
#include "sqlExpression.h"


/**************************************************
* Implementation: SqlExpression.
**************************************************/

SqlExpression *SqlExpression::newExpressionFor(JCSqlTokens::Tokens aToken)
{
    switch(aToken) {
	case JCSqlTokens::queryExpr:
	    return new SqlQueryExpr();
	    break;
	case JCSqlTokens::identReference:
	    return new SqlIdentifierExpr();
	    break;
	case JCSqlTokens::nativeHolder:
	    return new SqlHoleExpr();
	    break;
	case JCSqlTokens::nullIdentifier:
	    return new SqlConstantExpr(SqlConstantExpr::ctNull);
	    break;
	case JCSqlTokens::specialConstant:
	    return new SqlCompactCte(SqlConstantExpr::ctSpecial);
	    break;
	case JCSqlTokens::enumConstant:
	    return new SqlCompactCte(SqlConstantExpr::ctEnum);
	    break;
	case JCSqlTokens::byteLiteral:
	    return new SqlCompactCte(SqlConstantExpr::ctByte);
	    break;
	case JCSqlTokens::shortILiteral:
	    return new SqlCompactCte(SqlConstantExpr::ctShort);
	    break;
	case JCSqlTokens::integerLiteral:
	    return new SqlCompactCte(SqlConstantExpr::ctInteger);
	    break;
	case JCSqlTokens::floatLiteral:
	    return new SqlCompactCte(SqlConstantExpr::ctReal);
	    break;
	case JCSqlTokens::trueBoolLiteral:
	    return new SqlCompactCte(SqlConstantExpr::ctBoolean, 1);
	    break;
	case JCSqlTokens::falseBoolLiteral:
	    return new SqlCompactCte(SqlConstantExpr::ctBoolean, 0);
	    break;
	case JCSqlTokens::maybeBoolLiteral:
	    return new SqlCompactCte(SqlConstantExpr::ctBoolean, 0x0FFFFFFFF);
	    break;
	case JCSqlTokens::charLiteral:
	    return new SqlCharStrCte();
	    break;
	case JCSqlTokens::orOp:
	case JCSqlTokens::andOp:
	case JCSqlTokens::notOp:
	case JCSqlTokens::isOp:
	case JCSqlTokens::isnotOp:
	case JCSqlTokens::parenthesisOp:
	case JCSqlTokens::equalOp:
	case JCSqlTokens::notEqualOp:
	case JCSqlTokens::ltOp:
	case JCSqlTokens::gtOp:
	case JCSqlTokens::leOp:
	case JCSqlTokens::geOp:
	case JCSqlTokens::betweenOp:
	case JCSqlTokens::notBetweenOp:
	case JCSqlTokens::inOp:
	case JCSqlTokens::notInOp:
	case JCSqlTokens::likeOp:
	case JCSqlTokens::notLikeOp:
	case JCSqlTokens::escapeOp:
	case JCSqlTokens::eqAllOp:
	case JCSqlTokens::eqSomeOp:
	case JCSqlTokens::eqAnyOp:
	case JCSqlTokens::neAllOp:
	case JCSqlTokens::neSomeOp:
	case JCSqlTokens::neAnyOp:
	case JCSqlTokens::ltAllOp:
	case JCSqlTokens::ltSomeOp:
	case JCSqlTokens::ltAnyOp:
	case JCSqlTokens::gtAllOp:
	case JCSqlTokens::gtSomeOp:
	case JCSqlTokens::gtAnyOp:
	case JCSqlTokens::leAllOp:
	case JCSqlTokens::leSomeOp:
	case JCSqlTokens::leAnyOp:
	case JCSqlTokens::geAllOp:
	case JCSqlTokens::geSomeOp:
	case JCSqlTokens::geAnyOp:
	case JCSqlTokens::existsOp:
	case JCSqlTokens::uniqueOp:
	case JCSqlTokens::overlapsOp:
	case JCSqlTokens::matchOp:
	case JCSqlTokens::matchUniqueOp:
	case JCSqlTokens::matchPartialOp:
	case JCSqlTokens::matchFullOp:
	case JCSqlTokens::matchUniPartialOp:
	case JCSqlTokens::matchUniFullOp:
	case JCSqlTokens::addOp:
	case JCSqlTokens::substractOp:
	case JCSqlTokens::multiplyOp:
	case JCSqlTokens::divideOp:
	case JCSqlTokens::posateOp:
	case JCSqlTokens::negateOp:
	case JCSqlTokens::userDefinedOp:
	    return new SqlOperExpr(aToken);
	    break;
    }

    return NULL;
}


/**************************************************
* Implementation: SqlQueryExpr.
**************************************************/

SqlQueryExpr::SqlQueryExpr(void)
{
    selectMode= 0x00;
    nbrSelectItems= nbrTableRefs= 0;
    selectArray= NULL;
    tableRefArray= NULL;
	// Optional components.
    where= NULL;
/* TODO:
    groupBy= NULL;
    having= NULL;
    orderBy= NULL;
*/
}


bool SqlQueryExpr::parseTokens(SqlExecFrame *frame, unsigned char *tokens, unsigned int tokensLength, unsigned int &tokenRead)
{
    JCSqlTokens::Tokens currentToken;
    int errorCode= 0;
    unsigned int section= 0, i, j, readDiff;
    unsigned char optionFormat;
    bool readMore= true, skipRead= false;

	// Sections are: 0: start, : read selectList, : read tableList, : read format, : read 'where'.
	// TODO: add the rest of the sections.
    do {
	if (skipRead) {
	    skipRead= false;
	}
	else {
	    currentToken= (JCSqlTokens::Tokens)*tokens++;
	    tokenRead++;
	}
	switch(section) {
	    case 0:
		if ((currentToken & 0x0E0) != JCSqlTokens::immediateConstant) {
		    readMore= false;
		    errorCode= -4;
		}
		else {
		    selectMode= currentToken & 0x1F;
		    section= 1;
		}
		break;
	    case 1:
		if (currentToken == JCSqlTokens::list) {
		    nbrSelectItems= (unsigned int)ReadUShort(tokens);
		    tokens+= 2;
		    tokenRead+= 2;
		}
		else {
		    nbrSelectItems= 1;
		    skipRead= true;
		}
		selectArray= new SqlSelectItem[nbrSelectItems];
		i= 0;
		section= 2;
		break;
	    case 2:		// Read a SelectItem.
		switch(currentToken) {
		    case JCSqlTokens::identReference:
			j= (unsigned int)ReadUShort(tokens);
			tokens+= 2;
			tokenRead+= 2;
			selectArray[i].defineAs(SqlSelectItem::identifier, frame->getIdent(j));
			i++;
			break;
		    case JCSqlTokens::containerReference:
			j= (unsigned int)ReadUShort(tokens);
			tokens+= 2;
			tokenRead+= 2;
			selectArray[i].defineAs(SqlSelectItem::containerIdent, frame->getIdent(j));
			break;
		    case JCSqlTokens::starIdentifier:
			selectArray[i].defineAs(SqlSelectItem::star, NULL);
			i++;
			break;
		    case JCSqlTokens::functionCall:
			currentToken= (unsigned int)ReadUByte(tokens);
			tokens++;
			tokenRead++;
			if (currentToken == JCSqlTokens::fctCustom) {
			    // TODO: Handle custom function calls.
			    // ATTN: Parsing will crash if we come by here without taking care of the
			    //	'todo'.
			    readMore= false;
			    errorCode= -100;	// Unsupported feature.
			}
			else {
			    SqlFunctionExpr *fctExpr;

			    fctExpr= new SqlFunctionExpr(currentToken);
			    if (!fctExpr->parseTokens(frame, tokens, tokensLength, tokenRead)) {
				readMore= false;
				errorCode= -4;	// Function is badly encoded.
			    }
			    else {
			      selectArray[i].defineAs(SqlSelectItem::function, fctExpr);
			    }
			}
			i++;
			break;
		    default:	// TODO: treat other kind of tokens (opers, constants, etc).
			break;
		}
		if (i == nbrSelectItems) section= 3;
		break;
	    case 3:		// Prepare reading 'from' list.
		if (currentToken == JCSqlTokens::list) {
		    nbrTableRefs= (unsigned int)ReadUShort(tokens);
		    tokens+= 2;
		    tokenRead+= 2;
		}
		else {
		    nbrTableRefs= 1;
		    skipRead= true;
		}
		tableRefArray= new SqlTableRef[nbrTableRefs];
		i= 0;
		section= 4;
		break;
	    case 4:		// Reading a TableRef item.
		switch(currentToken) {
		    case JCSqlTokens::identReference:    // Simple name.
			j= (unsigned int)ReadUShort(tokens);
			tokens+= 2;
			tokenRead+= 2;
			tableRefArray[i].defineAs(SqlTableRef::simpleName, frame->getIdent(j));
			i++;
			break;
		    case JCSqlTokens::extSynonymExpr:    // columns, name, source.
		    case JCSqlTokens::synonymExpr:	    // name, source.
		    case JCSqlTokens::queryExpr:		// Joined tables.
			// TODO: Take care of other kind of table definitions.
		    default:
			break;
		}
		if (i == nbrTableRefs) section= 5;
		break;
	    case 5:
	    	if ((currentToken & JCSqlTokens::immediateConstant) == JCSqlTokens::immediateConstant) {
		    optionFormat= (currentToken & 0x01F);
		    if (optionFormat & 0x01) section= 6;
		    else if (optionFormat & 0x02) section= 7;
		    else if (optionFormat & 0x04) section= 8;
		    else if (optionFormat & 0x08) section= 9;
		    else readMore= false;
		}
		else {		// Error, didn't find the format specification.
		    readMore= false;
		    errorCode= -1;
		}
		break;
	    case 6:	// Read 'where'.
		if ((where= SqlExpression::newExpressionFor(currentToken)) != NULL) {
		    readDiff= tokenRead;
		    if (!where->parseTokens(frame, tokens, tokensLength, tokenRead)) {
			delete where;
			readMore= false;
			errorCode= -3;
		    }
		    else {
			tokens= tokens + tokenRead - readDiff;
			if (optionFormat & 0x02) section= 7;
			else if (optionFormat & 0x04) section= 8;
			else if (optionFormat & 0x08) section= 9;
			else readMore= false;
		    }
		}
		else {
		    readMore= false;
		    errorCode= -2;
		}
		break;
	    case 7:	// Read 'group by'.
		// TODO: read the 'group by' block
		break;
	    case 8:	// Read 'having'.
		// TODO: read the 'having' block
		break;
	    case 9:	// Read 'order by'.
		// TODO: read the 'order by' block
		break;
	    default:		// Impossible section.
		readMore= false;
		break;
	}
    } while (readMore && (tokenRead < tokensLength));

    return (errorCode > -1);		// Positive error codes are not failures.
}


unsigned int SqlQueryExpr::getSelectMode(void)
{
    return (unsigned int)selectMode;
}


unsigned int SqlQueryExpr::getNbrSelectItems(void)
{
    return nbrSelectItems;
}


SqlSelectItem *SqlQueryExpr::getSelectItems(void)
{
    return selectArray;
}


unsigned int SqlQueryExpr::getNbrTableRef(void)
{
    return nbrTableRefs;
}


SqlTableRef *SqlQueryExpr::getTableRefs(void)
{
    return tableRefArray;
}


SqlExpression *SqlQueryExpr::getWherePart(void)
{
    return where;
}


SqlExpression::Categories SqlQueryExpr::getCategory(void)
{
    return cQuery;
}


/**************************************************
* Implementation: SqlOperExpr.
**************************************************/

SqlOperExpr::SqlOperExpr(void)
{
    operation= (JCSqlTokens::Tokens)0;
    left= right= NULL;
}


SqlOperExpr::SqlOperExpr(JCSqlTokens::Tokens aToken)
{
    operation= aToken;
    left= right= NULL;
}


bool SqlOperExpr::parseTokens(SqlExecFrame *frame, unsigned char *tokens, unsigned int tokensLength, unsigned int &tokenRead)
{
    JCSqlTokens::Tokens currentToken;
    int errorCode= 0;
    unsigned int section= 0, i, j , readDiff;
    unsigned char optionFormat;
    bool readMore= true;

    do {
	currentToken= (JCSqlTokens::Tokens)*tokens++;
	tokenRead++;
	switch(section) {
	    case 0:
		if ((left= SqlExpression::newExpressionFor(currentToken)) != NULL) {
		    readDiff= tokenRead;
		    if (!left->parseTokens(frame, tokens, tokensLength, tokenRead)) {
			delete left;
			readMore= false;
			errorCode= -2;
		    }
		    else {
			readDiff= tokenRead - readDiff;
			switch(operation) {
			    case JCSqlTokens::parenthesisOp:
			    case JCSqlTokens::escapeOp:
			    case JCSqlTokens::existsOp:
			    case JCSqlTokens::uniqueOp:
			    case JCSqlTokens::posateOp:
			    case JCSqlTokens::negateOp:
				readMore= false;
				break;
			    default:
				tokens= tokens + readDiff;
				section= 1;
				break;
			}
		    }
		}
		else {
		    readMore= false;
		    errorCode= -1;
		}
		break;
	    case 1:
		if ((right= SqlExpression::newExpressionFor(currentToken)) != NULL) {
		    if (!right->parseTokens(frame, tokens, tokensLength, tokenRead)) {
			delete left;
			delete right;
			errorCode= -3;
		    }
		}
		readMore= false;
		break;
	}
    } while (readMore && (tokenRead < tokensLength));


    return (errorCode > -1);		// Positive error codes are not failures.
}


SqlExpression::Categories SqlOperExpr::getCategory(void)
{
    return cOperation;
}


JCSqlTokens::Tokens SqlOperExpr::getOperation(void)
{
    return operation;
}


SqlExpression *SqlOperExpr::getLeftOperand(void)
{
    return left;
}


SqlExpression *SqlOperExpr::getRightOperand(void)
{
    return right;
}


/**************************************************
* Implementation: SqlIdentifierExpr.
**************************************************/

bool SqlIdentifierExpr::parseTokens(SqlExecFrame *frame, unsigned char *tokens, unsigned int tokensLength, unsigned int &tokenRead)
{
    int errorCode= 0;
    unsigned int tmpIndex;

    tmpIndex= (unsigned int)ReadUShort(tokens);
    tokenRead+= 2;
    value= frame->getIdent(tmpIndex);

    return (errorCode > -1);
}


SqlExpression::Categories SqlIdentifierExpr::getCategory(void)
{
    return cIdentifier;
}


SqlIdentifier *SqlIdentifierExpr::getValue(void)
{
    return (SqlIdentifier *)value;
}


/**************************************************
* Implementation: SqlHoleExpr.
**************************************************/

bool SqlHoleExpr::parseTokens(SqlExecFrame *frame, unsigned char *tokens, unsigned int tokensLength, unsigned int &tokenRead)
{
    int errorCode= 0;

    index= (unsigned int)ReadUShort(tokens);
    tokenRead+= 2;

    return (errorCode > -1);
}



SqlExpression::Categories SqlHoleExpr::getCategory(void)
{
    return cHole;
}


unsigned int SqlHoleExpr::getIndex(void)
{
    return index;
}


/**************************************************
* Implementation: SqlConstantExpr.
**************************************************/

SqlConstantExpr::SqlConstantExpr(void)
{
    type= ctInteger;		// Warning: should we use a 'unknown' type ?
}


SqlConstantExpr::SqlConstantExpr(CteType aType)
{
    type= aType;
}


SqlConstantExpr::CteType SqlConstantExpr::getType(void)
{
    return type;
}


bool SqlConstantExpr::parseTokens(SqlExecFrame *frame, unsigned char *tokens, unsigned int tokensLength, unsigned int &tokenRead)
{
    int errorCode= 0;

    if (type != ctNull) {
	errorCode= -1;		// Only the 'null' constant is not represented by a subclass.
    }

    return (errorCode > -1);
}


SqlExpression::Categories SqlConstantExpr::getCategory(void)
{
    return cConstant;
}


/**************************************************
* Implementation: SqlCompactCte.
**************************************************/

SqlCompactCte::SqlCompactCte(void)
    : SqlConstantExpr()
{
    value= 0;
}


SqlCompactCte::SqlCompactCte(CteType aType)
    : SqlConstantExpr(aType)
{
    value= 0;
}


SqlCompactCte::SqlCompactCte(CteType aType, unsigned int aValue)
    : SqlConstantExpr(aType)
{
    value= aValue;
}


bool SqlCompactCte::parseTokens(SqlExecFrame *frame, unsigned char *tokens, unsigned int tokensLength, unsigned int &tokenRead)
{
    int errorCode= 0;

    if (type != ctBoolean) {
	switch(type) {
	    case ctSpecial:
	    case ctByte:
		value= (unsigned int)*tokens++;
		tokenRead++;
		break;
	    case ctEnum:
	    case ctShort:
		value= (unsigned int)ReadUShort(tokens);
		tokenRead+= 2;
		break;
	    case ctInteger:
		value= ReadUInt(tokens);
		tokenRead+= 4;
		break;
	    case ctReal:
		value= ReadUInt(tokens);
		tokenRead+= 4;
		break;
	    default:
		errorCode= -1;		// Unknown constant type.
		break;
	}
    }

    return (errorCode > -1);
}


unsigned int SqlCompactCte::getValue(void)
{
    return value;
}


/**************************************************
* Implementation: SqlCharStrCte.
**************************************************/

SqlCharStrCte::SqlCharStrCte(void)
    : SqlConstantExpr(SqlConstantExpr::ctCharStr)
{
    reference= 0;
}


bool SqlCharStrCte::parseTokens(SqlExecFrame *frame, unsigned char *tokens, unsigned int tokensLength, unsigned int &tokenRead)
{
    int errorCode= 0;

    reference= (unsigned int)ReadUShort(tokens);
    tokenRead+= 2;

    return (errorCode > -1);
}


unsigned int SqlCharStrCte::getValue(void)
{
    return reference;
}


/**************************************************
* Implementation: SqlFunctionExpr.
**************************************************/
SqlFunctionExpr::SqlFunctionExpr(void)
{
// TODO.
}

SqlFunctionExpr::SqlFunctionExpr(JCSqlTokens::Tokens aToken)
{
}


bool SqlFunctionExpr::parseTokens(SqlExecFrame *frame, unsigned char *tokens, unsigned int tokensLength, unsigned int &tokenRead)
{
// TODO.
  return false;
}

SqlExpression::Categories SqlFunctionExpr::getCategory(void)
{
// TODO.
    return cFunction;
}

JCSqlTokens::Tokens SqlFunctionExpr::getOperation(void)
{
// TODO.
    return operation;
}

SqlExpression *SqlFunctionExpr::getLeftOperand(void)
{
// TODO.
    return left;
}

SqlExpression *SqlFunctionExpr::getRightOperand(void)
{
// TODO.
    return right;
}
