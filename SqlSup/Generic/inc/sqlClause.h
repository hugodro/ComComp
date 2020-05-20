#ifndef _SQL_CLAUSE_H_
#define _SQL_CLAUSE_H_
/**************************************************
* File: sqlClause.h.
* Desc: Definition of the SQL clauses classes (things that don't fit elsewhere).
* Module: AkraLog : SqlSupport.
* Rev: 25 mai 1998 : REV 0 : Hugo DesRosiers : Creation.
**************************************************/

#include <akra/AkObject.h>
#include <akra/portableDefs.h>

class SqlExpression;
class SqlIdentifier;


class ClassExport SqlSelectItem : public AkObject {
  public:
    enum Kind {
	star, expression, identifier, containerIdent, function
	, undefined
    };

  protected:
    Kind kind;
    void *content;

  public:
    SqlSelectItem(void);
    virtual void defineAs(Kind aKind, void *aContent);
    Kind getContentKind(void);
    SqlExpression *getExpression(void);
    SqlIdentifier *getIdentifier(void);
};


class ClassExport SqlTableRef : public AkObject {
  public:
    enum Kind {
	simpleName, synonym, extendedSynonym, join
	, undefined
    };

  protected:		// Instance variables.
    Kind kind;
    void *content;

  public:			// Instance methods.
    SqlTableRef(void);
    virtual void defineAs(Kind aKind, void *aContent);
    Kind getContentKind(void);
    SqlIdentifier *getIdentifier(void);
};


#endif		/* _SQL_CLAUSE_H_ */
