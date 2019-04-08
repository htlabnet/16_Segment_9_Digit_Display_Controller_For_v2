using System;
using System.Collections.Generic;
using System.Text;
using HidSharp;

namespace _16SegController_UWP.Core.Tests.MSTest.Mock
{
    /// <summary>
    ///  HIDSHarpのDeviceListクラスのモック
    /// 最低限の実装
    /// </summary>
    class MockHidList : DeviceList
    {
        private List<HidDevice> _hidDevices = new List<HidDevice>();

        // テスト用に任意のモックデバイスを追加する
        public void AddDevice(int vendorId, int productId)
        {
            _hidDevices.Add(new MockHidDevice(vendorId, productId));
        }

        public override IEnumerable<Device> GetAllDevices()
        {
            return _hidDevices;
        }

        public override bool AreDriversBeingInstalled { get; }
    }
}
