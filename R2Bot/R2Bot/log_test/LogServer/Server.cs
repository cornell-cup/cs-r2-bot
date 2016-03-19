using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace LogServer
{
    class Server
    {
        public static void createServer(String proxyName)
        {
            try
            {
                Ice.Communicator communicator = Ice.Util.initialize();
                Ice.ObjectAdapter adapter = communicator.createObjectAdapterWithEndpoints(
                   "proxyName", "tcp -p 10003");
                adapter.add(new RemoteLogI(), communicator.stringToIdentity(proxyName));
                adapter.activate();
                communicator.waitForShutdown();
                communicator.destroy();
            }
            catch (System.Exception ex)
            {
                System.Console.Error.WriteLine(ex);
                System.Environment.Exit(1);
            }
        }
    }
}
