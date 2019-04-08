using System;
using _16SegController_UWP.Core.Models;
using _16SegController_UWP.Core.Tests.MSTest.Mock;
using Microsoft.VisualStudio.TestTools.UnitTesting;

namespace _16SegController_UWP.Core.Tests.MSTest
{
    [TestClass]
    public class HidDeviceManagerTest
    {
        [TestMethod]
        public void QueryTest()
        {
            var mockHidList = new MockHidList();
            mockHidList.AddDevice(0x4d8, 0x3f);
            var hidDeviceManager = new HidDeviceManager(mockHidList);
            Assert.IsTrue(hidDeviceManager.QueryHidDevice(0x4d8, 0x3f));
            Assert.IsFalse(hidDeviceManager.QueryHidDevice(0x4d8, 0x3d));
        }
    }
}
