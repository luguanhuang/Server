namespace 服务器启动工具
{
    partial class Form4
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.label1 = new System.Windows.Forms.Label();
            this.timetext = new System.Windows.Forms.TextBox();
            this.resetcur = new System.Windows.Forms.Button();
            this.confirm = new System.Windows.Forms.Button();
            this.cancel = new System.Windows.Forms.Button();
            this.SuspendLayout();
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(46, 48);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(59, 12);
            this.label1.TabIndex = 8;
            this.label1.Text = "修改时间:";
            // 
            // timetext
            // 
            this.timetext.Location = new System.Drawing.Point(111, 45);
            this.timetext.Name = "timetext";
            this.timetext.Size = new System.Drawing.Size(126, 21);
            this.timetext.TabIndex = 9;
            this.timetext.Tag = "";
            // 
            // resetcur
            // 
            this.resetcur.Location = new System.Drawing.Point(158, 154);
            this.resetcur.Name = "resetcur";
            this.resetcur.Size = new System.Drawing.Size(89, 28);
            this.resetcur.TabIndex = 10;
            this.resetcur.Text = "置为当前时间";
            this.resetcur.UseVisualStyleBackColor = true;
            this.resetcur.Click += new System.EventHandler(this.resetcur_Click);
            // 
            // confirm
            // 
            this.confirm.Location = new System.Drawing.Point(57, 90);
            this.confirm.Name = "confirm";
            this.confirm.Size = new System.Drawing.Size(68, 28);
            this.confirm.TabIndex = 11;
            this.confirm.Text = "确定";
            this.confirm.UseVisualStyleBackColor = true;
            this.confirm.Click += new System.EventHandler(this.confirm_Click);
            // 
            // cancel
            // 
            this.cancel.Location = new System.Drawing.Point(147, 90);
            this.cancel.Name = "cancel";
            this.cancel.Size = new System.Drawing.Size(68, 28);
            this.cancel.TabIndex = 12;
            this.cancel.Text = "取消";
            this.cancel.UseVisualStyleBackColor = true;
            this.cancel.Click += new System.EventHandler(this.cancel_Click);
            // 
            // Form4
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(264, 217);
            this.Controls.Add(this.cancel);
            this.Controls.Add(this.confirm);
            this.Controls.Add(this.resetcur);
            this.Controls.Add(this.timetext);
            this.Controls.Add(this.label1);
            this.Name = "Form4";
            this.Text = "调时间";
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.TextBox timetext;
        private System.Windows.Forms.Button resetcur;
        private System.Windows.Forms.Button confirm;
        private System.Windows.Forms.Button cancel;
    }
}