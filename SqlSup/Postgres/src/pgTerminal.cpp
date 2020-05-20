/**************************************************
* File: pgTerminal.cc.
* Desc: Definition of the PostgresTerminal class.
* Module: AkraLog : SqlSupport.
* Rev: 23 mai 1998 : REV 0 : Hugo DesRosiers : Creation.
**************************************************/

extern "C" {
#include <stdio.h>
#include <libpq-fe.h>
}

#include "sqlDataPipe.h"
#include "pgTerminal.h"


PostgresTerminal::PostgresTerminal(JVInstance *anInstance)
{
    connection= NULL;
    dual= anInstance;
}


int PostgresTerminal::connectToServer(char **arguments, unsigned int nbrArguments)
{
    char *connArgs[5]= { NULL, NULL, NULL, NULL, NULL };

    for (unsigned int i= 0; (i < 5) && (i < nbrArguments); i++) {
	connArgs[i]= arguments[i];
    }
	// Format: host, port, options, debug tty, universe (~database).
    connection= PQsetdb(connArgs[0], connArgs[1], connArgs[2], connArgs[3], connArgs[4]);
    if (PQstatus(connection) == CONNECTION_BAD) {
	PQfinish(connection);
	return -1;		// Warning: quick exit (No connection).
    }
    else {
	host= PQhost(connection);
	options= PQoptions(connection);
	port= PQport(connection);
	pgTty= PQtty(connection);
	database= PQdb(connection);
    }

    return 0;
}


boolean PostgresTerminal::disconnect(void)
{
    PQfinish(connection);
    connection= NULL;
    return true;			// No way to find out if it didn't disconnect ?
}


boolean PostgresTerminal::hasConnection(void)
{
    return (connection != NULL);
}


SqlDataPipe *PostgresTerminal::createPipe(void)
{
    return new SqlDataPipe(NULL);
}


PGconn *PostgresTerminal::getConnection(void)
{
    return connection;
}

