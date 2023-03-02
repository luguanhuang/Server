namespace 服务器启动工具
{
    partial class Form1
    {
        /// <summary>
        /// 必需的设计器变量。
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// 清理所有正在使用的资源。
        /// </summary>
        /// <param name="disposing">如果应释放托管资源，为 true；否则为 false。</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows 窗体设计器生成的代码

        /// <summary>
        /// 设计器支持所需的方法 - 不要
        /// 使用代码编辑器修改此方法的内容。
        /// </summary>
        private void InitializeComponent()
        {
            this.Update = new System.Windows.Forms.Button();
            this.Compile = new System.Windows.Forms.Button();
            this.Status = new System.Windows.Forms.Button();
            this.Start = new System.Windows.Forms.Button();
            this.Stop = new System.Windows.Forms.Button();
            this.comboBox1 = new System.Windows.Forms.ComboBox();
            this.label1 = new System.Windows.Forms.Label();
            this.label2 = new System.Windows.Forms.Label();
            this.label3 = new System.Windows.Forms.Label();
            this.labelUserInfo = new System.Windows.Forms.Label();
            this.WhiteList = new System.Windows.Forms.Button();
            this.ChangeTime = new System.Windows.Forms.Button();
            this.SuspendLayout();
            // 
            // Update
            // 
            this.Update.Location = new System.Drawing.Point(204, 103);
            this.Update.Name = "Update";
            this.Update.Size = new System.Drawing.Size(75, 23);
            this.Update.TabIndex = 1;
            this.Update.Text = "更新";
            this.Update.UseVisualStyleBackColor = true;
            this.Update.Click += new System.EventHandler(this.Update_Click);
            // 
            // Compile
            // 
            this.Compile.Location = new System.Drawing.Point(204, 132);
            this.Compile.Name = "Compile";
            this.Compile.Size = new System.Drawing.Size(75, 23);
            this.Compile.TabIndex = 2;
            this.Compile.Text = "编译";
            this.Compile.UseVisualStyleBackColor = true;
            this.Compile.Click += new System.EventHandler(this.Compile_Click);
            // 
            // Status
            // 
            this.Status.Location = new System.Drawing.Point(81, 160);
            this.Status.Name = "Status";
            this.Status.Size = new System.Drawing.Size(75, 23);
            this.Status.TabIndex = 3;
            this.Status.Text = "状态";
            this.Status.UseVisualStyleBackColor = true;
            this.Status.Click += new System.EventHandler(this.Status_Click);
            // 
            // Start
            // 
            this.Start.Location = new System.Drawing.Point(81, 102);
            this.Start.Name = "Start";
            this.Start.Size = new System.Drawing.Size(75, 23);
            this.Start.TabIndex = 4;
            this.Start.Text = "启动";
            this.Start.UseVisualStyleBackColor = true;
            this.Start.Click += new System.EventHandler(this.Start_Click);
            // 
            // Stop
            // 
            this.Stop.Location = new System.Drawing.Point(81, 131);
            this.Stop.Name = "Stop";
            this.Stop.Size = new System.Drawing.Size(75, 23);
            this.Stop.TabIndex = 5;
            this.Stop.Text = "关闭";
            this.Stop.UseVisualStyleBackColor = true;
            this.Stop.Click += new System.EventHandler(this.Stop_Click);
            // 
            // comboBox1
            // 
            this.comboBox1.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.comboBox1.FormattingEnabled = true;
            this.comboBox1.Location = new System.Drawing.Point(81, 38);
            this.comboBox1.Name = "comboBox1";
            this.comboBox1.Size = new System.Drawing.Size(151, 20);
            this.comboBox1.TabIndex = 6;
            this.comboBox1.SelectedIndexChanged += new System.EventHandler(this.comboBox1_SelectedIndexChanged);
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(18, 41);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(47, 12);
            this.label1.TabIndex = 7;
            this.label1.Text = "服务器:";
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(18, 107);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(35, 12);
            this.label2.TabIndex = 8;
            this.label2.Text = "操作:";
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(18, 74);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(35, 12);
            this.label3.TabIndex = 9;
            this.label3.Text = "用户:";
            // 
            // labelUserInfo
            // 
            this.labelUserInfo.AutoSize = true;
            this.labelUserInfo.Location = new System.Drawing.Point(79, 74);
            this.labelUserInfo.Name = "labelUserInfo";
            this.labelUserInfo.Size = new System.Drawing.Size(35, 12);
            this.labelUserInfo.TabIndex = 10;
            this.labelUserInfo.Text = "用户:";
            // 
            // WhiteList
            // 
            this.WhiteList.Location = new System.Drawing.Point(204, 160);
            this.WhiteList.Name = "WhiteList";
            this.WhiteList.Size = new System.Drawing.Size(75, 23);
            this.WhiteList.TabIndex = 11;
            this.WhiteList.Text = "白名单";
            this.WhiteList.UseVisualStyleBackColor = true;
            this.WhiteList.Click += new System.EventHandler(this.WhiteList_Click);
            // 
            // ChangeTime
            // 
            this.ChangeTime.Location = new System.Drawing.Point(81, 189);
            this.ChangeTime.Name = "ChangeTime";
            this.ChangeTime.Size = new System.Drawing.Size(75, 23);
            this.ChangeTime.TabIndex = 12;
            this.ChangeTime.Text = "调时间";
            this.ChangeTime.UseVisualStyleBackColor = true;
            this.ChangeTime.Click += new System.EventHandler(this.ChangeTime_Click);
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(372, 261);
            this.Controls.Add(this.ChangeTime);
            this.Controls.Add(this.WhiteList);
            this.Controls.Add(this.labelUserInfo);
            this.Controls.Add(this.label3);
            this.Controls.Add(this.label2);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.comboBox1);
            this.Controls.Add(this.Stop);
            this.Controls.Add(this.Start);
            this.Controls.Add(this.Status);
            this.Controls.Add(this.Compile);
            this.Controls.Add(this.Update);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedDialog;
            this.MaximizeBox = false;
            this.Name = "Form1";
            this.Text = "服务器启动工具";
            this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.Form1_FormClosing);
            this.Load += new System.EventHandler(this.Form1_Load);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Button Update;
        private System.Windows.Forms.Button Compile;
        private System.Windows.Forms.Button Status;
        private System.Windows.Forms.Button Start;
        private System.Windows.Forms.Button Stop;
        private System.Windows.Forms.ComboBox comboBox1;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.Label labelUserInfo;
        private System.Windows.Forms.Button WhiteList;
        private System.Windows.Forms.Button ChangeTime;
    }
}

