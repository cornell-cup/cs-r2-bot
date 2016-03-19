using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace LogClient
{
    public class Program
    {
        public static void Main(String[] args)
        {

            Client.Client.createClient("localhost", "10003", "Ping", "my", "5pm", "second log");

            //Client.Client.createClient("localhost", "10003", "Ping");
            //Client.Client.logStation.log("mycomputer", "6pm", "client_created");
        }
    }
}