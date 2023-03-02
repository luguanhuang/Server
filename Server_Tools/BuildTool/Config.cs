using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;

namespace BuildTool
{
    class Config
    {
        public string branch;
        public string pull;
        public string build;
        public string run;
        public string show;
        public string pack;
        public string submit;

        public string code;
        public string runname;
        public string showname;

        public string basedir;
        public string basebranch;
        public string baseip;

        public HashSet<string> specialbranchs;
        public Dictionary<string, string> branch2dir;

        public Config()
        {
            specialbranchs = new HashSet<string>();
            branch2dir = new Dictionary<string, string>();
        }

        public void Init()
        {
            // branch
            string branch1 = "{\"dir\":\"" + basedir + "/resource\", \"task\":[\"git\", \"fetch\"]}";
            string branch2 = "{\"dir\":\"" + basedir + "/code\", \"task\":[\"git\", \"fetch\"]}";
            string branch3 = "{\"dir\":\"" + basedir + "/resource\", \"task\":[\"git\", \"branch\", \"-a\"]}";
            string branch4 = "{\"dir\":\"" + basedir + "/code\", \"task\":[\"git\", \"branch\", \"-a\"]}";
            branch = "{\"cmd\":\"tasks\", \"tasks\":[" + branch1 + "," + branch2 + "," + branch3 + "," + branch4 + "]}";

            // checkout .
            string pull1 = "{\"dir\":\"" + basedir + "/resource\", \"task\":[\"git\", \"checkout\"," + "\".\"" + "]}";
            string pull2 = "{\"dir\":\"" + basedir + "/code\", \"task\":[\"git\", \"checkout\"," + "\".\"" + "]}";
            // checkout branch
            string pull3 = "";
            string pull4 = "";
            if (specialbranchs.Contains(code))
            {
                pull3 = "{\"dir\":\"" + basedir + "/resource\", \"task\":[\"git\", \"checkout\"," + "\"" + code + "\"" + "]}";
                pull4 = "{\"dir\":\"" + basedir + "/code\", \"task\":[\"git\", \"checkout\"," + "\"" + code + "\"" + "]}";
            }
            else if (code == "release")
            {
                pull3 = "{\"dir\":\"" + basedir + "/resource\", \"task\":[\"git\", \"checkout\"," + "\"" + code + "-res\"" + "]}";
                pull4 = "{\"dir\":\"" + basedir + "/code\", \"task\":[\"git\", \"checkout\"," + "\"" + code + "-src\"" + "]}";
            }
            else
            {
                pull3 = "{\"dir\":\"" + basedir + "/resource\", \"task\":[\"git\", \"checkout\"," + "\"" + code + "\"" + "]}";
                pull4 = "{\"dir\":\"" + basedir + "/code\", \"task\":[\"git\", \"checkout\"," + "\"" + code + "\"" + "]}";
            }
            // pull
            if (code == "release")
            {
                string pull5 = "{\"dir\":\"" + basedir + "\", \"task\":[\"./pull.sh\"]}";
                pull = "{\"cmd\":\"tasks\", \"tasks\":[" + pull1 + "," + pull2 + "," + pull3 + "," + pull4 + "," + pull5 + "]}";
            }
            else
            {
                string pull5 = "{\"dir\":\"" + basedir + "/resource\", \"task\":[\"git\", \"pull\"]}";
                string pull6 = "{\"dir\":\"" + basedir + "/code\", \"task\":[\"git\", \"pull\"]}";
                pull = "{\"cmd\":\"tasks\", \"tasks\":[" + pull1 + "," + pull2 + "," + pull3 + "," + pull4 + "," + pull5 + "," + pull6 + "]}";
            }

            // build
            string build1 = "{\"dir\":\"" + basedir + "/code/server/Build/Debug\", \"task\":[\"touch\", \"" + basedir + "/code/server/CMakeLists.txt\"]}";
            string build2 = "{\"dir\":\"" + basedir + "/code/server/Build/Debug\", \"task\":[\"make\", \"-j3\"]}";
            build = "{\"cmd\":\"tasks\", \"tasks\":[" + build1 + "," + build2 + "]}";

            string show1 = "{\"dir\":\"" + basedir + "\", \"task\":[\"./show.py\", \"" + showname + "\"] }";
            show = "{\"cmd\":\"tasks\", \"tasks\":[" + show1 + "]}";

            string run1 = "{\"dir\":\"" + basedir + "/code/server/Build/Debug/bin\", \"task\":[\"./kill.py\", \"" + runname + "\"] }";
            run = "{\"cmd\":\"tasks\", \"tasks\":[" + run1 + "]}";

            string pack1 = "{\"dir\":\"" + basedir + "\", \"task\":[\"./mkdist.sh\"] }";
            pack = "{\"cmd\":\"tasks\", \"tasks\":[" + pack1 + "]}";

            string submit1 = "{\"dir\":\"" + basedir + "\", \"task\":[\"./submit.sh\"] }";
            submit = "{\"cmd\":\"tasks\", \"tasks\":[" + submit1 + "]}";
        }

        public void LoadFile()
        {
            StreamReader lines = new StreamReader("config.txt", Encoding.Default);
            baseip = lines.ReadLine();

            string str = lines.ReadLine();
            string[] values = str.Split(':');

            for (int i = 0; i < values.Length; ++i)
            {
                specialbranchs.Add(values[i]);
            }

            string tempstr = lines.ReadLine();
            while ("" != tempstr && null != tempstr)
            {
                string[] tempvalues = tempstr.Split(':');
                branch2dir[tempvalues[1]] = tempvalues[0];
                tempstr = lines.ReadLine();
            }


            lines.Close();
        }
        public void SetBase(string dir)
        {
            basedir = branch2dir[dir];
            basebranch = dir;
        }
    }
}
