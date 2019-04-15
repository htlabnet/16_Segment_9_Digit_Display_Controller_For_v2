using System.Collections.Generic;
using System.Linq;
using _16SegController_UWP.Core.Models.Segment;

namespace _16SegController_UWP.Core.Models.Display
{
    public class DisplayBase
    {
        public readonly int MaxDisplaySize = 9;

        public byte[] BuildDisplayChar(int index, SegChar displayChar)
        {
            if (index > MaxDisplaySize) return null;
            var buffer = new List<byte>
            {
                0x00,
                0x10,
                (byte) (index),
                (byte) displayChar.Char,
                (byte) (displayChar.IsDot ? 0x1 : 0x0)
            };

            return buffer.ToArray();
        }

        public byte[] BuildDisplay(List<SegChar> displaySegList)
        {
            var dots = new List<int>();

            if (displaySegList.Count > MaxDisplaySize) return null;
            var buffer = new List<byte>
            {
                0x00,
                0x1F
            };
            foreach (var seg in displaySegList)
            {
                buffer.Add((byte) seg.Char);
                dots.Add(seg.IsDot ? 0x01 : 0x00);
            }

            if (displaySegList.Count != MaxDisplaySize)
            {
                dots.AddRange(Enumerable.Repeat(0, MaxDisplaySize - displaySegList.Count));
                buffer.AddRange(Enumerable.Repeat((byte) 0x00, (MaxDisplaySize + 1) - buffer.Count));
            }

            buffer.AddRange(LedDotToBytes(dots.ToArray()));
            return buffer.ToArray();
        }

        private byte[] LedDotToBytes(int[] flags)
        {
            byte dot1 = 0x00;
            byte dot2 = 0x00;
            for (var i = 0; i < 9; i++)
            {
                if (flags[i] == 1) dot1 = (byte) ((1 << i) | dot1);
                if (i >= 8 && (flags[i] == 1)) dot2 = (byte) ((1 << i - 8) | dot2);
            }
            return new[] { dot1, dot2 };
        }
    }
}
