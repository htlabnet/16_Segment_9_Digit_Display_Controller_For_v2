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
        public void OpenTest()
        {
            // モックのデバイスをつくってクエリできるか調べる
            var mockHidList = new MockHidList();
            // モックデバイスを追加
            mockHidList.AddDevice(0x4d8, 0x3f);
            
            var hidDeviceManager = new HidDeviceManager(mockHidList);
            
            // リストにあるデバイスを開こうとする
            Assert.IsTrue(hidDeviceManager.OpenHidDevice(0x4d8, 0x3f));
            // リストに無いデバイスを開こうとする
            Assert.IsFalse(hidDeviceManager.OpenHidDevice(0x4d8, 0x3d));
        }

        [TestMethod]
        public void WriteReadTest()
        {
            // サンプルデータ
            var sample = new byte[] {0xDE, 0xAD, 0xBE, 0xEF};
            
            // モックデバイスを作成する
            var mockHidList = new MockHidList();
            mockHidList.AddDevice(0x4d8, 0x3f);
            var hidDeviceManager = new HidDeviceManager(mockHidList);

            // Openする前に書き込もうとするとnullになる
            Assert.AreEqual(null, hidDeviceManager.WriteAndReadByteHidDevice(sample));
            // 開いてから書き込んで読み込むと同じ結果が返ってくる
            hidDeviceManager.OpenHidDevice(0x4d8, 0x3f);
            var ret = hidDeviceManager.WriteAndReadByteHidDevice(sample);
            CollectionAssert.AreEqual(ret, sample);
        }
    }
}
