using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Logging;

namespace Client
{
    class Client
    {
        public static RemoteLogPrx logStation;

        public static void createClient(string ip, string port, string proxyname, string name, string time, string msg)
        {
            try
            {
                Ice.Communicator communicator = Ice.Util.initialize();
                logStation = RemoteLogPrxHelper.checkedCast(
                    communicator.stringToProxy(String.Format("{0}:tcp -h {1} -p {2}", proxyname, ip, port)));
                System.Console.WriteLine("Connection Successful");

                Client.logStation.log(name, time, msg);
                while (true)
                {
                };
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