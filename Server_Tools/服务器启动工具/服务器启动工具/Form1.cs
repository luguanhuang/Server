using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.Runtime.InteropServices;
using System.Xml;
using System.Threading;
using System.IO;

namespace 服务器启动工具
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
        }

        uint WM_CHAR = 0x0102;
        [DllImport("User32.dll", EntryPoint = "PostMessage")]
        //public static extern int SendMessage(IntPtr hWnd, uint Msg, uint wParam, uint lParam);
        public static extern bool PostMessage(IntPtr hWnd, uint Msg, uint wParam, uint lParam);

        System.Diagnostics.Process m_oShell = null;
        List<ServerInfo> m_oServerList = new List<ServerInfo>();
        int m_ping = 1000;

        private void button1_Click(object sender, EventArgs e)
        {
            SendCommand("ls");
        }

        private void Form1_Load(object sender, EventArgs e)
        {
            XmlDocument doc = new XmlDocument();
            try
            {
                doc.Load("server_conf.xml");
            }
            catch (System.Exception ex)
            {
                MessageBox.Show(ex.Message);
                return;
            }

            XmlNode root = doc.SelectSingleNode("ServerList");
            XmlElement rootElem = (XmlElement)root;
            m_ping = Convert.ToInt32(rootElem.GetAttribute("ping"));
            foreach (XmlNode serverNode in root.ChildNodes)
            {
                if (serverNode.NodeType == XmlNodeType.Comment) continue;

                XmlElement elem = (XmlElement)serverNode;
                ServerInfo oInfo = new ServerInfo();
                oInfo.m_strName = elem.GetAttribute("name");
                oInfo.m_strIP   = elem.GetAttribute("ip");
                oInfo.m_strPort = elem.GetAttribute("port");
                oInfo.m_strUser = elem.GetAttribute("user");
                oInfo.m_strPwd  = elem.GetAttribute("pwd");

                oInfo.m_strInitCmd    = ReadCommand(elem.SelectSingleNode("init"));
                oInfo.m_strUpdateCmd    = ReadCommand(elem.SelectSingleNode("update"));
                oInfo.m_strCompileCmd = ReadCommand(elem.SelectSingleNode("compile"));
                oInfo.m_strStartCmd = ReadCommand(elem.SelectSingleNode("start"));
                oInfo.m_strStopCmd = ReadCommand(elem.SelectSingleNode("stop"));
                oInfo.m_strStatusCmd = ReadCommand(elem.SelectSingleNode("status"));
                oInfo.m_strChangeTimeCmd = ReadCommand(elem.SelectSingleNode("modifytime"));
                oInfo.m_strWhiteListCmd = new WhiteListCmd();
                oInfo.m_strWhiteListCmd.ReadCommand(elem.SelectSingleNode("whitelist"));

                m_oServerList.Add(oInfo);
                this.comboBox1.Items.Add(oInfo.m_strName);
            }
        }

        private string ReadCommand(XmlNode node)
        {
            if (node == null) return "";

            XmlElement elem = (XmlElement)node;
            return elem.GetAttribute("cmd");
        }

        private string ReadVarCommand(XmlNode node)
        {
            if (node == null) return "";

            XmlElement elem = (XmlElement)node;
            return elem.GetAttribute("varcmd");
        }

        void ConnectToServer(ServerInfo roInfo)
        {
            if (m_oShell != null)
            {
                try
                {
                    m_oShell.Kill();
                    m_oShell = null;
                }
                catch (System.Exception ex)
                {
                    MessageBox.Show(ex.Message);
                    m_oShell = null;
                }
            }

            string strCmdLine = "-P " + roInfo.m_strPort + " -pw " + roInfo.m_strPwd + " " + roInfo.m_strUser + "@" + roInfo.m_strIP;
            m_oShell = System.Diagnostics.Process.Start("putty.exe", strCmdLine);
        }

        private void SendCommand(string cmd)
        {
            for(int i = 0; i < cmd.Length; ++i)
            {
                PostMessage(m_oShell.MainWindowHandle, WM_CHAR, (uint)cmd[i], 0);
            }
            PostMessage(m_oShell.MainWindowHandle, WM_CHAR, (uint)'\r', 0);
        }

        private void Form1_FormClosing(object sender, FormClosingEventArgs e)
        {
            try
            {
                if (m_oShell != null)
                {
                    m_oShell.Kill();
                }
            }
            catch (System.Exception ex)
            {
                MessageBox.Show(ex.Message);
            }
        }

        private void comboBox1_SelectedIndexChanged(object sender, EventArgs e)
        {
            ServerInfo oInfo = GetCurrentServer();
            if (oInfo == null) return;

            ConnectToServer(oInfo);
            //Thread.Sleep(600);
            Thread.Sleep(m_ping);
            SendCommand(oInfo.m_strInitCmd);
            this.labelUserInfo.Text = oInfo.m_strUser + "@" + oInfo.m_strIP;
        }

        private ServerInfo GetCurrentServer()
        {
            int nIndex = this.comboBox1.SelectedIndex;
            if (nIndex == -1) return null;
            if (nIndex >= m_oServerList.Count) return null;

            return m_oServerList[nIndex];
        }

        private void Start_Click(object sender, EventArgs e)
        {
            if(m_oShell == null)
            {
                MessageBox.Show("error, need select server");
                return;
            }
            ServerInfo oInfo = GetCurrentServer();
            SendCommand(oInfo.m_strStartCmd);
        }

        private void Stop_Click(object sender, EventArgs e)
        {
            if(m_oShell == null)
            {
                MessageBox.Show("error, need select server");
                return;
            }
            ServerInfo oInfo = GetCurrentServer();
            SendCommand(oInfo.m_strStopCmd);
        }

        private void Status_Click(object sender, EventArgs e)
        {
            if(m_oShell == null)
            {
                MessageBox.Show("error, need select server");
                return;
            }
            ServerInfo oInfo = GetCurrentServer();
            SendCommand(oInfo.m_strStatusCmd);
        }

        private void Update_Click(object sender, EventArgs e)
        {
            if(m_oShell == null)
            {
                MessageBox.Show("error, need select server");
                return;
            }
            ServerInfo oInfo = GetCurrentServer();
            SendCommand(oInfo.m_strUpdateCmd);
        }

        private void Compile_Click(object sender, EventArgs e)
        {
            if(m_oShell == null)
            {
                MessageBox.Show("error, need select server");
                return;
            }
            ServerInfo oInfo = GetCurrentServer();
            SendCommand(oInfo.m_strCompileCmd);
        }

        private void WhiteList_Click(object sender, EventArgs e)
        {
            if(m_oShell == null)
            {
                MessageBox.Show("error, need select server");
                return;
            }

            ServerInfo oInfo = GetCurrentServer();
            OpenFileDialog dlg = new OpenFileDialog();
            dlg.Filter = "(txt file)|*.txt";
            if (dlg.ShowDialog() != DialogResult.OK)
            {
                MessageBox.Show("error, need select file");
                return;
            }

            if (MessageBox.Show("select file  " + dlg.FileName + " ?", "是否确认", MessageBoxButtons.OKCancel) != DialogResult.OK)
                return;

            HashSet<string> whitelist = new HashSet<string>();
            StreamReader r = new StreamReader(dlg.FileName, Encoding.Default);
            string line;
            while((line = r.ReadLine()) != null)
            {
                whitelist.Add(line);
            }

            SendCommand(oInfo.m_strWhiteListCmd.m_bkcmd);
            Thread.Sleep(500);

            int count = 0;
            string varCmd = oInfo.m_strWhiteListCmd.m_addcmd;
            foreach(var i in whitelist)
            {
                string echoCmd = varCmd.Replace("$var", i);
                SendCommand(echoCmd);
                ++count;
                if (count == 100)
                {
                    Thread.Sleep(2000);
                    count = 0;
                }
            }

            Thread.Sleep(1500);
            SendCommand(oInfo.m_strWhiteListCmd.m_reloadcmd);
        }

        private void ChangeTime_Click(object sender, EventArgs e)
        {
            if(m_oShell == null)
            {
                MessageBox.Show("error, need select server");
                return;
            }

            if (MessageBox.Show("确认需要调时间吗？往回调时间需要自行重启服务器", "是否确认", MessageBoxButtons.OKCancel) != DialogResult.OK)
                return;

            ServerInfo oInfo = GetCurrentServer();
            ConnectToServer(oInfo);
            Thread.Sleep(m_ping);

            Form4 changeTime = new Form4();
            changeTime.Tag = "";
            changeTime.StartPosition = FormStartPosition.CenterScreen;
            if (changeTime.ShowDialog() == DialogResult.OK)
            {
                string inputTime = changeTime.GetInputTime();
                if (inputTime.Equals("now"))
                {
                    System.DateTime currentTime = new System.DateTime();
                    currentTime = System.DateTime.Now;
                    string day = currentTime.ToString("d");
                    string time = currentTime.ToString("T");
                    inputTime = day + " " + time;
                    SendCommand(oInfo.m_strChangeTimeCmd + "\"" + inputTime + "\"");
                    //MessageBox.Show(oInfo.m_strChangeTimeCmd + "\"" + inputTime + "\"");
                }
                else
                {
                    SendCommand(oInfo.m_strChangeTimeCmd + "\"" + inputTime + "\"");
                    //MessageBox.Show(oInfo.m_strChangeTimeCmd + "\"" + inputTime + "\"");
                }
            }

            SendCommand(oInfo.m_strInitCmd);
        }

    }
}
