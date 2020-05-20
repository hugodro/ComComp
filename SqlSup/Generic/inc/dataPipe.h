#ifndef _SQLDATAPIPE_H_
#define _SQLDATAPIPE_H_
/**************************************************
* File: sqlDataPipe.h.
* Desc: Definition of the SqlDataPipe class.
* Module: AkraLog : SqlSupport.
* Rev: 23 mai 1998 : REV 0 : Hugo DesRosiers : Creation.
**************************************************/



class ClassExport SqlDataPipe : public DataPipeGen {
  public:
    virtual SqlReplyValue **makeReplyArray(unsigned int aSize);
};

#endif    /* _GENERIC_DATA_PIPE_H_ */
