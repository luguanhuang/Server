using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using KKSG;
using ProtoBuf;
using System.IO;

namespace DataWatcher
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();

            DBInfo info;
            info.server = "10.0.128.229";
            info.uid = "root";
            info.password = "";
            info.database = "world20";
            info.charset = "utf8";
            DBManager.handler.SetDBInfo(info);
            try
            {
                DBManager.handler.Open();
            }
            catch (MySql.Data.MySqlClient.MySqlException ex)
            {
                MessageBox.Show(ex.Message);
            }
        }

        private void button1_Click(object sender, EventArgs e)
        {
            Blob data = DBManager.handler.SearchRole(textBox1.Text);
            if (data == null)
            {
                return;
            }

            MemoryStream ms = new MemoryStream(data.buff, 0, (int)data.size);
            RoleAllInfo info = Serializer.Deserialize<RoleAllInfo>(ms);
            propertyGrid1.SelectedObject = info;
        }
    }
}
