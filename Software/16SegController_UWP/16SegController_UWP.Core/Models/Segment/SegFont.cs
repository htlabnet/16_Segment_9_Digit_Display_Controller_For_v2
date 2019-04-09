using System;
using System.Collections.Generic;
using System.Text;

namespace _16SegController_UWP.Core.Models.Segment
{
    public class SegFont : SegBase, IEquatable<SegFont>
    {
        public bool A1 { get; set; }
        public bool A2 { get; set; }
        public bool B { get; set; }
        public bool C { get; set; }
        public bool D1 { get; set; }
        public bool D2 { get; set; }
        public bool E { get; set; }
        public bool F { get; set; }
        public bool G1 { get; set; }
        public bool G2 { get; set; }
        public bool H { get; set; }
        public bool I { get; set; }
        public bool J { get; set; }
        public bool K { get; set; }
        public bool L { get; set; }
        public bool M { get; set; }
        public bool DP { get; set; }

        public SegFont()
        {
            this.A1 = false;
            this.A2 = false;
            this.B = false;
            this.C = false;
            this.D1 = false;
            this.D2 = false;
            this.E = false;
            this.F = false;
            this.G1 = false;
            this.G2 = false;
            this.H = false;
            this.I = false;
            this.J = false;
            this.K = false;
            this.L = false;
            this.M = false;
            this.DP = false;
        }

        public bool Equals(SegFont other)
        {
            if (ReferenceEquals(null, other)) return false;
            if (ReferenceEquals(this, other)) return true;
            return A1 == other.A1 && A2 == other.A2 && B == other.B && C == other.C && D1 == other.D1 && D2 == other.D2 && E == other.E && F == other.F && G1 == other.G1 && G2 == other.G2 && H == other.H && I == other.I && J == other.J && K == other.K && L == other.L && M == other.M && DP == other.DP;
        }

        public override bool Equals(object obj)
        {
            if (ReferenceEquals(null, obj)) return false;
            if (ReferenceEquals(this, obj)) return true;
            if (obj.GetType() != this.GetType()) return false;
            return Equals((SegFont) obj);
        }

        public override int GetHashCode()
        {
            unchecked
            {
                var hashCode = A1.GetHashCode();
                hashCode = (hashCode * 397) ^ A2.GetHashCode();
                hashCode = (hashCode * 397) ^ B.GetHashCode();
                hashCode = (hashCode * 397) ^ C.GetHashCode();
                hashCode = (hashCode * 397) ^ D1.GetHashCode();
                hashCode = (hashCode * 397) ^ D2.GetHashCode();
                hashCode = (hashCode * 397) ^ E.GetHashCode();
                hashCode = (hashCode * 397) ^ F.GetHashCode();
                hashCode = (hashCode * 397) ^ G1.GetHashCode();
                hashCode = (hashCode * 397) ^ G2.GetHashCode();
                hashCode = (hashCode * 397) ^ H.GetHashCode();
                hashCode = (hashCode * 397) ^ I.GetHashCode();
                hashCode = (hashCode * 397) ^ J.GetHashCode();
                hashCode = (hashCode * 397) ^ K.GetHashCode();
                hashCode = (hashCode * 397) ^ L.GetHashCode();
                hashCode = (hashCode * 397) ^ M.GetHashCode();
                hashCode = (hashCode * 397) ^ DP.GetHashCode();
                return hashCode;
            }
        }
    }
}
