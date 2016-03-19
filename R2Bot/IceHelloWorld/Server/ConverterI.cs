using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace IceHelloworld
{
    public class ConverterI : Example.ConverterDisp_
    {
        public override string toUpper(string s, Ice.Current c)
        {
            return s.ToUpper();
        }
    }
}
