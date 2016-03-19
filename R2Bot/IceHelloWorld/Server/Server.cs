using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Example;

namespace IceHelloworld
{
    class Server
    {
        static void Main(string[] args)
        {
            try
            {
                Ice.Communicator communicator = Ice.Util.initialize();
                Ice.ObjectAdapter adapter = communicator.createObjectAdapterWithEndpoints(
                   "converter", "tcp -p 10000");
                adapter.add(new ConverterI(), communicator.stringToIdentity("converter"));
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
