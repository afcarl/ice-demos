// **********************************************************************
//
// Copyright (c) 2003-2015 ZeroC, Inc. All rights reserved.
//
// **********************************************************************

#include <Ice/Ice.h>
#include <HelloI.h>

using namespace std;

class HelloServer : public Ice::Application
{
public:

    virtual int run(int, char*[]);
};

#ifdef ICE_STATIC_LIBS
extern "C"
{

Ice::Plugin* createIceSSL(const Ice::CommunicatorPtr&, const string&, const Ice::StringSeq&);

}
#endif

int
main(int argc, char* argv[])
{
#ifdef ICE_STATIC_LIBS
    Ice::registerPluginFactory("IceSSL", createIceSSL, false);
#endif
    HelloServer app;
    return app.main(argc, argv, "config.server");
}

int
HelloServer::run(int argc, char*[])
{
    if(argc > 1)
    {
        cerr << appName() << ": too many arguments" << endl;
        return EXIT_FAILURE;
    }

    //
    // Create the 'Hello' adapter and register it with the Locator registry.
    //
    Ice::ObjectAdapterPtr adapter = communicator()->createObjectAdapter("Hello");
    Ice::LocatorRegistryPrx registry = communicator()->getDefaultLocator()->getRegistry();
    registry->setAdapterDirectProxy("Hello", adapter->createDirectProxy(communicator()->stringToIdentity("dummy")));

    Demo::HelloPtr hello = new HelloI;
    adapter->add(hello, communicator()->stringToIdentity("hello"));
    adapter->activate();

    communicator()->waitForShutdown();
    return EXIT_SUCCESS;
}