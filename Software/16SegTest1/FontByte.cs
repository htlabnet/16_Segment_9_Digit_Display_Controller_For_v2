using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace _16SegControl
{
    struct FontByte
    {
        public byte Msb { get; }
        public byte Lsb { get; }
        public bool Dot { get; }

        public FontByte(byte msb, byte lsb, bool dot)
        {
            Msb = msb;
            Lsb = lsb;
            Dot = dot;
        }
    }
}
