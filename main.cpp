
#include "Server.hpp"
#include "HttpHandlerFabrica.hpp"
#include "IntervalStorage.hpp"

#include <iostream>

using namespace std;

int main(int argc, char* argv[])
{
    const size_t numThreads(4);
    const char*  port = "8080";
    const char*  netInterface = "127.0.0.1";

    try
    {
        // Initialise the server.
        IntervalStorage intervalStorage;
        Http::HttpHandlerFabrica httpHandlerFabrica(intervalStorage);
        Network::Server netSrv(netInterface, port, numThreads, httpHandlerFabrica);

        cout << "Server initialized.\nRun ...\n" << flush;

        netSrv.Run();
    }
    catch (exception& ex)
    {
        cerr << "Error: " << ex.what() << endl;
    }
    return 0;
}
