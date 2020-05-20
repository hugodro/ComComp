/**************************************************
* File: pgSupport.cc.
* Desc: Implementation of the plzen.sql.PostgresTerminal support class, in C++.
* Module: AkraLog : PlzenSql.
* Rev: 26 mai 1998 : REV 0 : Hugo DesRosiers : Creation.
**************************************************/

#include <RunTime/jmachine.h>
#include <NativeLib/nativity.h>
#include <RunTime/classFile.h>
#include <RunTime/jvInstance.h>
#include <RunTime/execFrame.h>
#include <RunTime/stacks.h>
#include <RunTime/stringSupport.h>
#include "nativeError.h"
#include "pgTerminal.h"
#include "pgPipe.h"
#include "pgSupport.h"


JVVariable *plzen_sql_PostgresTerminal::llowField;
boolean plzen_sql_PostgresTerminal::mustFetchDict= true;


boolean plzen_sql_PostgresTerminal::fetchDictionary(JVMachine *aMachine, ClassFile *aClass)
{
    if (mustFetchDict) {
	llowField= aMachine->getFieldOfClass(aClass, "linkLow");
	mustFetchDict= false;
    }

    return (llowField != NULL);
}


plzen_sql_PostgresTerminal::plzen_sql_PostgresTerminal(JVInstance *anInstance, JVMachine *aMachine)
{
    javaEquiv= anInstance;
    if (mustFetchDict) {
	if (!fetchDictionary(aMachine, javaEquiv->category())) {
#if !defined(NeXT)
	    throw new NativeError(1);
#endif
	}
    }
}


plzen_sql_PostgresTerminal::~plzen_sql_PostgresTerminal(void)
{
    // Anything to do ?
}


JVInstance *plzen_sql_PostgresTerminal::get_this(void)
{
    return javaEquiv;
}


int plzen_sql_PostgresTerminal::get_linkLow(void)
{
    return (int)javaEquiv->getField(llowField);
}


// Native implemented.
Define_Native(plzen_sql, PostgresTerminal, createLowLevel);
Define_Native(plzen_sql, PostgresTerminal, disconnect0);
Define_Native(plzen_sql, PostgresTerminal, hasConn0);

Define_Native(plzen_sql, PostgresPipe, linkLowLevel);


//    private native int createLowLevel();
Implement_Native(plzen_sql, PostgresTerminal, createLowLevel)
{
    JVInstance *receiver;
    JVRunValue *tmpValue;
    plzen_sql_PostgresTerminal *plzenTerminal;
    PostgresTerminal *pgTerminal;
    unsigned int gotAProblem= 0;
    char *serverDef[5]= { NULL, NULL, NULL, NULL, NULL };

    tmpValue= callFrame->getLocalValue(0);
    if (tmpValue != NULL) {
	if ((receiver= tmpValue->extractInstance()) != NULL) {
	    plzenTerminal= new plzen_sql_PostgresTerminal(receiver, machine);
	}
	else gotAProblem= 2;
    }
    else gotAProblem= 1;

    if (gotAProblem != 0) {
	if (gotAProblem > 2) delete plzenTerminal;
	callFrame->returnInteger(0);
	return 0;
    }
    pgTerminal= new PostgresTerminal(receiver);

    serverDef[4]= "testHugo1";
    pgTerminal->connectToServer(serverDef, 5);

    delete plzenTerminal;
    callFrame->returnInteger((int)pgTerminal);
    return 0;
}


//    native boolean disconnect0();
Implement_Native(plzen_sql, PostgresTerminal, disconnect0)
{
    JVInstance *receiver;
    JVRunValue *tmpValue;
    plzen_sql_PostgresTerminal *plzenTerminal;
    PostgresTerminal *pgTerm;
    unsigned int gotAProblem= 0;

    tmpValue= callFrame->getLocalValue(0);
    if (tmpValue != NULL) {
	if ((receiver= tmpValue->extractInstance()) != NULL) {
	    plzenTerminal= new plzen_sql_PostgresTerminal(receiver, machine);
	}
	else gotAProblem= 2;
    }
    else gotAProblem= 1;

    if (gotAProblem != 0) {
	if (gotAProblem > 2) delete plzenTerminal;
	callFrame->returnInteger(0);
	return 0;
    }

    pgTerm= (PostgresTerminal *)plzenTerminal->get_linkLow();
    if (pgTerm != NULL) {
	pgTerm->disconnect();
	callFrame->returnInteger(1);
    }
    else {
	callFrame->returnInteger(0);
    }
 
    delete plzenTerminal;
    return 0;
}


//    native boolean hasConn0();
Implement_Native(plzen_sql, PostgresTerminal, hasConn0)
{
// TODO: Do a real check on connection !
    callFrame->returnInteger(1);
   
    return 0;
}


//    private native int linkLowLevel();
Implement_Native(plzen_sql, PostgresPipe, linkLowLevel)
{
    JVInstance *receiver, *terminal;
    JVRunValue *tmpValue;
    plzen_sql_DataPipe *pipeDual;
    PostgresPipe *sqlPipe;
    unsigned int gotAProblem= 0;

    tmpValue= callFrame->getLocalValue(0);
    if (tmpValue != NULL) {
	if ((receiver= tmpValue->extractInstance()) != NULL) {
	    pipeDual= new plzen_sql_DataPipe(receiver, machine);
	}
	else gotAProblem= 2;
    }
    else gotAProblem= 1;

    if (gotAProblem != 0) {
	if (gotAProblem > 2) delete pipeDual;
	callFrame->returnInteger(0);
	return 0;
    }

    sqlPipe= new PostgresPipe(receiver);
    if ((terminal= pipeDual->get_terminal()) != NULL) {
	plzen_sql_PostgresTerminal pzTerminal(terminal, machine);
	sqlPipe->installOnTerminal((PostgresTerminal *)pzTerminal.get_linkLow());
    }

    delete pipeDual;
    callFrame->returnInteger((int)sqlPipe);
    return 0;
}


