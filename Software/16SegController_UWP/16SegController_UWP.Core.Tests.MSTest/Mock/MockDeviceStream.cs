using System;
using System.Collections.Generic;
using System.IO;
using System.Text;
using HidSharp;

namespace _16SegController_UWP.Core.Tests.MSTest.Mock
{
    /// <summary>
    /// HIDSharpのHidStreamのモック
    /// HIDとの通信はMemoryStreamに置き換え、Writeされた物をReadするようにした
    /// </summary>
    class MockDeviceStream : HidStream
    {
        private readonly MemoryStream _memoryStream;
        public MockDeviceStream(HidDevice device) : base(device)
        {
            _memoryStream = new MemoryStream();
        }

        public override void Flush()
        {
            _memoryStream.Flush();
        }

        public override void GetFeature(byte[] buffer, int offset, int count)
        {
            throw new NotImplementedException();
        }

        public override void SetFeature(byte[] buffer, int offset, int count)
        {
            throw new NotImplementedException();
        }

        public override int Read(byte[] buffer, int offset, int count)
        {
            // Seekしないと取得できない……
            _memoryStream.Seek(offset, SeekOrigin.Begin); 
            return _memoryStream.Read(buffer, offset, count);
        }

        public override void Write(byte[] buffer, int offset, int count)
        {
            _memoryStream.Write(buffer, offset, count);
        }

        public override int ReadTimeout { get; set; }
        public override int WriteTimeout { get; set; }
    }
}
