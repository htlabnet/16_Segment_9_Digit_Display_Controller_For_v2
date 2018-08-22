using System;
using System.Text.RegularExpressions;
using System.Threading;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.IO.Ports;
using System.Collections.Generic;
using System.Linq;
using System.Management;

namespace _16seg_control
{
    public partial class MainForm : Form
    {
        const int DISPLAY_DIGITS = 9;

        delegate void ProgressWriteDelegate(string text);

        public MainForm()
        {
            InitializeComponent();
        }

        private CancellationTokenSource _tokenSource;
        private Task _task;
        private int _scrollSpeed;

        private void Form1_Load(object sender, EventArgs e)
        {
            //インタフェース設定
            lbl_nowtext.Text = "";
            lbl_ledtext.Text = "";
            lbl_COMName.Text = "";

            //シリアルポート
            serialPort1.BaudRate = 9600;
            serialPort1.ReadTimeout = 500;
            serialPort1.WriteTimeout = 500;

            //シリアルポートの列挙
            cbox_port.Items.AddRange(SerialPort.GetPortNames());
            if (cbox_port.Items.Count == 1)
                cbox_port.SelectedIndex = 0;
        }

        private void btn_sendtext_Click(object sender, EventArgs e)
        {
            CancelAndWaitTask();
            send_led(text_box.Text, !cb_replace.Checked);
        }

        private void btn_delete_Click(object sender, EventArgs e)
        {
            CancelAndWaitTask();

            //空白を送る
            send_led(" ");

            lbl_nowtext.Text = "";
            lbl_ledtext.Text = "";
        }

        private void CancelAndWaitTask()
        {
            //タスクを止める
            if (_tokenSource != null)
            {
                _tokenSource.Cancel(true);
                _task.Wait();
            }
        }

        private void cbox_port_DropDown(object sender, EventArgs e)
        {
            string[] ports = SerialPort.GetPortNames();
            List<string> oldPorts = new List<string>();
            foreach (var item in cbox_port.Items)
            {
                oldPorts.Add(item.ToString());
            }
            // 新規のポート
            var newPorts = ports.Except(oldPorts).ToArray();
            // 削除されたポート
            var deletePorts = oldPorts.Except(ports).ToArray();
            foreach (var item in deletePorts)
            {
                cbox_port.Items.Remove(item);
            }
            foreach (var item in newPorts)
            {
                cbox_port.Items.Add(item);
            }
        }

        private void progress_write(string text)
        {
            lbl_ledtext.Text = text;
        }

