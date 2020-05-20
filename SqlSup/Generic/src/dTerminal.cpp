/**************************************************
* File: dTerminal.cc.
* Desc: Implementation of the plzen.sql.DataTerminal and associated support classes, in C++.
* Module: AkraLog : PlzenSql.
* Rev: 23 mai 1998 : REV 0 : Hugo DesRosiers : Creation.
**************************************************/

#if defined(_WIN32)
#include <string.h>
extern size_t  __cdecl strlen(const char *);
#endif

#include <RunTime/jmachine.h>
#include <NativeLib/nativity.h>
#include <RunTime/classFile.h>
#include <RunTime/jvInstance.h>
#include <RunTime/execFrame.h>
#include <RunTime/stacks.h>
#include <RunTime/stringSupport.h>
#include <RunTime/exceptions.h>
#include "sqlDataPipe.h"
#include "execFrame.h"
#include "replyValue.h"
#include "dTerminal.h"

JVVariable *plzen_sql_DataPipe::llowField;
JVVariable *plzen_sql_DataPipe::terminalField;
bool plzen_sql_DataPipe::mustFetchDict= true;


bool plzen_sql_DataPipe::fetchDictionary(JVMachine *aMachine, ClassFile *aClass)
{
    if (mustFetchDict) {
	llowField= aMachine->getFieldOfClass(aClass, "linkLow");
	terminalField= aMachine->getFieldOfClass(aClass, "terminal");
	mustFetchDict= false;
    }

    return ((llowField != NULL) && (terminalField != NULL));
}


plzen_sql_DataPipe::plzen_sql_DataPipe(JVInstance *anInstance, JVMachine *aMachine)
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


plzen_sql_DataPipe::~plzen_sql_DataPipe(void)
{
    // Anything to do ?
}


JVInstance *plzen_sql_DataPipe::get_this(void)
{
    return javaEquiv;
}


int plzen_sql_DataPipe::get_linkLow(void)
{
    return (int)javaEquiv->getField(llowField);
}


JVInstance *plzen_sql_DataPipe::get_terminal(void)
{
    return (JVInstance *)javaEquiv->getField(terminalField);
}



// Native implemented.
Define_Native(plzen_sql, DataPipe, linkLowLevel);
Define_Native(plzen_sql, DataPipe, execSql0);

//    private native int linkLowLevel();
Implement_Native(plzen_sql, DataPipe, linkLowLevel)
{
    JVInstance *receiver;
    JVRunValue *tmpValue;
    plzen_sql_DataPipe *pipeDual;
    SqlDataPipe *sqlPipe;
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
    sqlPipe= new SqlDataPipe(receiver);

    delete pipeDual;
    callFrame->returnInteger((int)sqlPipe);
    return 0;
}


//    private native bool execSql0(String tokens, int stackIn, int stackOut);
Implement_Native(plzen_sql, DataPipe, execSql0)
{
    JVInstance *receiver, *tokensStr;
    JVRunValue *tmpValue;
    plzen_sql_DataPipe *pipeDual;
    SqlDataPipe *sqlPipe;
    SqlExecFrame *execFrame;
    unsigned int gotAProblem= 0;
    unsigned int stackIn, stackOut;
    void **inValues;
    unsigned char *tokenCharArray, *unifiedTokens;
    
// TODO: Manage error situations !

    tmpValue= callFrame->getLocalValue(0);
    if (tmpValue != NULL) {
	if ((receiver= tmpValue->extractInstance()) != NULL) {
	    pipeDual= new plzen_sql_DataPipe(receiver, machine);
	}
	else gotAProblem= 2;
     }
     else gotAProblem= 1;

	// Get <String tokens>.
    tmpValue= callFrame->getLocalValue(1);
    if (tmpValue != NULL) {
	if ((tokensStr= tmpValue->extractInstance()) != NULL) {
	    if (tokensStr != JVAbstractInstance::globalNullObject()) {
		StringSupport::mapStringToNarrowArray(tokensStr, (char **)&tokenCharArray);
	    }
	    else gotAProblem= 5;
	 }
	 else gotAProblem= 4;
     }
     else gotAProblem= 3;

    tmpValue= callFrame->getLocalValue(2);
    if (tmpValue != NULL) {
	stackIn= tmpValue->extractInteger();
    }
    else gotAProblem= 6;

    tmpValue= callFrame->getLocalValue(3);
    if (tmpValue != NULL) {
	stackOut= tmpValue->extractInteger();
    }
    else gotAProblem= 7;

    sqlPipe= (SqlDataPipe *)pipeDual->get_linkLow();

	// ATTN: This is based on escaped-zero strings.
#if defined(_WIN32)
    unsigned int stringLength= strlen((const char *)tokenCharArray);
#else
    unsigned int stringLength= strlen(tokenCharArray);
#endif
    unsigned int i, j;
    unifiedTokens= tokenCharArray;

    for (j= i= 0; i < stringLength; j++, i++) {
	if (tokenCharArray[i] == (unsigned char)0x088) {
	    switch(tokenCharArray[i+1]) {
		case (unsigned char)0x01:
		    unifiedTokens[j]= '\0';
		    break;
		case (unsigned char)0x088:
		    unifiedTokens[j]= (unsigned char)0x088;
		    break;
	    }
	    i++;
	}
	else {
	    unifiedTokens[j]= tokenCharArray[i];
	}
    }

	// Pop the values from the stack.
	// TMP: This does not work for values which are not 32 bits long !!
    if (stackIn > 0) {
	JVRunValue tmpStackedValues;

	inValues= new void *[stackIn];
	for (int i= stackIn-1; i > -1;) {
		// TMPTMP: To test, we do a direct stack access...
	    machine->operStack->pop(tmpStackedValues);
	    if (tmpStackedValues.getType() != TYPE_INSTANCE) {
		inValues[i]= (JVInstance *)tmpStackedValues.extractInteger();
	    }
	    else {
		// TMP: For now, we only accept string instances.
		inValues[i]= tmpStackedValues.extractInstance();
	    }
	    i--;
	}
    }

    if ((execFrame= sqlPipe->parseTokens(unifiedTokens, j, tokensStr)) != NULL) {
	if (sqlPipe->execute(execFrame, inValues, stackIn) >  -1) {
	    // Push the result from the command on the stack.
	    if (execFrame->getReplyWidth() == stackOut) {
		if (stackOut > 0) {
		    SqlReplyValue *reply;
		    int i;
		    JVRunValue tmpValue(TYPE_INSTANCE);
	    
		    for (i= stackOut-1; i > -1;) {
			reply= execFrame->getReply(i);
			switch(reply->getType()) {
			    case SqlReplyValue::tChar:
			    case SqlReplyValue::tVarchar:
				tmpValue.setValue(machine->createStringFromNarrowArray(
						reply->getAsString()
				));
				machine->operStack->push(tmpValue);
				break;
			    case SqlReplyValue::tInt:
				machine->operStack->pushInteger(reply->getAsInteger());
				break;
			    case SqlReplyValue::tReal:
				machine->operStack->pushInteger(reply->getAsInteger());
				break;
			}
			i--;
		    }
		}
		callFrame->returnInteger(1);
	    }
	    else  callFrame->returnInteger(0);
	}
	else callFrame->returnInteger(0);
    }
    else {
	callFrame->returnInteger(0);
    }

    if (stackIn > 0) delete[] inValues;
    delete[] tokenCharArray;

    return 0;
}


extern "C" {
void __pure_virtual(void)
{
// TMP HACK.
}
}
