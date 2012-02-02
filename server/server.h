#ifndef __SERVER_H__
#define __SERVER_H__

#include <Ice/Ice.h>


class FleCSServer : public Ice::Application
{
public:
	FleCSServer();

    virtual int run(int, char*[]);
};


#endif
