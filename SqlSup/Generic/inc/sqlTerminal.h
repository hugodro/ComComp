#ifndef _SQL_DATA_TERMINAL_H_
#define _SQL_DATA_TERMINAL_H_
/**************************************************
* File: sqlDataPipe.h.
* Desc: Definition of the SqlDataPipe class.
* Module: AkraLog : SqlSupport.
* Rev: 23 mai 1998 : REV 0 : Hugo DesRosiers : Creation.
**************************************************/

#include <akra/AkObject.h>
#include <akra/portableDefs.h>

class SqlDataPipe;


class ClassExport SqlDataTerminal : public AkObject {
  protected:

  public:
    SqlDataTerminal(void);
    virtual int connectToServer(char **arguments, unsigned int nbrArguments);
    virtual bool disconnect(void);
    virtual bool hasConnection(void);
    virtual SqlDataPipe *createPipe(void);
};


#endif		/* _SQL_DATA_TERMINAL_H_ */
