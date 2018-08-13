using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace _16SegControl
{
    public class SegChar
    {
        public bool IsDot { get; set; }
        public char Char { get; set; }

        public SegChar()
        {
            IsDot = false;
        }
        public SegChar(char c) : this(c, false)
        {
        }

        public SegChar(char c, bool dot) : this()
        {
            IsDot = dot;
            Char = c;
        }
    }
}
