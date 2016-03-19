using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Example;

namespace IceHelloworld
{
    class Client
    {
        static void Main(string[] args)
        {
            try
            {
                Ice.Communicator communicator = Ice.Util.initialize();
                ConverterPrx cvt = ConverterPrxHelper.checkedCast(communicator.stringToProxy("converter:tcp -p 10000 -h localhost"));
                string upper = cvt.toUpper("hello world");
                System.Console.WriteLine("Server returned: " + upper);
                while (true) ; //holds the window to show the console output

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