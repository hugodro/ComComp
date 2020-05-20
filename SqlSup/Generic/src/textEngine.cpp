/**************************************************
* File: textEngine.cc.
* Desc: Implementation of the statement to text executor.
* Module: AkraLog : SqlSupport.
* Rev: 27 mai 1998 : REV 0 : Hugo DesRosiers : Creation.
**************************************************/

#include <Compiler/sql3Support.h>
#include <Compiler/constants.h>
#include <RunTime/jvInstance.h>
#include "dataStat.h"
#include "textOutputBuffer.h"
#include "execFrame.h"
#include "sqlClause.h"
#include "sqlExpression.h"
#include "textEngine.h"


TextSqlEngine::TextSqlEngine(void)
{
    output= NULL;
}


TextSqlEngine::TextSqlEngine(TextOutputBuffer *outBuffer)
{
    output= outBuffer;
}


TextSqlEngine::~TextSqlEngine(void)
{
// TODO.
}


int TextSqlEngine:: execute(SqlExecFrame *frame, void **suppValues)
{
    SqlStatement *statement;
    int result= 0;

    output->reset();
    if ((statement= frame->getStatement()) != NULL) {
	switch(statement->getMainAction()) {
	    case SELECT_MULT_DATA:
		result= execSelectMult(frame, (SqlSelectStat *)statement, suppValues);
		break;
	    case SELECT_SNG_DATA:
		result= execSelectSng(frame, (SqlSelectStat *)statement, suppValues);
		break;
	    case INSERT_DATA:
		result= execInsert(frame, (SqlInsertStat *)statement, suppValues);
		break;
	    case DELETE_SEARCH_DATA:
		result= execDeleteSrch(frame, (SqlDeleteStat *)statement, suppValues);
		break;
	    case DELETE_POS_DATA:
		result= execDeletePos(frame, (SqlDeleteStat *)statement, suppValues);
		break;
	    case UPDATE_SEARCH_DATA:
		result= execUpdateSrch(frame, (SqlUpdateStat *)statement, suppValues);
		break;
	    case UPDATE_POS_DATA:
		result= execUpdatePos(frame, (SqlUpdateStat *)statement, suppValues);
		break;
	}
    }
    else result= -1;

    return result;
}


int TextSqlEngine::execSelectMult(SqlExecFrame *frame, SqlSelectStat *statement, void **suppValues)
{
    int result= 0;

    SqlQueryExpr *query= statement->getQuery();
    execQueryExpr(frame, query, suppValues);
    
    return result;
}


int TextSqlEngine::execSelectSng(SqlExecFrame *frame, SqlSelectStat *statement, void **suppValues)
{
    int result= 0;
    SqlQueryExpr *query= statement->getQuery();
    execQueryExpr(frame, query, suppValues);
    
    return result;
}


int TextSqlEngine::execInsert(SqlExecFrame *frame, SqlInsertStat *statement, void **suppValues)
{
    int result= 0;
    unsigned int nbrColumns, i;
    
    output->addKeyword(TextOutputBuffer::kwInsert);
    output->addKeyword(TextOutputBuffer::kwInto);
    output->addIdentifier(statement->getDestination());
    if (statement->getValuesKind() == SqlInsertStat::defaultValues) {
	output->addKeyword(TextOutputBuffer::kwDefault);
	output->addKeyword(TextOutputBuffer::kwValues);
    }
    else {
	if ((nbrColumns= statement->getNbrColumns()) > 0) {
	    output->addParenthesis(false);
	    i= 0;
	    do {
		output->addIdentifier(statement->getColumn(i++));
		if (i < nbrColumns) output->addListSeparator();
		else break;
	    } while (1);
	    output->addParenthesis(true);
	}
	if (statement->getValuesKind() == SqlInsertStat::valuesList) {
	    SqlExpression **values;

	    output->addKeyword(TextOutputBuffer::kwValues);
	    nbrColumns= statement->getNbrValues();
	    values= (SqlExpression **)statement->getValues();
		// ATTN-980602 [HD]: According to SQL92 standard, parentheses are not necessary,
		// and thus must be given by the statement.  But SQL engines seen so far do not give a
		// choice, and as of now the compiler implies the presence of parentheses.
	    output->addParenthesis(false);
	    i= 0; 
	    do {
		execExpression(frame, values[i++], suppValues);
		if (i < nbrColumns) output->addListSeparator();
		else break;
	    } while (1);
	    output->addParenthesis(true);
	}
	else if (statement->getValuesKind() == SqlInsertStat::tableName) {
	    output->addKeyword(TextOutputBuffer::kwTable);
	    output->addIdentifier((SqlIdentifier *)statement->getValues());
	}
	else if (statement->getValuesKind() == SqlInsertStat::expression) {
	    // TODO.
	}
    }
    return result;
}


