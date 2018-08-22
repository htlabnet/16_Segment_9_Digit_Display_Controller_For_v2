using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace _16seg_control
{
    public class SegChar
    {
        public bool isDot { get; set; }
        public char Char { get; set; }

        public SegChar()
        {
            isDot = false;
        }
        public SegChar(char c) : this(c, false)
        {
        }

        public SegChar(char c, bool dot) : this()
        {
            isDot = dot;
            Char = c;
        }
    }
}
