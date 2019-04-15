using System.Collections.Generic;
using _16SegController_UWP.Core.Models;
using _16SegController_UWP.Core.Models.Display;
using _16SegController_UWP.Core.Models.Segment;
using _16SegController_UWP.Core.Tests.MSTest.Mock;
using Microsoft.VisualStudio.TestTools.UnitTesting;

namespace _16SegController_UWP.Core.Tests.MSTest
{
    [TestClass]
    public class HidDisplayTest
    {
        [TestMethod]
        public void CharWriteTest()
        {
            // サンプルデータ
            var sample = new byte[] {0x00, 0x10, 0x00, 0x61, 0x00};
            
            // モックデバイスを作成する
            var mockHidList = new MockHidList();
            mockHidList.AddDevice(0x4d8, 0x3f);
            var hidDeviceManager = new HidDeviceManager(mockHidList);


            // 書き込むデータを作成する
            var display = new DisplayBase();
            var data = display.BuildDisplayChar(0, new SegChar('a'));
            // 開いてから書き込んで読み込むと同じ結果が返ってくる
            hidDeviceManager.OpenHidDevice(0x4d8, 0x3f);
            var ret = hidDeviceManager.WriteAndReadByteHidDevice(data);
            CollectionAssert.AreEqual(ret, sample);
        }

        [TestMethod]
        public void DisplayWriteTest()
        {
            // サンプルデータ
            var sample = new byte[]
            {
                0x00,
                0x1F,
                0x41,
                0x42,
                0x43,
                0x44,
                0x45,
                0x46,
                0x47,
                0x48,
                0x49,
                0xAA,
                0x00
            };
            
            // モックデバイスを作成する
            var mockHidList = new MockHidList();
            mockHidList.AddDevice(0x4d8, 0x3f);
            var hidDeviceManager = new HidDeviceManager(mockHidList);

            // 書き込むデータを作成する
            var display = new DisplayBase();
            var data = display.BuildDisplay(new List<SegChar>()
            {
                new SegChar('A'),
                new SegChar('B', true),
                new SegChar('C'),
                new SegChar('D', true),
                new SegChar('E'),
                new SegChar('F', true),
                new SegChar('G'),
                new SegChar('H', true),
                new SegChar('I')
            });
            // 開いてから書き込んで読み込むと同じ結果が返ってくる
            hidDeviceManager.OpenHidDevice(0x4d8, 0x3f);
            var ret = hidDeviceManager.WriteAndReadByteHidDevice(data);
            CollectionAssert.AreEqual(ret, sample);
        }
    }
}