int TextSqlEngine::execDeleteSrch(SqlExecFrame *frame, SqlDeleteStat *statement, void **suppValues)
{
    int result= 0;
    return result;
}


int TextSqlEngine::execDeletePos(SqlExecFrame *frame, SqlDeleteStat *statement, void **suppValues)
{
    int result= 0;
    return result;
}


int TextSqlEngine::execUpdateSrch(SqlExecFrame *frame, SqlUpdateStat *statement, void **suppValues)
{
    int result= 0;
    return result;
}


int TextSqlEngine::execUpdatePos(SqlExecFrame *frame, SqlUpdateStat *statement, void **suppValues)
{
    int result= 0;
    return result;
}


int TextSqlEngine::execQueryExpr(SqlExecFrame *frame, SqlQueryExpr *query, void **suppValues)
{
    SqlSelectItem *selectItems;
    SqlTableRef *tableRefs;
    SqlExpression *expression;
    
    int result= 0;
    unsigned int i, nbrItems;

    output->addKeyword(TextOutputBuffer::kwSelect);
    nbrItems= query->getNbrSelectItems();
    selectItems= query->getSelectItems();
    i= 0;
    do {
	execSelectItem(frame, &selectItems[i++], suppValues);
	if (i < nbrItems) {
	    output->addListSeparator();
	}
	else break;
    } while(1);

    output->addKeyword(TextOutputBuffer::kwFrom);
    nbrItems= query->getNbrTableRef();
    tableRefs= query->getTableRefs();
    i= 0;
    do {
	execTableRefItem(frame, &tableRefs[i++], suppValues);
	if (i < nbrItems) {
	    output->addListSeparator();
	}
	else break;
    } while(1);

    if ((expression= query->getWherePart()) != NULL) {
	output->addKeyword(TextOutputBuffer::kwWhere);
	execExpression(frame, expression, suppValues);
    }
    // TODO: Add the other components of a query.
    return result;
}


int TextSqlEngine::execExpression(SqlExecFrame *frame, SqlExpression *expression, void **suppValues)
{
    JVInstance *strValue;
    float *tmpFloatPtr;
    int result= 0;

    switch(expression->getCategory()) {
	case SqlExpression::cOperation:
	    execOperation(frame, (SqlOperExpr *)expression, suppValues);
	    break;
	case SqlExpression::cIdentifier:
	    output->addIdentifier(((SqlIdentifierExpr *)expression)->getValue());
	    break;
	case SqlExpression::cHole:
	    switch(frame->getHoleType(((SqlHoleExpr *)expression)->getIndex())) {
		case JVM_TYPE_BOOLEAN:
		    // TODO.
		    break;
		case JVM_TYPE_SIGNED_BYTE:
		    // TODO.
		    break;
		case JVM_TYPE_CHAR:
		    // TODO.
		    break;
		case JVM_TYPE_UNSIGNED_SHORT:
		    // TODO.
		    break;
		case JVM_TYPE_INTEGER:
		    output->addIntConstant((int)suppValues[((SqlHoleExpr *)expression)->getIndex()]);
		    break;
		case JVM_TYPE_FLOAT:
		    tmpFloatPtr= (float *)&suppValues[((SqlHoleExpr *)expression)->getIndex()];
		    output->addRealConstant(*tmpFloatPtr);
		    break;
		case JVM_TYPE_LONG:
		    // TODO.
		    break;
		case JVM_TYPE_DOUBLE:
		    // TODO.
		    break;
		case AKR_TYPE_STRING:
		    strValue= (JVInstance *)suppValues[((SqlHoleExpr *)expression)->getIndex()];
		    if (strValue != JVAbstractInstance::globalNullObject()) {
			JVBasicArray *charArray;
			unsigned int length, offset;

			charArray= (JVBasicArray *)strValue->getAtIndex(0);
			offset= (unsigned int)strValue->getAtIndex(1);
			length= (unsigned int)strValue->getAtIndex(2);
			output->addStringConstant((char *)charArray->getElementPtrAt(offset), length);
		    }
		    else {
			output->addKeyword(TextOutputBuffer::kwNull);
		    }
		    break;
	    }
	    break;
	case SqlExpression::cQuery:
	    execQueryExpr(frame, (SqlQueryExpr *)expression, suppValues);
	    break;
	case SqlExpression::cConstant:
	    execConstant(frame, (SqlConstantExpr *)expression);
	    break;
    }
    return result;
}


