using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using HidSharp;

namespace _16SegController_UWP.Core.Models
{
    public class HidDeviceManager
    {
        private readonly DeviceList _deviceList;
        private HidDevice _hidDevice;

        public HidDeviceManager(DeviceList deviceList = null)
        {
            _deviceList = deviceList ?? DeviceList.Local;
        }

        public bool QueryHidDevice(int vendorId, int productId)
        {
            try
            {
                _hidDevice = _deviceList.GetHidDevices()
                    .First(x => (x.VendorID == vendorId && x.ProductID == productId));
            }
            catch (InvalidOperationException)
            {
                return false;
            }

            return true;
        }

        public byte[] WriteAndReadByteHidDevice(byte[] dataBytes)
        {
            byte[] readBytes = null;
            if (_hidDevice == null) return null;
            if (_hidDevice.TryOpen(out var hidStream))
            {
                using (hidStream)
                {
                    hidStream.Write(dataBytes);
                    readBytes = hidStream.Read();

                }
            }

            return readBytes;
        }
     }
}
