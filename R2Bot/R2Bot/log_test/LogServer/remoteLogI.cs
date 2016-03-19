using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.IO;
using Logging;

namespace LogServer
{
    class RemoteLogI : Logging.RemoteLogDisp_
    {
        public override void log(String name, string time, String msg, Ice.Current x)
        {
            string filename = "Log.txt";
            string path = Path.GetFullPath(filename);

            if (!File.Exists(path))
            {
                using (StreamWriter sw = File.CreateText(path))
                {
                    sw.WriteLine(name + " " + time + " " + msg);
                }
            }
            else
            {
                using (StreamWriter sw = File.AppendText(path))
                    sw.WriteLine(name + " " + time + " " + msg);
            }
            Console.Out.WriteLine("Log is logged");
        }
    }
}
