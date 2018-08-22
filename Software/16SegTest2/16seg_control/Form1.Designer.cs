namespace _16seg_control
{
    partial class MainForm
    {
        /// <summary>
        /// 必要なデザイナー変数です。
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// 使用中のリソースをすべてクリーンアップします。
        /// </summary>
        /// <param name="disposing">マネージ リソースが破棄される場合 true、破棄されない場合は false です。</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows フォーム デザイナーで生成されたコード

        /// <summary>
        /// デザイナー サポートに必要なメソッドです。このメソッドの内容を
        /// コード エディターで変更しないでください。
        /// </summary>
        private void InitializeComponent()
        {
            this.components = new System.ComponentModel.Container();
            this.label1 = new System.Windows.Forms.Label();
            this.lbl_nowtext = new System.Windows.Forms.Label();
            this.btn_delete = new System.Windows.Forms.Button();
            this.text_box = new System.Windows.Forms.TextBox();
            this.btn_sendtext = new System.Windows.Forms.Button();
            this.cb_replace = new System.Windows.Forms.CheckBox();
            this.serialPort1 = new System.IO.Ports.SerialPort(this.components);
            this.trackBar1 = new System.Windows.Forms.TrackBar();
            this.label3 = new System.Windows.Forms.Label();
            this.label4 = new System.Windows.Forms.Label();
            this.label5 = new System.Windows.Forms.Label();
            this.lbl_ledtext = new System.Windows.Forms.Label();
            this.cbox_port = new System.Windows.Forms.ComboBox();
            this.label6 = new System.Windows.Forms.Label();
            this.lbl_COMName = new System.Windows.Forms.Label();
            ((System.ComponentModel.ISupportInitialize)(this.trackBar1)).BeginInit();
            this.SuspendLayout();
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(10, 230);
            this.label1.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(52, 15);
            this.label1.TabIndex = 0;
            this.label1.Text = "表示中";
            // 
            // lbl_nowtext
            // 
            this.lbl_nowtext.AutoSize = true;
            this.lbl_nowtext.Location = new System.Drawing.Point(73, 230);
            this.lbl_nowtext.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.lbl_nowtext.Name = "lbl_nowtext";
            this.lbl_nowtext.Size = new System.Drawing.Size(42, 15);
            this.lbl_nowtext.TabIndex = 1;
            this.lbl_nowtext.Text = "TEXT";
            // 
            // btn_delete
            // 
            this.btn_delete.Location = new System.Drawing.Point(136, 168);
            this.btn_delete.Margin = new System.Windows.Forms.Padding(4);
            this.btn_delete.Name = "btn_delete";
            this.btn_delete.Size = new System.Drawing.Size(237, 29);
            this.btn_delete.TabIndex = 10;
            this.btn_delete.Text = "掲示板表示文字を削除";
            this.btn_delete.UseVisualStyleBackColor = true;
            this.btn_delete.Click += new System.EventHandler(this.btn_delete_Click);
            // 
            // text_box
            // 
            this.text_box.CharacterCasing = System.Windows.Forms.CharacterCasing.Upper;
            this.text_box.Location = new System.Drawing.Point(32, 32);
            this.text_box.Margin = new System.Windows.Forms.Padding(4);
            this.text_box.Name = "text_box";
            this.text_box.Size = new System.Drawing.Size(308, 22);
            this.text_box.TabIndex = 6;
            // 
            // btn_sendtext
            // 
            this.btn_sendtext.Location = new System.Drawing.Point(349, 27);
            this.btn_sendtext.Margin = new System.Windows.Forms.Padding(4);
            this.btn_sendtext.Name = "btn_sendtext";
            this.btn_sendtext.Size = new System.Drawing.Size(100, 29);
            this.btn_sendtext.TabIndex = 7;
            this.btn_sendtext.Text = "送信";
            this.btn_sendtext.UseVisualStyleBackColor = true;
            this.btn_sendtext.Click += new System.EventHandler(this.btn_sendtext_Click);
            // 
            // cb_replace
            // 
            this.cb_replace.AutoSize = true;
            this.cb_replace.Location = new System.Drawing.Point(357, 124);
            this.cb_replace.Margin = new System.Windows.Forms.Padding(4);
            this.cb_replace.Name = "cb_replace";
            this.cb_replace.Size = new System.Drawing.Size(175, 19);
            this.cb_replace.TabIndex = 9;
            this.cb_replace.Text = "入力内容をそのまま送信";
            this.cb_replace.UseVisualStyleBackColor = true;
            this.cb_replace.CheckedChanged += new System.EventHandler(this.cb_replace_CheckedChanged);
            // 
            // trackBar1
            // 
            this.trackBar1.LargeChange = 50;
            this.trackBar1.Location = new System.Drawing.Point(26, 104);
            this.trackBar1.Margin = new System.Windows.Forms.Padding(4);
            this.trackBar1.Maximum = 900;
            this.trackBar1.Name = "trackBar1";
            this.trackBar1.Size = new System.Drawing.Size(309, 56);
            this.trackBar1.SmallChange = 50;
            this.trackBar1.TabIndex = 8;
            this.trackBar1.TickFrequency = 50;
            this.trackBar1.Value = 700;
            this.trackBar1.ValueChanged += new System.EventHandler(this.trackBar1_ValueChanged);
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(10, 13);
            this.label3.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(146, 15);
            this.label3.TabIndex = 13;
            this.label3.Text = "テキストボックスから送信";
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Location = new System.Drawing.Point(10, 86);
            this.label4.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(93, 15);
            this.label4.TabIndex = 14;
            this.label4.Text = "スクロール速度";
            // 
            // label5
            // 
            this.label5.AutoSize = true;
            this.label5.Location = new System.Drawing.Point(13, 250);
            this.label5.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(33, 15);
            this.label5.TabIndex = 15;
            this.label5.Text = "LED";
            // 
            // lbl_ledtext
            // 
            this.lbl_ledtext.AutoSize = true;
            this.lbl_ledtext.Location = new System.Drawing.Point(73, 250);
            this.lbl_ledtext.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.lbl_ledtext.Name = "lbl_ledtext";
            this.lbl_ledtext.Size = new System.Drawing.Size(42, 15);
            this.lbl_ledtext.TabIndex = 16;
            this.lbl_ledtext.Text = "TEXT";
            // 
            // cbox_port
            // 
            this.cbox_port.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.cbox_port.FormattingEnabled = true;
            this.cbox_port.Location = new System.Drawing.Point(372, 224);
            this.cbox_port.Margin = new System.Windows.Forms.Padding(4);
            this.cbox_port.Name = "cbox_port";
            this.cbox_port.Size = new System.Drawing.Size(119, 23);
            this.cbox_port.TabIndex = 11;
            this.cbox_port.DropDown += new System.EventHandler(this.cbox_port_DropDown);
            this.cbox_port.TextChanged += new System.EventHandler(this.cbox_port_TextChanged);
            // 
            // label6
            // 
            this.label6.AutoSize = true;
            this.label6.Location = new System.Drawing.Point(288, 228);
            this.label6.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.label6.Name = "label6";
            this.label6.Size = new System.Drawing.Size(72, 15);
            this.label6.TabIndex = 18;
            this.label6.Text = "コントローラ";
            // 
            // lbl_COMName
            // 
            this.lbl_COMName.AutoSize = true;
            this.lbl_COMName.Location = new System.Drawing.Point(288, 258);
            this.lbl_COMName.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.lbl_COMName.Name = "lbl_COMName";
            this.lbl_COMName.Size = new System.Drawing.Size(42, 15);
            this.lbl_COMName.TabIndex = 20;
            this.lbl_COMName.Text = "TEXT";
            // 
            // MainForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(8F, 15F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(541, 286);
            this.Controls.Add(this.lbl_COMName);
            this.Controls.Add(this.label6);
            this.Controls.Add(this.cbox_port);
            this.Controls.Add(this.lbl_ledtext);
            this.Controls.Add(this.label5);
            this.Controls.Add(this.label4);
            this.Controls.Add(this.label3);
            this.Controls.Add(this.trackBar1);
            this.Controls.Add(this.cb_replace);
            this.Controls.Add(this.btn_sendtext);
            this.Controls.Add(this.text_box);
            this.Controls.Add(this.btn_delete);
            this.Controls.Add(this.lbl_nowtext);
            this.Controls.Add(this.label1);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedSingle;
            this.Margin = new System.Windows.Forms.Padding(4);
            this.MaximizeBox = false;
            this.MinimizeBox = false;
            this.Name = "MainForm";
            this.Text = "16 Segment LED Contoller (Serial Ver)";
            this.Load += new System.EventHandler(this.Form1_Load);
            ((System.ComponentModel.ISupportInitialize)(this.trackBar1)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Label lbl_nowtext;
        private System.Windows.Forms.Button btn_delete;
        private System.Windows.Forms.TextBox text_box;
        private System.Windows.Forms.Button btn_sendtext;
        private System.Windows.Forms.CheckBox cb_replace;
        private System.IO.Ports.SerialPort serialPort1;
        private System.Windows.Forms.TrackBar trackBar1;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.Label label5;
        private System.Windows.Forms.Label lbl_ledtext;
        private System.Windows.Forms.ComboBox cbox_port;
        private System.Windows.Forms.Label label6;
        private System.Windows.Forms.Label lbl_COMName;
    }
}

