/**************************************************
* File: sqlDataPipe.cc.
* Desc: Implementation of the SqlDataPipe class.
* Module: AkraLog : SqlSupport.
* Rev: 26 mai 1998 : REV 0 : Hugo DesRosiers : Creation.
**************************************************/

#include "sqlDataPipe.h"
#include "sqlTerminal.h"


/**************************************************
* Implementation: SqlDataTerminal.
**************************************************/

SqlDataTerminal::SqlDataTerminal(void)
{
}


int SqlDataTerminal::connectToServer(char **arguments, unsigned int nbrArguments)
{
    return 0;
}


bool SqlDataTerminal::disconnect(void)
{
    return true;
}


bool SqlDataTerminal::hasConnection(void)
{
    return false;
}


SqlDataPipe *SqlDataTerminal::createPipe(void)
{
    return new SqlDataPipe(NULL);
}

