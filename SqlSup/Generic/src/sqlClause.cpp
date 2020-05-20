/**************************************************
* File: sqlClause.cc.
* Desc: Definition of the SQL clauses classes (things that don't fit elsewhere).
* Module: AkraLog : SqlSupport.
* Rev: 25 mai 1998 : REV 0 : Hugo DesRosiers : Creation.
**************************************************/

#include "sqlClause.h"

/**************************************************
* Implementation: SqlSelectItem.
**************************************************/

SqlSelectItem::SqlSelectItem(void)
{
    kind= undefined;
    content= NULL;
}


void SqlSelectItem::defineAs(Kind aKind, void *aContent)
{
    if (kind == containerIdent) {
	// TODO: link many identifiers.
    }
    else {
	kind= aKind;
	content= aContent;
    }
}


SqlSelectItem::Kind SqlSelectItem::getContentKind(void)
{
    return kind;
}


SqlExpression *SqlSelectItem::getExpression(void)
{
    return (SqlExpression *)content;
}


SqlIdentifier *SqlSelectItem::getIdentifier(void)
{
    return (SqlIdentifier *)content;
}


/**************************************************
* Implementation: SqlTableRef.
**************************************************/

SqlTableRef::SqlTableRef(void)
{
    kind= undefined;
    content= NULL;
}


void SqlTableRef::defineAs(Kind aKind, void *aContent)
{
    kind= aKind;
    content= aContent;
}


SqlTableRef::Kind SqlTableRef::getContentKind(void)
{
    return kind;
}


SqlIdentifier *SqlTableRef::getIdentifier(void)
{
    return (SqlIdentifier *)content;
}

