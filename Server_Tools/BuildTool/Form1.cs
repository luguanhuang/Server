using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.Threading;
namespace BuildTool
{
    public partial class Form1 : Form
    {
        private UDP _udp;
        private Config _config;
        private Thread _thread;

        public Form1()
        {
            InitializeComponent();
            Control.CheckForIllegalCrossThreadCalls = false;
            richTextBox1.BackColor = Color.Black;
            richTextBox1.ForeColor = Color.Green;

            richTextBox2.BackColor = Color.Black;
            richTextBox2.ForeColor = Color.Yellow;

            _config = new Config();
            _config.LoadFile();

            foreach (KeyValuePair<string, string> i in _config.branch2dir)
            {
                comboBox1.Items.Add(i.Key);
            }
            comboBox1.SelectedIndex = 0;

            _udp = new UDP(_config.baseip, 31500);
            _udp.Bind(31501);

            _thread = new Thread(ShowTextThread);
            _thread.IsBackground = true;
            _thread.Start();
        }

        public void ShowTextThread()
        {
            while (true)
            {
                string output = _udp.Work();
                if (richTextBox1.InvokeRequired)
                {
                    Log log = JSON.parse<Log>(output);
                    Action<string> actionDelegate = (x) => {
                        if (log.cmd == "info")
                        {
                            richTextBox2.AppendText(log.data);
                            richTextBox2.AppendText(Environment.NewLine);
                        }
                        else if (log.cmd == "message")
                        {
                            richTextBox1.AppendText(log.data);
                            richTextBox1.AppendText(Environment.NewLine);
                        }
                        else
                        {
                            richTextBox1.AppendText(log.data);
                            richTextBox1.AppendText(Environment.NewLine);
                        }
                    };
                    this.richTextBox1.Invoke(actionDelegate, output);
                }
                else
                {
                    richTextBox1.AppendText(output);
                }
            }
            //_udp.Stop();
        }

        private void button1_Click(object sender, EventArgs e)
        {
            _config.code = textBox1.Text;
            _config.Init();
            _udp.Send(_config.pull);
        }

        private void button2_Click(object sender, EventArgs e)
        {
            _config.Init();
            _udp.Send(_config.build);
        }

        private void button4_Click(object sender, EventArgs e)
        {
            richTextBox1.Clear(); 
            richTextBox2.Clear(); 
        }

        private void richTextBox1_TextChanged(object sender, EventArgs e)
        {
            richTextBox1.SelectionStart = richTextBox1.Text.Length;
            richTextBox1.ScrollToCaret();
        }

        private void button5_Click(object sender, EventArgs e)
        {
            _config.showname = textBox3.Text;
            _config.Init();
            _udp.Send(_config.show);
        }

        private void button6_Click(object sender, EventArgs e)
        {
            _config.Init();
            _udp.Send(_config.branch);
        }

        private void button3_Click(object sender, EventArgs e)
        {
            _config.runname = textBox4.Text;
            _config.Init();
            _udp.Send(_config.run);
        }

        private void richTextBox2_TextChanged(object sender, EventArgs e)
        {
            richTextBox2.SelectionStart = richTextBox1.Text.Length;
            richTextBox2.ScrollToCaret();
        }

        private void button7_Click(object sender, EventArgs e)
        {
            _config.runname = "gateserver";
            _config.Init();
            _udp.Send(_config.run);

            _config.runname = "gameserver";
            _config.Init();
            _udp.Send(_config.run);

            _config.runname = "dbserver";
            _config.Init();
            _udp.Send(_config.run);
        }

        private void button9_Click(object sender, EventArgs e)
        {
            _config.Init();
            _udp.Send(_config.pack);
        }

        private void button8_Click(object sender, EventArgs e)
        {
            Thread updatethread = new Thread(UpdateThread);
            updatethread.IsBackground = true;
            updatethread.Start();
        }

        public void UpdateThread()
        {
            UDP tempudp = new UDP("42.62.22.46", 31600);
            tempudp.Bind(31601);
            tempudp.Send("run");

            richTextBox2.AppendText("update");
            richTextBox2.AppendText(Environment.NewLine);

            string output = tempudp.Work();
            richTextBox2.AppendText(output);
            richTextBox2.AppendText(Environment.NewLine);

            tempudp.Stop();
        }

        private void comboBox1_SelectedIndexChanged(object sender, EventArgs e)
        {
            _config.SetBase(comboBox1.SelectedItem.ToString());
            textBox1.Text = _config.basebranch;
            _config.Init();
        }

        private void button10_Click(object sender, EventArgs e)
        {
            _config.Init();
            _udp.Send(_config.submit);
        }
    }
}
