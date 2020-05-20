#ifndef _POSTGRES_SUPPORT_H_
#define _POSTGRES_SUPPORT_H_
/**************************************************
* File: pgSupport.h.
* Desc: Definition of the plzen.sql.PostgresTerminal and associated support classes, in C++.
* Module: AkraLog : PlzenSql.
* Rev: 26 mai 1998 : REV 0 : Hugo DesRosiers : Creation.
**************************************************/

#include <akra/AkObject.h>
#include <akra/portableDefs.h>

#include "dTerminal.h"


class JVMachine;
class ClassFile;
class JVInstance;
class JVVariable;


class plzen_sql_PostgresTerminal : public AkObject {
  protected:	// Class variables.
    static JVVariable *llowField;
    static boolean mustFetchDict;

  public:	// Class methods.
    static boolean fetchDictionary(JVMachine *aMachine, ClassFile *aClass);

  protected:	// Instance variables.
    JVInstance *javaEquiv;

  public:	// Instance methods.
    plzen_sql_PostgresTerminal(JVInstance *anInstance, JVMachine *aMachine);
    ~plzen_sql_PostgresTerminal(void);

	// Field accessors.
    JVInstance *get_this(void);
    int get_linkLow(void);
};


#endif		/* _POSTGRES_SUPPORT_H_ */