        private void send_led(string text, bool replace = true)
        {
            //表示可能な文字列に整形する
            if (replace)
            {
                Regex re = new Regex(@"[^!%()*+-/<>A-Z0-9]");
                text = (re.Replace(text.ToUpper(), " "));
            }

            //フォームに反映
            lbl_nowtext.Text = text;
            Refresh();

            SerialOpen();

            if (_tokenSource == null) _tokenSource = new CancellationTokenSource();

            _task = Task.Factory.StartNew(() =>
            {
                while (true)
                {
                    Console.WriteLine("while loop");
                    // TaskのCancelを拾う
                    // Cancelを拾うとOperationCanceledExceptionが発生する
                    try
                    {
                        _tokenSource.Token.ThrowIfCancellationRequested();
                    }
                    catch (OperationCanceledException)
                    {
                        Console.WriteLine("Cancel Interrupt");
                        return true;
                    }

                    // パディングの作成
                    SegChar[] segPad = new SegChar[DISPLAY_DIGITS];
                    for (int i = 0; i < segPad.Length; i++)
                    {
                        segPad[i] = new SegChar(' ');
                    }
                    // 入力されたTextのPack
                    var t = text.ToCharArray();
                    var segList = DisplayUtil.PackInputText(t);

                    // DISPLAY_DIGITS 文字以下はそのまま出していく
                    if (DISPLAY_DIGITS >= segList.ToArray().Length)
                    {
                        Console.WriteLine("send text: " + text);
                        Invoke(new ProgressWriteDelegate(progress_write), text);
                        DisplayUtil.send_16seg(segList.ToArray(), true, serialPort1);
                        return true;
                    }

                    // DISPLAY_DIGITS 文字以上の場合パディング+テキスト
                    List<SegChar> buf = new List<SegChar>();
                    buf.AddRange(segPad);
                    buf.AddRange(segList);

                    for (int i = 0; i <= buf.ToArray().Length; i++)
                    {
                        bool isShowExist = false;
                        // for文内でもTaskのCancelを拾う
                        try
                        {
                            _tokenSource.Token.ThrowIfCancellationRequested();
                        }
                        catch (OperationCanceledException)
                        {
                            Console.WriteLine("Cancel Interrupt");
                            return true;
                        }
                        if (i + DISPLAY_DIGITS < buf.ToArray().Length)
                        {
                            // ConvertAllにてList<T>に入っているSegCharクラスのメンバ変数を取ってくる
                            // List<string>で戻ってくるのでstring.join()で繋げる
                            if (isShowExist == false)
                            {
                                string msg = DisplayUtil.ThreadLoop_Send16Seg(buf, i, DISPLAY_DIGITS, serialPort1);
                                Invoke(new ProgressWriteDelegate(progress_write), msg);
                                isShowExist = true;
                            }
                            else // TODO: 確認出来たらelse以下をカット
                            {
                                Console.WriteLine("Skip...");
                            }
                        }
                        else
                        {
                            string msg = DisplayUtil.ThreadLoop_Send16Seg(buf, i, buf.ToArray().Length - i, serialPort1);
                            Invoke(new ProgressWriteDelegate(progress_write), msg);
                        }
                        Thread.Sleep(1000 - _scrollSpeed);
                    }
                }
            }, _tokenSource.Token).ContinueWith(thread_releease);
        }

        private void thread_releease(Task t)
        {
            // Taskが完了したあとの処理
            _tokenSource.Dispose();
            _tokenSource = null;

            if (serialPort1.IsOpen) serialPort1.Close();

            if (t.IsCanceled)
            {
                // TODO:キャンセルされたときの処理
                Console.WriteLine("Task cancelled.");
            }
            if (t.IsCompleted)
            {
                // TODO:タスク終了したときの処理
                Console.WriteLine("Task completed.");
            }
        }

        private void cb_replace_CheckedChanged(object sender, EventArgs e)
        {
            if (cb_replace.Checked)
            {
                text_box.CharacterCasing = CharacterCasing.Normal;
            } else
            {
                text_box.CharacterCasing = CharacterCasing.Upper;
            }
        }

        private void cbox_port_TextChanged(object sender, EventArgs e)
        {
            using (var searcher = new ManagementObjectSearcher("SELECT * FROM WIN32_SerialPort"))
            {
                string[] portnames = SerialPort.GetPortNames();
                var ports = searcher.Get().Cast<ManagementBaseObject>().ToList();
                var tList = (from n in portnames
                             join p in ports on n equals p["DeviceID"].ToString()
                             select n + " - " + p["Caption"]).ToList();
                bool notfound = true;
                foreach (string s in tList)
                {
                    if (s.IndexOf(cbox_port.SelectedItem.ToString(), StringComparison.Ordinal) >= 0)
                    {
                        lbl_COMName.Text = s;
                        notfound = false;
                    }
                }
                if (notfound)
                {
                    lbl_COMName.Text = "";
                }
            }
            serialPort1.PortName = cbox_port.SelectedItem.ToString();
        }

        private void trackBar1_ValueChanged(object sender, EventArgs e)
        {
            _scrollSpeed = trackBar1.Value;
        }

        private void SerialOpen()
        {
            if (serialPort1.IsOpen) return;
            cbox_port.Enabled = false;
            try
            {
                serialPort1.PortName = cbox_port.SelectedItem.ToString();
                serialPort1.Open();
            }
            catch (Exception)
            {
                MessageBox.Show("指定したポートが開けません。", "エラー", MessageBoxButtons.OK, MessageBoxIcon.Error);
                cbox_port.Enabled = true;
            }
        }
    }

}