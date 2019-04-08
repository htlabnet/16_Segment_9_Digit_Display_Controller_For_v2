using System;
using System.Collections.Generic;
using System.Text;
using HidSharp;

namespace _16SegController_UWP.Core.Tests.MSTest.Mock
{
    /// <summary>
    ///  HIDSharpのHidDeviceクラスのモック
    /// とりあえず動けば良い
    /// </summary>
    class MockHidDevice : HidDevice
    {
        public MockHidDevice(int vendorId, int productId)
        {
            // モックに任意のVendorsとProductIDを注入する
            ProductID = productId;
            VendorID = vendorId;
        }

        protected override DeviceStream OpenDeviceDirectly(OpenConfiguration openConfig)
        {
            return new MockDeviceStream(this);
        }

        public override string GetFileSystemName()
        {
            throw new NotImplementedException();
        }

        public override string DevicePath { get; }
        public override string GetManufacturer()
        {
            throw new NotImplementedException();
        }

        public override string GetProductName()
        {
            throw new NotImplementedException();
        }

        public override string GetSerialNumber()
        {
            throw new NotImplementedException();
        }

        public override int GetMaxInputReportLength()
        {
            // 値は適当
            return 100;
        }

        public override int GetMaxOutputReportLength()
        {
            // 値は適当
            return 100;
        }

        public override int GetMaxFeatureReportLength()
        {
            throw new NotImplementedException();
        }

        public override int ProductID { get; }
        public override int ReleaseNumberBcd { get; }
        public override int VendorID { get; }
    }
}
