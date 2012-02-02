#include "server.h"


int main(int argc, char* argv[])
{
    FleCSServer app;
    return app.main(argc, argv, "config.server");
}

// TODO: shutdownOnInterrupt()?
