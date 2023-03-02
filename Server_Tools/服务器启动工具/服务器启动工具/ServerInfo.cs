using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Xml;

namespace 服务器启动工具
{
    class WhiteListCmd
    {
        public string m_bkcmd;
        public string m_addcmd;
        public string m_reloadcmd;
        public WhiteListCmd()
        {

        }

        public bool ReadCommand(XmlNode node)
        {
            if (node == null) return false;

            XmlElement elem = (XmlElement)node;
            m_bkcmd = elem.GetAttribute("cmd");
            m_addcmd = elem.GetAttribute("varcmd");
            m_reloadcmd = elem.GetAttribute("finalcmd");

            return true;
        }
    }
    class ServerInfo
    {
        public string m_strName;
        public string m_strIP;
        public string m_strPort;
        public string m_strUser;
        public string m_strPwd;

        public string m_strInitCmd;
        public string m_strUpdateCmd;
        public string m_strCompileCmd;
        public string m_strStartCmd;
        public string m_strStopCmd;
        public string m_strStatusCmd;
        public string m_strChangeTimeCmd;
        public WhiteListCmd m_strWhiteListCmd;

        public ServerInfo()
        {
        }
    }
}
