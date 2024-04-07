#include <fcp++/client.hpp>

int main()
{
    fcp::Client client("Demo");

    client.Connect("127.0.0.1", 9481);

    client.Shutdown();

    client.Disconnect();
}