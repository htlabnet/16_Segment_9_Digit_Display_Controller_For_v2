using System;
using System.Collections.Generic;
using System.Text;
using _16SegController_UWP.Core.Models;
using _16SegController_UWP.Core.Models.Segment;

namespace _16SegControllerCli
{
    class MainClass
    {
        public MainClass()
        {
            LoadHidDevice();

        }

        private void LoadHidDevice()
        {
            var hidDeviceManager = new HidDeviceManager();
            if (hidDeviceManager.OpenHidDevice(0x4d8, 0x3f))
            {
                Console.WriteLine("HID Device Open.");
            }


            var display = new Display();
            var singleChar = display.BuildDisplayChar(0, new SegChar('A'));
            
            Console.WriteLine("Show 1 Digit...");
            hidDeviceManager.WriteByteHidDevice(singleChar, false);
            Console.WriteLine("Next 9 Digit Test... Press any key.");
            Console.ReadKey();

            var multiChar = display.BuildDisplay(new List<SegChar>()
            {
                new SegChar(' ', true),
                new SegChar('N'),
                new SegChar('E'),
                new SegChar('T'),
                new SegChar(' '),
                new SegChar('C'),
                new SegChar('O'),
                new SegChar('R'),
                new SegChar('E')
            });
            Console.WriteLine("Show 9 Digit...");
            hidDeviceManager.WriteByteHidDevice(multiChar, false);
            Console.WriteLine("Test end. Press any key.");
            Console.ReadKey();
        }
    }
}