int TextSqlEngine::execOperation(SqlExecFrame *frame, SqlOperExpr *expression, void **suppValues)
{
    int result= 0;
    JCSqlTokens::Tokens theOp= expression->getOperation();

    switch(theOp) {
	case JCSqlTokens::parenthesisOp:
	    output->addParenthesis(false);
	    execExpression(frame, expression->getLeftOperand(), suppValues);
	    output->addParenthesis(true);
	    break;
	case JCSqlTokens::escapeOp:
	    output->addKeyword(TextOutputBuffer::kwEscape);
	    execExpression(frame, expression->getLeftOperand(), suppValues);
	    break;
	case JCSqlTokens::existsOp:
	    output->addKeyword(TextOutputBuffer::kwExists);
	    execExpression(frame, expression->getLeftOperand(), suppValues);
	    break;
	case JCSqlTokens::uniqueOp:
	    output->addKeyword(TextOutputBuffer::kwUnique);
	    execExpression(frame, expression->getLeftOperand(), suppValues);
	    break;
	case JCSqlTokens::posateOp:
	    output->addSingleOp('+');
	    execExpression(frame, expression->getLeftOperand(), suppValues);
	    break;
	case JCSqlTokens::negateOp:
	    output->addSingleOp('-');
	    execExpression(frame, expression->getLeftOperand(), suppValues);
	    break;
	default:
	    execExpression(frame, expression->getLeftOperand(), suppValues);
	    output->addOperator(theOp);
	    execExpression(frame, expression->getRightOperand(), suppValues);
	    break;
    }
    return result;
}


int TextSqlEngine::execConstant(SqlExecFrame *frame, SqlConstantExpr *aConstant)
{
    int result= 0;
    switch(aConstant->getType()) {
	case SqlConstantExpr::ctBoolean:
	    switch(((SqlCompactCte *)aConstant)->getValue()) {
		case 0x01:
		    output->addKeyword(TextOutputBuffer::kwTrue);
		    break;
		case 0x00:
		    output->addKeyword(TextOutputBuffer::kwFalse);
		    break;
		case 0x0FFFFFFFF:
		    output->addKeyword(TextOutputBuffer::kwUnknown);
		    break;
	    }
	    break;
	case SqlConstantExpr::ctByte:
	    output->addIntConstant((int)((SqlCompactCte *)aConstant)->getValue());
	    break;
	case SqlConstantExpr::ctShort:
	    output->addIntConstant((int)((SqlCompactCte *)aConstant)->getValue());
	    break;
	case SqlConstantExpr::ctInteger:
	    output->addIntConstant((int)((SqlCompactCte *)aConstant)->getValue());
	    break;
	case SqlConstantExpr::ctReal:
	    break;
	case SqlConstantExpr::ctCharStr:
	    output->addStringConstant(frame->getConstant(((SqlCharStrCte *)aConstant)->getValue()));
	    break;
	case SqlConstantExpr::ctSpecial:
	    // TODO.
	    break;
	case SqlConstantExpr::ctEnum:
	    // TODO.
	    break;
	case SqlConstantExpr::ctNull:
	    output->addKeyword(TextOutputBuffer::kwNull);
	    break;
    }
    return result;
}


int TextSqlEngine::execSelectItem(SqlExecFrame *frame, SqlSelectItem *anItem, void **suppValues)
{
    int result= 0;

    switch(anItem->getContentKind()) {
	case SqlSelectItem::star:
	    output->addSingleOp('*');
	    break;
	case SqlSelectItem::expression:
	    execExpression(frame, anItem->getExpression(), suppValues);
	    break;
	case SqlSelectItem::identifier:
	    output->addIdentifier(anItem->getIdentifier());
	    break;
	case SqlSelectItem::containerIdent:
	    output->addIdentifier(anItem->getIdentifier());
	    output->addSingleOp('.');
	    break;
    }

    return result;
}


int TextSqlEngine::execTableRefItem(SqlExecFrame *frame, SqlTableRef *anItem, void **suppValues)
{
    int result= 0;

    switch(anItem->getContentKind()) {
	case SqlTableRef::simpleName:
	    output->addIdentifier(anItem->getIdentifier());
	    break;
	case SqlTableRef::synonym:
	    // TODO.
	    break;
	case SqlTableRef::extendedSynonym:
	    // TODO.
	    break;
	case SqlTableRef::join:
	    // TODO.
	    break;
    }
    return result;
}
