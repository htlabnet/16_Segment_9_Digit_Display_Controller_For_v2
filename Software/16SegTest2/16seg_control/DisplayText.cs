using System;
using System.Collections.Generic;
using System.IO.Ports;
using System.Threading;
using System.Threading.Tasks;

namespace _16seg_control
{
    class DisplayText
    {
        // 表示するテキスト(複数)
        private List<char[]> Text;
        public bool IsLoop { get; set; }
        public bool UsePadding { get; set; }
        public int Delay { get; set; }
        private SerialPort _sp;
        private CancellationTokenSource _tokenSource;
        private Task _task;

        public DisplayText()
        {
            _tokenSource = new CancellationTokenSource();
            IsLoop = false;
            UsePadding = false;
            Delay = 0;
            Text = new List<char[]>();
            _sp = null;
        }

        public DisplayText(SerialPort com) : this()
        {
            _sp = com;
        }
        
        public void SetSerialPort(SerialPort com)
        {
            _sp = com;
        }

        public void AddText(string s)
        {
            Text.Add(s.ToCharArray());
        }

        public void AddText(char[] c)
        {
            Text.Add(c);
        }

        public List<SegChar[]> ToSegCharList()
        {
            List<SegChar[]> list = new List<SegChar[]>();
            foreach (var item in Text)
            {
                list.Add(DisplayUtil.PackInputText(item).ToArray());
            }
            return list;
        }

        public void StopDisplay()
        {
            //タスクを止める
            if (_tokenSource != null)
            {
                _tokenSource.Cancel(true);
                _task.Wait();
            }
        }

        public void ShowDisplay()
        {
            if (_sp == null)
            {
                throw new InvalidOperationException("COMPort not set.");
            }
            _task = Task.Factory.StartNew(() =>
            {
                _sp.Open();
                do
                {
                    try
                    {
                        _tokenSource.Token.ThrowIfCancellationRequested();
                    }
                    catch (OperationCanceledException)
                    {
                        Console.WriteLine("Cancel Interrupt");
                        return;
                    }
                    foreach (var item in Text)
                    {
                        try
                        {
                            _tokenSource.Token.ThrowIfCancellationRequested();
                        }
                        catch (OperationCanceledException)
                        {
                            Console.WriteLine("Cancel Interrupt");
                        }
                        DisplayUtil.send_16seg(DisplayUtil.PackInputText(item).ToArray(), UsePadding, _sp);
                        Thread.Sleep(Delay);
                    }
                } while (IsLoop);
            }, _tokenSource.Token).ContinueWith(t => { _tokenSource.Dispose(); _tokenSource = null; });
            _sp.Close();
        }
    }
}
