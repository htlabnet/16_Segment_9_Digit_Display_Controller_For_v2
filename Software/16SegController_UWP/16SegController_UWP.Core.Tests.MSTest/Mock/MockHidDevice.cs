using System;
using System.Collections.Generic;
using System.Text;
using HidSharp;

namespace _16SegController_UWP.Core.Tests.MSTest.Mock
{
    class MockHidDevice : HidDevice
    {
        public MockHidDevice(int vendorId, int productId)
        {
            ProductID = productId;
            VendorID = vendorId;
        }

        protected override DeviceStream OpenDeviceDirectly(OpenConfiguration openConfig)
        {
            throw new NotImplementedException();
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
            throw new NotImplementedException();
        }

        public override int GetMaxOutputReportLength()
        {
            throw new NotImplementedException();
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
