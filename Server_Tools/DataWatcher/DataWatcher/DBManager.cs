using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using MySql.Data.MySqlClient;
using System.Windows.Forms;

namespace DataWatcher
{
    struct DBInfo
    {
        public string server;
        public string uid;
        public string password;
        public string database;
        public string charset;
    }

    class Blob
    {
        public byte[] buff;
        public long size;
    }


    class DBManager
    {
        public static DBManager handler;

        private DBInfo _dbinfo;
        private MySqlConnection _conn;

        static DBManager()
        {
            handler = new DBManager();
        }

        public void SetDBInfo(DBInfo info)
        {
            _dbinfo = info;
        }

        public void Open()
        {
            StringBuilder sb = new StringBuilder();
            sb.Append("Server=" + _dbinfo.server + ";");
            sb.Append("Uid=" + _dbinfo.uid + ";");
            sb.Append("Pwd=" + _dbinfo.password + ";");
            sb.Append("Database=" + _dbinfo.database + ";");
            sb.Append("Charset=" + _dbinfo.charset + ";");

            _conn = new MySqlConnection(sb.ToString());
            _conn.Open();
        }

        public void Close()
        {
            if (_conn != null)
            {
                _conn.Close();
                _conn = null;
            }
        }

        public List<string> TestQuery()
        {
            List<string> data = new List<string>();
            string roleinfo = "roleid | rolename | account | level | profession ";
            data.Add(roleinfo);

            string query = "select * from rolestate where rolename is not null limit 20";
            MySqlCommand cmd = new MySqlCommand(query, _conn);
            MySqlDataReader reader = cmd.ExecuteReader();
            while (reader.Read())
            {
                roleinfo = "";
                roleinfo += reader["roleid"];
                roleinfo += " | ";
                roleinfo += reader["rolename"];
                roleinfo += " | ";
                roleinfo += reader["account"];
                roleinfo += " | ";
                roleinfo += reader["level"];
                roleinfo += " | ";
                roleinfo += reader["profession"];
                data.Add(roleinfo);
            }
            reader.Close();
            return data;
        }


        public Blob SearchRole(string roleID)
        {
            string query = "select * from role where _id = @roleid";
            MySqlCommand cmd = new MySqlCommand(query, _conn);
            cmd.Parameters.AddWithValue("@roleid", roleID);
            MySqlDataReader reader = cmd.ExecuteReader();
            if (!reader.HasRows)
            {
                reader.Close();
                return null;
            }

            reader.Read();
            int size = 10240;
            Blob blob = new Blob();
            blob.buff = new byte[size];
            blob.size = reader.GetBytes(reader.GetOrdinal("data"), 0, blob.buff, 0, size);
            reader.Close();
            return blob;
        }
    }
}
