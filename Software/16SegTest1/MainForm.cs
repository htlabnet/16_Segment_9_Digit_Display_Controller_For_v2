using System;
using System.Collections.Generic;
using System.Linq;
using System.Text.RegularExpressions;
using System.Threading;
using System.Threading.Tasks;
using System.Windows.Forms;
using HidSharp;

namespace _16SegControl
{
    public partial class MainForm : Form
    {
        // ユニットの表示桁数
        private const int DisplayDigits = 9;
        private HidDevice _segDev = null;
        private CancellationTokenSource _tokenSource= new CancellationTokenSource();
        private Task _task;
        private int _scroolSpeed = 500;

        public MainForm()
        {
            InitializeComponent();
            tbarScrollSpeed.Value = _scroolSpeed;
        }

        /// <summary>
        /// HIDデバイスと接続する。Vendor IDとProduct IDは現在固定
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void btnHIDDevConnect_Click(object sender, EventArgs e)
        {
            var hidDeviceList = DeviceList.Local.GetHidDevices();
            bool error = false;
            try
            {
                _segDev = hidDeviceList.First(x => (x.VendorID == 0x4d8 && x.ProductID == 0x3f));
            }
            catch (InvalidOperationException)
            {
                error = true;
            }
            if (_segDev == null || error)
            {
                MessageBox.Show("HIDデバイスが見つかりません", "エラー", MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
            else
            {
                btnSend.Enabled = true;
            }
        }

        /// <summary>
        /// 指定された桁数のCheckBoxがチェックされているか調べる
        /// </summary>
        /// <param name="digits">ドットを表示するか確認する桁数</param>
        /// <returns></returns>
        private bool GetLedDot(int digits)
        {
            bool dot = false;
            switch (digits)
            {
                case 1:
                    dot = cbDot1.Checked;
                    break;
                case 2:
                    dot = cbDot2.Checked;
                    break;
                case 3:
                    dot = cbDot3.Checked;
                    break;
                case 4:
                    dot = cbDot4.Checked;
                    break;
                case 5:
                    dot = cbDot5.Checked;
                    break;
                case 6:
                    dot = cbDot6.Checked;
                    break;
                case 7:
                    dot = cbDot7.Checked;
                    break;
                case 8:
                    dot = cbDot8.Checked;
                    break;
                case 9:
                    dot = cbDot9.Checked;
                    break;
            }
            return dot;
        }

        /// <summary>
        /// CheckBoxのステータスからFirmwareに送信するデータを作成する
        /// </summary>
        /// <returns>ファームウエアで使用するDOTSバイト列</returns>
        private byte[] LedDotToBytes()
        {
            byte dot1 = 0x00;
            byte dot2 = 0x00;
            for (var i = 1; i <= 9; i++)
            {
                if (GetLedDot(i)) dot1 = (byte) ((1 << i - 1) | dot1);
                if (i > 8 && GetLedDot(i)) dot2 = (byte) ((1 << i - 9) | dot2);
            }
            return new byte[] { dot1, dot2 };
        }

        /// <summary>
        /// int配列からFirmwareにドット用の送信するデータを作成する。
        /// </summary>
        /// <param name="flags">1であればdotsを付ける。9桁まで対応</param>
        /// <returns></returns>
        private byte[] LedDotToBytes(int[] flags)
        {
            byte dot1 = 0x00;
            byte dot2 = 0x00;
            for (var i = 0; i < 9; i++)
            {
                if (flags[i] == 1) dot1 = (byte) ((1 << i) | dot1);
                if (i > 8 && (flags[i] == 1)) dot2 = (byte) ((1 << i - 8) | dot2);
            }
            return new[] { dot1, dot2 };
        }

        /// <summary>
        /// 送信ボタンのクリック
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void btnSend_Click(object sender, EventArgs e)
        {
            var text = tbText.Text;

            if (cbStrReplace.Checked)
            {
                // LEDに表示できない文字は " "(空白)として扱う
                Regex re = new Regex(@"[^!%()*+-/<>A-Z0-9]");
                text = (re.Replace(text.ToUpper(), " "));
            }

            // 入力されたTextを解析してSegChar形式に変換する
            // SegCharにすることで文字とドットの関係性が作れる
            var t = text.ToCharArray();
            var segList = PackInputText(t);

            // DISPLAY_DIGITS 文字以下はそのまま出していく
            if (DisplayDigits >= segList.Count)
            {
                // HIDに書き込む
                CheckSegDevAlive();
                if (_segDev.TryOpen(out var hidStream))
                {
                    using (hidStream)
                    {
                        hidStream.Write(CreateDisplayCommandBuffer(segList));
                    }
                }
            }
            else
            {
                int i = 0;
                _task = Task.Factory.StartNew(() =>
                {
                    while (true)
                    {
                        // 先頭に戻る
                        if (i > segList.Count) i = 0;
                        // 前回から一文字ずらして取得する。Takeでは要素が足りない場合その要素は取得されない特性がある
                        // ex. 10文字あって先頭が8文字目のとき、9文字取得しようとすると2文字しか出てこない(Count = 2)ことになる
                        // このため、以降のパディング処理が楽になる
                        var disp = segList.Skip(i++).Take(DisplayDigits).ToList();

                        if (disp.Count <= DisplayDigits)
                            // " "でパディング
                            disp.AddRange(Enumerable.Repeat(new SegChar(' '), DisplayDigits - disp.Count));

                        // HIDに書き込む
                        // TODO: 書き込み自体は関数を分離させた方が良いかも知れない
                        if (_segDev.TryOpen(out var hidStream))
                        {
                            using (hidStream)
                            {
                                hidStream.Write(CreateDisplayCommandBuffer(disp));
                            }
                        }
                        else
                        {
                            // デバイス死んでたらトークンをキャンセルしてwhileループを殺す
                            if (CheckSegDevAlive()) _tokenSource.Cancel(true);
                        }

                        var waitTask = Task.Run(async delegate { await Task.Delay(_scroolSpeed); });
                        waitTask.Wait();
                    }
                }, _tokenSource.Token);
            }
        }

        private bool CheckSegDevAlive()
        {
            var state = true;
            if (_segDev == null)
            {
                state = false;
            }
            else
            {
                if (_segDev.TryOpen(out var hidStream))
                {
                    using (hidStream)
                    {
                        hidStream.Write(new byte[] {0, 0xFF});
                        hidStream.ReadTimeout = 300;
                        var read = hidStream.Read();
                        var versionInfo = read.Skip(1).Take(2).ToArray();
                        // TODO: バージョンをハードコードするのをやめよう
                        if ((versionInfo[0] != 0xFF || versionInfo[1] != 0x02))
                        {
                            state = false;
                        }
                    }
                }
            }

            if (!state)
            {
                MessageBox.Show("コントローラが接続されていません", "エラー", MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
            return state;
        }

        private byte[] CreateDisplayCommandBuffer(List<SegChar> segList)
        {
            var buffer = new List<byte>();
            var dots = new List<int>();
            // HIDのDescription ID
            buffer.Add(0x0);
            // 9桁同時送信のコマンド
            buffer.Add(0x1F);
            // 1文字ごとにバッファに入れていく
            foreach (var seg in segList)
            {
                buffer.Add((byte) seg.Char);
                dots.Add(seg.IsDot ? 1 : 0);
            }

            // 9桁に満たない場合、dotsとbufferにパディングをする
            dots.AddRange(Enumerable.Repeat(0, DisplayDigits - segList.Count));
            buffer.AddRange(Enumerable.Repeat((byte) 0x00, (DisplayDigits + 2) - buffer.Count));
            // dotsデータからファームに送るバイト列を作る
            // 上書きがチェックされていたらUIのチェックボックスを優先する
            buffer.AddRange(cbDotOverride.Checked ? LedDotToBytes() : LedDotToBytes(dots.ToArray()));

            return buffer.ToArray();
        }

        /// <summary>
        /// 入力された文字をSegCharに変換する。
        /// 入力文字が"A."だった場合、Aの桁にdotを付けることになるので、
        /// そのパースを行う。
        /// </summary>
        /// <param name="t">解析を行うchar配列</param>
        /// <returns></returns>
        public static List<SegChar> PackInputText(char[] t)
        {
            List<SegChar> segList = new List<SegChar>();
            bool last = false;
            for (int i = 0; i < t.Length; i++)
            {
                // 一文字の場合の処理
                if (t.Length == 1)
                {
                    segList.Add(new SegChar(t[i], false));
                }
                else
                {
                    // 先頭で"."が来るパターン
                    if ((i == 0) && (t[i] == '.'))
                    {
                        segList.Add(new SegChar(' ', true));
                        continue;
                    }

                    // 末尾が"."
                    if ((i == t.Length - 1) && (t[i] == '.'))
                    {
                        segList.Add(new SegChar(' ', true));
                        continue;
                    }

                    // dotが連続しているケース
                    // 範囲外にならないように注意
                    if (i - 1 >= 0)
                    {
                        if ((t[i] == '.') && (t[i - 1] == '.'))
                        {
                            segList.Add(new SegChar(' ', true));
                            continue;
                        }
                    }

                    // 通常処理(1文字前まで)
                    if (i < t.Length - 1)
                    {
                        if (t[i + 1] == '.')
                        {
                            // 次の文字列に"."がある
                            segList.Add(new SegChar(t[i], true));
                            // 次の"."を飛ばす
                            i++;
                            // フラグを立てておく
                            if (i == t.Length - 1) last = true;
                        }
                        else
                        {
                            // それ以外
                            segList.Add(new SegChar(t[i], false));
                        }
                    }

                    // ここまできたら,最後の文字はドット無いことが確約されている
                    if ((i == t.Length - 1) && (last == false))
                    {
                        segList.Add(new SegChar(t[i], false));
                    }
                }
            }
            return segList;
        }

        private void cbDotOverride_CheckedChanged(object sender, EventArgs e)
        {
            grpBoxDots.Enabled = cbDotOverride.Checked;
        }

        private void tbarScrollSpeed_Scroll(object sender, EventArgs e)
        {
            _scroolSpeed = tbarScrollSpeed.Value;
        }
    }
}
