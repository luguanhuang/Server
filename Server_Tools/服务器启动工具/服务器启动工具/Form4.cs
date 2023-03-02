using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace 服务器启动工具
{
    public partial class Form4 : Form
    {
        private string m_inputTime;

        public Form4()
        {
            InitializeComponent();
            m_inputTime = "";
            timetext.Text = "YYYY/MM/DD HH:MM:SS";
        }

        public string GetInputTime()
        {
            return m_inputTime;
        }

        private void confirm_Click(object sender, EventArgs e)
        {
            string time = timetext.Text;
            if (time == "")
            {
                ShowTimeErrorFormate();
                return;
            }

            string[] strArr = time.Split(' ');
            if (strArr.Length != 2)
            {
                ShowTimeErrorFormate();
                return;
            }
            string day = strArr[0];
            string[] dayArr = day.Split('/');
            if (dayArr.Length != 3)
            {
                ShowTimeErrorFormate();
                return;
            }
            if (Convert.ToInt32(dayArr[0]) < 2017 || Convert.ToInt32(dayArr[1]) < 1 || Convert.ToInt32(dayArr[1]) > 12 || Convert.ToInt32(dayArr[2]) < 1 || Convert.ToInt32(dayArr[2]) > 31)
            {
                ShowTimeErrorFormate();
                return;
            }

            day = strArr[1];
            dayArr = null;
            dayArr = day.Split(':');
            if (dayArr.Length != 3)
            {
                ShowTimeErrorFormate();
                return;
            }
            if (Convert.ToInt32(dayArr[0]) < 0 || Convert.ToInt32(dayArr[0]) >= 24 ||Convert.ToInt32(dayArr[1]) < 0 || Convert.ToInt32(dayArr[1]) >= 60 || Convert.ToInt32(dayArr[2]) < 0 || Convert.ToInt32(dayArr[2]) >= 60)
            {
                ShowTimeErrorFormate();
                return;
            }

            m_inputTime = time;
            this.DialogResult = DialogResult.OK;
            return;
        }

        private void cancel_Click(object sender, EventArgs e)
        {
            this.DialogResult = DialogResult.No;
            return;
        }

        private void resetcur_Click(object sender, EventArgs e)
        {
            m_inputTime = "now";
            this.DialogResult = DialogResult.OK;
            return;
        }

        private void ShowTimeErrorFormate()
        {
            MessageBox.Show("error, need input time format:  year/month/day hour:minute:second");
        }

    }
}
