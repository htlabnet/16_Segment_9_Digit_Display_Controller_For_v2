using System;
using System.Collections.Generic;
using System.Text;
using _16SegController_UWP.Core.Models.Segment;
using Microsoft.VisualStudio.TestTools.UnitTesting;

namespace _16SegController_UWP.Core.Tests.MSTest
{
    [TestClass]
    public class SegFontTest
    {
        [TestMethod]
        public void ConstructTest()
        {
            var except = new SegFont()
            {
                A1 = true
            };
            var actual = new SegFont()
            {
                A1 = true
            };
            Assert.AreEqual(except, actual);
            except = new SegFont()
            {
                A1 = false
            };
            Assert.AreNotEqual(except, actual);
        }

        [TestMethod]
        public void HashCodeTest()
        {
            var except = new SegFont()
            {
                A1 = true
            };
            var actual = new SegFont()
            {
                A1 = true
            };
            Assert.AreEqual(except.GetHashCode(), actual.GetHashCode());

            except = new SegFont()
            {
                A1 = true,
                A2 = true
            };
            Assert.AreNotEqual(except.GetHashCode(), actual.GetHashCode());
        }

        [TestMethod]
        public void EqualTest()
        {
            var actual = new SegFont()
            {
                A1 = true
            };
            var test = 1;
            Assert.IsFalse(actual.Equals(null));
            Assert.IsTrue(actual.Equals(actual));
            Assert.IsFalse(actual.Equals((object)null));
            Assert.IsFalse(actual.Equals((object)test));
            Assert.IsTrue(actual.Equals((object)actual));
        }
    }
}
