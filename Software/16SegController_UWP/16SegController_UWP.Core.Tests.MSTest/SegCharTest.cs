using System;
using System.Collections.Generic;
using System.Text;
using _16SegController_UWP.Core.Models.Segment;
using Microsoft.VisualStudio.TestTools.UnitTesting;

namespace _16SegController_UWP.Core.Tests.MSTest
{
    [TestClass]
    public class SegCharTest
    {
        [TestMethod]
        public void ConstructorTest()
        {
            var except = new SegChar('A');
            var actual = new SegChar('A', false);
            Assert.AreEqual(except, actual);

            except = new SegChar('A', true);
            Assert.AreNotEqual(except, actual);

            except = new SegChar('B');
            Assert.AreNotEqual(except, actual);
        }

        [TestMethod]
        public void HashCodeTest()
        {
            var except = new SegChar('A');
            var actual = new SegChar('A');
            Assert.AreEqual(except.GetHashCode(), actual.GetHashCode());

            except = new SegChar('A', true);
            Assert.AreNotEqual(except.GetHashCode(), actual.GetHashCode());
        }

        [TestMethod]
        public void EqualTest()
        {
            var actual = new SegChar('A');
            var test = 1;
            Assert.IsFalse(actual.Equals(null));
            Assert.IsTrue(actual.Equals(actual));
            Assert.IsFalse(actual.Equals((object)null));
            Assert.IsFalse(actual.Equals((object)test));
            Assert.IsTrue(actual.Equals((object)actual));
        }
    }
}
