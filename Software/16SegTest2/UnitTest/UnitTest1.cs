using System;
using _16seg_control;
using Microsoft.VisualStudio.TestTools.UnitTesting;

namespace UnitTest
{
    [TestClass]
    public class UnitTest1
    {
        [TestMethod]
        public void InputTextPackTest()
        {
            int test = 1;
            SegChar[] a1 = { new SegChar('a', false) };
            PackTest("a", a1, test++);

            SegChar[] a2 = {new SegChar(' ', true), new SegChar('A', false) };
            PackTest(".A", a2, test++);

            SegChar[] a3 = { new SegChar('A', true) };
            PackTest("A.", a3, test++);

            SegChar[] a4 = { new SegChar('A', true), new SegChar('B', true), new SegChar('C', false) };
            PackTest("A.B.C", a4, test++);

            SegChar[] a5 = { new SegChar('A', true), new SegChar('B', true), new SegChar('C', true) };
            PackTest("A.B.C.", a5, test++);

            SegChar[] a6 = {new SegChar(' ', true), new SegChar(' ', true),
                            new SegChar('A', true), new SegChar('B', true),
                            new SegChar(' ', true) };
            PackTest("..A.B..", a6, test++);
        }

        private void PackTest(string ex, SegChar[] actual, int test_no)
        {
            var except = DisplayUtil.PackInputText(ex.ToCharArray()).ToArray();
            Assert.AreEqual(except.Length, actual.Length, string.Format("Test {0}: Length Error", test_no));
            for (int i = 0; i < except.Length; i++)
            {
                Assert.AreEqual(except[i].Char, actual[i].Char, string.Format("Test {0}: Invalid data", test_no));
                Assert.AreEqual(except[i].isDot, actual[i].isDot, string.Format("Test {0}: Invalid flag", test_no));
            }
        }
    }
}
