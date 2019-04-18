using System;
using System.Collections.Generic;
using System.Linq;
using _16SegController_UWP.Core.Models.Segment;

namespace _16SegController_UWP.Core.Models
{
    public class Display
    {
        public enum DisplayType
        {
            Hid,
            Serial
        }
        public readonly int MaxDisplaySize = 9;
        private readonly DisplayType _displayType;


        public Display(DisplayType type = DisplayType.Hid)
        {
            _displayType = type;
        }

        public byte[] BuildDisplayChar(int index, SegChar displayChar)
        {
            byte[] ret = null;
            if (index > MaxDisplaySize) return null;
            switch (_displayType)
            {
                case DisplayType.Hid:
                    var hidList = new List<byte>
                    {
                        0x00,
                        0x10,
                        (byte) (index),
                        (byte) displayChar.Char,
                        (byte) (displayChar.IsDot ? 0x1 : 0x0)
                    };
                    ret = hidList.ToArray();
                    break;
                case DisplayType.Serial:
                    byte[] digitStandard = new byte[9] { 0xE0, 0xE1, 0xE2, 0xE3, 0xE4, 0xE5, 0xE6, 0xE7, 0xE8 };
                    byte[] digitWithDots = new byte[9] { 0xF0, 0xF1, 0xF2, 0xF3, 0xF4, 0xF5, 0xF6, 0xF7, 0xF8 };
                    var serialList = new List<byte>
                    {
                        displayChar.IsDot ? digitWithDots[index] : digitStandard[index],
                        (byte) displayChar.Char
                    };
                    ret = serialList.ToArray();
                    break;
            }
            return ret;
        }

        public byte[] BuildDisplay(List<SegChar> displaySegList)
        {
            if (_displayType != DisplayType.Hid)
            {
                throw new InvalidOperationException("This method HID device only.");
            }
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
