#ifndef _SQL_ENGINE_H_
#define _SQL_ENGINE_H_
/**************************************************
* File: sqlEngine.h.
* Desc: Definition of the SQL statement executor.
* Module: AkraLog : SqlSupport.
* Rev: 27 mai 1998 : REV 0 : Hugo DesRosiers : Creation.
**************************************************/

#include <akra/AkObject.h>
#include <akra/portableDefs.h>


class SqlExecFrame;


class ClassExport SqlEngine : public AkObject {
  protected:

  public:
    SqlEngine(void);
    virtual ~SqlEngine(void);
    virtual int execute(SqlExecFrame *frame, void **suppValues);
};



#endif	/* _SQL_ENGINE_H_ */
