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

        public bool OpenHidDevice(int vendorId, int productId)
        {
            try
            {
                _hidDevice = _deviceList.GetHidDevices()
                    .First(x => (x.VendorID == vendorId && x.ProductID == productId));
            }
            catch (InvalidOperationException)
            {
                // 引数のデバイスが見つからない場合、First()で例外が出てくる
                return false;
            }

            return true;
        }


        // HIDデバイスに対して書き込んで読み込む
        public byte[] WriteByteHidDevice(byte[] dataBytes, bool withRead = true)
        {
            byte[] readBytes = null;
            if (_hidDevice == null) return null;
            if (_hidDevice.TryOpen(out var hidStream))
            {
                using (hidStream)
                {
                    hidStream.Write(dataBytes);
                    if (withRead)
                    {
                        readBytes = hidStream.Read();
                    }
                }
            }

            return readBytes;
        }
     }
}
