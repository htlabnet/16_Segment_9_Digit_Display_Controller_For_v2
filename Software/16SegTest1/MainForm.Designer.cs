namespace _16SegControl
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
        /// <param name="disposing">マネージ リソースを破棄する場合は true を指定し、その他の場合は false を指定します。</param>
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
            this.btnHIDDevConnect = new System.Windows.Forms.Button();
            this.grpBoxDots = new System.Windows.Forms.GroupBox();
            this.cbDot9 = new System.Windows.Forms.CheckBox();
            this.cbDot1 = new System.Windows.Forms.CheckBox();
            this.cbDot8 = new System.Windows.Forms.CheckBox();
            this.cbDot2 = new System.Windows.Forms.CheckBox();
            this.cbDot7 = new System.Windows.Forms.CheckBox();
            this.cbDot3 = new System.Windows.Forms.CheckBox();
            this.cbDot6 = new System.Windows.Forms.CheckBox();
            this.cbDot4 = new System.Windows.Forms.CheckBox();
            this.cbDot5 = new System.Windows.Forms.CheckBox();
            this.btnSend = new System.Windows.Forms.Button();
            this.grpBoxController = new System.Windows.Forms.GroupBox();
            this.tbText = new System.Windows.Forms.TextBox();
            this.grpBoxControl = new System.Windows.Forms.GroupBox();
            this.lblScroolSpeed = new System.Windows.Forms.Label();
            this.tbarScrollSpeed = new System.Windows.Forms.TrackBar();
            this.cbDotOverride = new System.Windows.Forms.CheckBox();
            this.cbStrReplace = new System.Windows.Forms.CheckBox();
            this.lblText = new System.Windows.Forms.Label();
            this.grpBoxDots.SuspendLayout();
            this.grpBoxController.SuspendLayout();
            this.grpBoxControl.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.tbarScrollSpeed)).BeginInit();
            this.SuspendLayout();
            // 
            // btnHIDDevConnect
            // 
            this.btnHIDDevConnect.Location = new System.Drawing.Point(8, 111);
            this.btnHIDDevConnect.Name = "btnHIDDevConnect";
            this.btnHIDDevConnect.Size = new System.Drawing.Size(144, 34);
            this.btnHIDDevConnect.TabIndex = 0;
            this.btnHIDDevConnect.Text = "HIDデバイスと接続";
            this.btnHIDDevConnect.UseVisualStyleBackColor = true;
            this.btnHIDDevConnect.Click += new System.EventHandler(this.btnHIDDevConnect_Click);
            // 
            // grpBoxDots
            // 
            this.grpBoxDots.Controls.Add(this.cbDot9);
            this.grpBoxDots.Controls.Add(this.cbDot1);
            this.grpBoxDots.Controls.Add(this.cbDot8);
            this.grpBoxDots.Controls.Add(this.cbDot2);
            this.grpBoxDots.Controls.Add(this.cbDot7);
            this.grpBoxDots.Controls.Add(this.cbDot3);
            this.grpBoxDots.Controls.Add(this.cbDot6);
            this.grpBoxDots.Controls.Add(this.cbDot4);
            this.grpBoxDots.Controls.Add(this.cbDot5);
            this.grpBoxDots.Enabled = false;
            this.grpBoxDots.Location = new System.Drawing.Point(409, 12);
            this.grpBoxDots.Name = "grpBoxDots";
            this.grpBoxDots.Size = new System.Drawing.Size(79, 249);
            this.grpBoxDots.TabIndex = 1;
            this.grpBoxDots.TabStop = false;
            this.grpBoxDots.Text = "ドット";
            // 
            // cbDot9
            // 
            this.cbDot9.AutoSize = true;
            this.cbDot9.Location = new System.Drawing.Point(6, 221);
            this.cbDot9.Name = "cbDot9";
            this.cbDot9.Size = new System.Drawing.Size(67, 19);
            this.cbDot9.TabIndex = 2;
            this.cbDot9.Text = "9桁目";
            this.cbDot9.UseVisualStyleBackColor = true;
            // 
            // cbDot1
            // 
            this.cbDot1.AutoSize = true;
            this.cbDot1.Location = new System.Drawing.Point(6, 21);
            this.cbDot1.Name = "cbDot1";
            this.cbDot1.Size = new System.Drawing.Size(67, 19);
            this.cbDot1.TabIndex = 2;
            this.cbDot1.Text = "1桁目";
            this.cbDot1.UseVisualStyleBackColor = true;
            // 
            // cbDot8
            // 
            this.cbDot8.AutoSize = true;
            this.cbDot8.Location = new System.Drawing.Point(6, 196);
            this.cbDot8.Name = "cbDot8";
            this.cbDot8.Size = new System.Drawing.Size(67, 19);
            this.cbDot8.TabIndex = 2;
            this.cbDot8.Text = "8桁目";
            this.cbDot8.UseVisualStyleBackColor = true;
            // 
            // cbDot2
            // 
            this.cbDot2.AutoSize = true;
            this.cbDot2.Location = new System.Drawing.Point(6, 46);
            this.cbDot2.Name = "cbDot2";
            this.cbDot2.Size = new System.Drawing.Size(67, 19);
            this.cbDot2.TabIndex = 2;
            this.cbDot2.Text = "2桁目";
            this.cbDot2.UseVisualStyleBackColor = true;
            // 
            // cbDot7
            // 
            this.cbDot7.AutoSize = true;
            this.cbDot7.Location = new System.Drawing.Point(6, 171);
            this.cbDot7.Name = "cbDot7";
            this.cbDot7.Size = new System.Drawing.Size(67, 19);
            this.cbDot7.TabIndex = 2;
            this.cbDot7.Text = "7桁目";
            this.cbDot7.UseVisualStyleBackColor = true;
            // 
            // cbDot3
            // 
            this.cbDot3.AutoSize = true;
            this.cbDot3.Location = new System.Drawing.Point(6, 71);
            this.cbDot3.Name = "cbDot3";
            this.cbDot3.Size = new System.Drawing.Size(67, 19);
            this.cbDot3.TabIndex = 2;
            this.cbDot3.Text = "3桁目";
            this.cbDot3.UseVisualStyleBackColor = true;
            // 
            // cbDot6
            // 
            this.cbDot6.AutoSize = true;
            this.cbDot6.Location = new System.Drawing.Point(6, 146);
            this.cbDot6.Name = "cbDot6";
            this.cbDot6.Size = new System.Drawing.Size(67, 19);
            this.cbDot6.TabIndex = 2;
            this.cbDot6.Text = "6桁目";
            this.cbDot6.UseVisualStyleBackColor = true;
            // 
            // cbDot4
            // 
            this.cbDot4.AutoSize = true;
            this.cbDot4.Location = new System.Drawing.Point(6, 96);
            this.cbDot4.Name = "cbDot4";
            this.cbDot4.Size = new System.Drawing.Size(67, 19);
            this.cbDot4.TabIndex = 2;
            this.cbDot4.Text = "4桁目";
            this.cbDot4.UseVisualStyleBackColor = true;
            // 
            // cbDot5
            // 
            this.cbDot5.AutoSize = true;
            this.cbDot5.Location = new System.Drawing.Point(6, 121);
            this.cbDot5.Name = "cbDot5";
            this.cbDot5.Size = new System.Drawing.Size(67, 19);
            this.cbDot5.TabIndex = 2;
            this.cbDot5.Text = "5桁目";
            this.cbDot5.UseVisualStyleBackColor = true;
            // 
            // btnSend
            // 
            this.btnSend.Enabled = false;
            this.btnSend.Location = new System.Drawing.Point(39, 18);
            this.btnSend.Name = "btnSend";
            this.btnSend.Size = new System.Drawing.Size(75, 23);
            this.btnSend.TabIndex = 2;
            this.btnSend.Text = "送信";
            this.btnSend.UseVisualStyleBackColor = true;
            this.btnSend.Click += new System.EventHandler(this.btnSend_Click);
            // 
            // grpBoxController
            // 
            this.grpBoxController.Controls.Add(this.btnHIDDevConnect);
            this.grpBoxController.Controls.Add(this.btnSend);
            this.grpBoxController.Location = new System.Drawing.Point(12, 103);
            this.grpBoxController.Name = "grpBoxController";
            this.grpBoxController.Size = new System.Drawing.Size(158, 158);
            this.grpBoxController.TabIndex = 3;
            this.grpBoxController.TabStop = false;
            this.grpBoxController.Text = "コントローラ";
            // 
            // tbText
            // 
            this.tbText.Location = new System.Drawing.Point(70, 58);
            this.tbText.Name = "tbText";
            this.tbText.Size = new System.Drawing.Size(317, 22);
            this.tbText.TabIndex = 4;
            // 
            // grpBoxControl
            // 
            this.grpBoxControl.Controls.Add(this.lblScroolSpeed);
            this.grpBoxControl.Controls.Add(this.tbarScrollSpeed);
            this.grpBoxControl.Controls.Add(this.cbDotOverride);
            this.grpBoxControl.Controls.Add(this.cbStrReplace);
            this.grpBoxControl.Location = new System.Drawing.Point(187, 103);
            this.grpBoxControl.Name = "grpBoxControl";
            this.grpBoxControl.Size = new System.Drawing.Size(200, 158);
            this.grpBoxControl.TabIndex = 5;
            this.grpBoxControl.TabStop = false;
            this.grpBoxControl.Text = "制御";
            // 
            // lblScroolSpeed
            // 
            this.lblScroolSpeed.AutoSize = true;
            this.lblScroolSpeed.Location = new System.Drawing.Point(6, 18);
            this.lblScroolSpeed.Name = "lblScroolSpeed";
            this.lblScroolSpeed.Size = new System.Drawing.Size(96, 15);
            this.lblScroolSpeed.TabIndex = 7;
            this.lblScroolSpeed.Text = "スクロール速度:";
            // 
            // tbarScrollSpeed
            // 
            this.tbarScrollSpeed.Location = new System.Drawing.Point(9, 36);
            this.tbarScrollSpeed.Maximum = 3000;
            this.tbarScrollSpeed.Name = "tbarScrollSpeed";
            this.tbarScrollSpeed.Size = new System.Drawing.Size(179, 56);
            this.tbarScrollSpeed.TabIndex = 8;
            this.tbarScrollSpeed.TickFrequency = 100;
            this.tbarScrollSpeed.Scroll += new System.EventHandler(this.tbarScrollSpeed_Scroll);
            // 
            // cbDotOverride
            // 
            this.cbDotOverride.AutoSize = true;
            this.cbDotOverride.Location = new System.Drawing.Point(9, 126);
            this.cbDotOverride.Name = "cbDotOverride";
            this.cbDotOverride.Size = new System.Drawing.Size(155, 19);
            this.cbDotOverride.TabIndex = 7;
            this.cbDotOverride.Text = "ドットを手動定義する";
            this.cbDotOverride.UseVisualStyleBackColor = true;
            this.cbDotOverride.CheckedChanged += new System.EventHandler(this.cbDotOverride_CheckedChanged);
            // 
            // cbStrReplace
            // 
            this.cbStrReplace.AutoSize = true;
            this.cbStrReplace.Location = new System.Drawing.Point(9, 98);
            this.cbStrReplace.Name = "cbStrReplace";
            this.cbStrReplace.Size = new System.Drawing.Size(139, 19);
            this.cbStrReplace.TabIndex = 6;
            this.cbStrReplace.Text = "文字列を置換する";
            this.cbStrReplace.UseVisualStyleBackColor = true;
            // 
            // lblText
            // 
            this.lblText.AutoSize = true;
            this.lblText.Location = new System.Drawing.Point(9, 61);
            this.lblText.Name = "lblText";
            this.lblText.Size = new System.Drawing.Size(55, 15);
            this.lblText.TabIndex = 6;
            this.lblText.Text = "テキスト:";
            // 
            // MainForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(8F, 15F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(496, 269);
            this.Controls.Add(this.lblText);
            this.Controls.Add(this.grpBoxControl);
            this.Controls.Add(this.tbText);
            this.Controls.Add(this.grpBoxController);
            this.Controls.Add(this.grpBoxDots);
            this.Name = "MainForm";
            this.Text = "16 Segment LED Control Software";
            this.grpBoxDots.ResumeLayout(false);
            this.grpBoxDots.PerformLayout();
            this.grpBoxController.ResumeLayout(false);
            this.grpBoxControl.ResumeLayout(false);
            this.grpBoxControl.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.tbarScrollSpeed)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Button btnHIDDevConnect;
        private System.Windows.Forms.GroupBox grpBoxDots;
        private System.Windows.Forms.CheckBox cbDot9;
        private System.Windows.Forms.CheckBox cbDot1;
        private System.Windows.Forms.CheckBox cbDot8;
        private System.Windows.Forms.CheckBox cbDot2;
        private System.Windows.Forms.CheckBox cbDot7;
        private System.Windows.Forms.CheckBox cbDot3;
        private System.Windows.Forms.CheckBox cbDot6;
        private System.Windows.Forms.CheckBox cbDot4;
        private System.Windows.Forms.CheckBox cbDot5;
        private System.Windows.Forms.Button btnSend;
        private System.Windows.Forms.GroupBox grpBoxController;
        private System.Windows.Forms.TextBox tbText;
        private System.Windows.Forms.GroupBox grpBoxControl;
        private System.Windows.Forms.CheckBox cbStrReplace;
        private System.Windows.Forms.Label lblText;
        private System.Windows.Forms.CheckBox cbDotOverride;
        private System.Windows.Forms.TrackBar tbarScrollSpeed;
        private System.Windows.Forms.Label lblScroolSpeed;
    }
}

