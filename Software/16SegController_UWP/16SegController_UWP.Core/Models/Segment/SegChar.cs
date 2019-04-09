using System;
using System.Collections.Generic;
using System.Text;

namespace _16SegController_UWP.Core.Models.Segment
{
    public class SegChar : SegBase, IEquatable<SegChar>
    {
        public bool IsDot { get; private set; }
        public char Char { get; private set; }

        public SegChar(char c, bool dot = false)
        {
            IsDot = dot;
            Char = c;
        }

        public bool Equals(SegChar other)
        {
            if (ReferenceEquals(null, other)) return false;
            if (ReferenceEquals(this, other)) return true;
            return IsDot == other.IsDot && Char == other.Char;
        }

        public override bool Equals(object obj)
        {
            if (ReferenceEquals(null, obj)) return false;
            if (ReferenceEquals(this, obj)) return true;
            if (obj.GetType() != this.GetType()) return false;
            return Equals((SegChar) obj);
        }

        public override int GetHashCode()
        {
            unchecked
            {
                return (IsDot.GetHashCode() * 397) ^ Char.GetHashCode();
            }
        }
    }
}
