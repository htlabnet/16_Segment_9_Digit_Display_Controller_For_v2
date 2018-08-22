using System;
using System.Collections.Generic;
using System.IO.Ports;
using System.Linq;
using System.Text;

namespace _16seg_control
{
    public static class DisplayUtil
    {
        const int DISPLAY_DIGITS = 9;

        public static string ThreadLoop_Send16Seg(List<SegChar> buf, int idx, int length, SerialPort sp)
        {
            string msg = string.Join("", buf.GetRange(idx, length).ConvertAll(x => x.Char));
            Console.WriteLine("send text_a: #" + msg + "#");
            send_16seg(buf.GetRange(idx, length).ToArray(), false, sp);
            return msg;
        }

        public static bool send_16seg(SegChar[] array, bool padding, SerialPort sp)
        {
            // 送信する文字列
            SegChar[] SendText = new SegChar[DISPLAY_DIGITS];
            // 配列の現在位置
            int pos = 0;
            // パディング文字数
            int pad_cnt = 0;

            // 表示文字が DISPLAY_DIGITS 桁超えたらダメ
            if (DISPLAY_DIGITS < array.Length) return false;

            //8文字以下の時にパディングを実施する
            if (padding)
            {
                if (array.Length <= DISPLAY_DIGITS)
                {
                    // パディング分を計算
                    // 両端にパディングを付けるので /2している
                    pad_cnt = (DISPLAY_DIGITS - array.Length) / 2;
#if DEBUG
                    Console.WriteLine("Padding: " + pos);
#endif
                    // 埋める
                    for (int i = 0; i < pad_cnt; i++)
                    {
                        SendText[i] = new SegChar(' ');
                    }
                }
                // 文字のスタートはパディング分から
                pos = pad_cnt;
            }

            //ドットは処理しつつ16セグ表示コマンドへ
            // foreachの部分で"."を" "に変換して、char[]に変化する
            // ToCharArray()はなくてもtexは暗黙的にcharになるが、明示的に変換を指示している
            for (int i = 0; i < array.Length; i++)
            {
                SendText[pos++] = array[i];
            }

            //後ろも埋める
            for (int i = pos; i < DISPLAY_DIGITS; i++)
            {
                SendText[i] = new SegChar(' ');
            }

            // まとめて送信
            send_16seg(SendText, sp);
            return true;
        }

        public static bool send_16seg(SegChar[] array, SerialPort sp)
        {
            bool state = true;
            for (int i = 0; i < array.Length; i++)
            {
                state = send_SegChar(i, Convert.ToByte(array[i].Char), array[i].isDot, sp);
            }
            return state;
        }

        public static bool send_SegChar(int digit, byte data, bool dot, SerialPort sp)
        {
            //桁指定コマンド(digit_dはドットが含まれる)
            byte[] digit_s = new byte[9] { 0xE0, 0xE1, 0xE2, 0xE3, 0xE4, 0xE5, 0xE6, 0xE7, 0xE8 };
            byte[] digit_d = new byte[9] { 0xF0, 0xF1, 0xF2, 0xF3, 0xF4, 0xF5, 0xF6, 0xF7, 0xF8 };

            //桁範囲チェック
            if (digit < 0 || 8 < digit)
                return false;

            byte[] buffer = new byte[2];
            if (dot)
                buffer[0] = digit_d[digit];
            else
                buffer[0] = digit_s[digit];

            buffer[1] = data;

            //16セグコントローラに出力
            if (sp.IsOpen == true)
                sp.Write(buffer, 0, buffer.Length);
            Console.WriteLine("16seg send: {0:X2}", BitConverter.ToString(buffer));
            return true;
        }


        public static List<SegChar> PackInputText(char[] t)
        {
            List<SegChar> SegList = new List<SegChar>();
            bool last = false;
            for (int i = 0; i < t.Length; i++)
            {
                // 一文字の場合の処理
                if (t.Length == 1)
                {
                    SegList.Add(new SegChar(t[i], false));
                }
                else
                {
                    // 先頭で"."が来るパターン
                    if ((i == 0) && (t[i] == '.'))
                    {
                        SegList.Add(new SegChar(' ', true));
                        continue;
                    }
                    // 末尾が"."
                    if ((i == t.Length - 1) && (t[i] == '.'))
                    {
                        SegList.Add(new SegChar(' ', true));
                        continue;
                    }
                    // dotが連続しているケース
                    // 範囲外にならないように注意
                    if (i - 1 >= 0)
                    {
                        if ((t[i] == '.') && (t[i - 1] == '.'))
                        {
                            SegList.Add(new SegChar(' ', true));
                            continue;
                        }
                    }
                    // 通常処理(1文字前まで)
                    if (i < t.Length - 1)
                    {
                        if (t[i + 1] == '.')
                        {
                            // 次の文字列に"."がある
                            SegList.Add(new SegChar(t[i], true));
                            // 次の"."を飛ばす
                            i++;
                            // フラグを立てておく
                            if (i == t.Length - 1) last = true;
                        }
                        else
                        {
                            // それ以外
                            SegList.Add(new SegChar(t[i], false));
                        }
                    }
                    // ここまできたら
                    // 最後の文字はドット無いことが確約されている
                    if ((i == t.Length - 1) && (last == false))
                    {
                        SegList.Add(new SegChar(t[i], false));
                    }
                }
            }
            return SegList;
        }
    }
}
