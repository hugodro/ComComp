#ifndef _SQL_POSTGRES_TERMINAL_H_
#define _SQL_POSTGRES_TERMINAL_H_
/**************************************************
* File: pgTerminal.h.
* Desc: Definition of the PostgresTerminal class.
* Module: AkraLog : SqlSupport.
* Rev: 23 mai 1998 : REV 0 : Hugo DesRosiers : Creation.
**************************************************/

#include "sqlTerminal.h"


struct PGconn;

class JVInstance;


class PostgresTerminal : public SqlDataTerminal {
  protected:
    JVInstance *dual;		// Our Java equivalent.
    PGconn *connection;
    char *host;
    char *options;
    char *port;
    char *pgTty;
    char *database;

  public:
    PostgresTerminal(JVInstance *anInstance);
    virtual int connectToServer(char **arguments, unsigned int nbrArguments);
    virtual boolean disconnect(void);
    virtual boolean hasConnection(void);
    virtual SqlDataPipe *createPipe(void);
    virtual PGconn *getConnection(void);
};

#endif		/* _SQL_POSTGRES_TERMINAL_H_ */

