using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/8/16 10:10:48

namespace XMainClient
{
    using DataType = KKSG.TeamChanged;

	class PtcM2C_TeamChangeM2CNtf : Protocol
	{
		public DataType Data = new DataType();

		public PtcM2C_TeamChangeM2CNtf()
		{

		}

		public override uint GetProtoType()
		{
			return 53586;
		}

		public override void Serialize(MemoryStream stream)
		{
			Serializer.Serialize(stream, Data);
		}

		public override void DeSerialize(MemoryStream stream)
		{
			Data = Serializer.Deserialize<DataType>(stream);
		}

		public override void Process()
		{
			Process_PtcM2C_TeamChangeM2CNtf.Process(this);
		}

	}
}
