using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/11/3 16:21:19

namespace XMainClient
{
    using DataType = KKSG.ResWarGuildBrief;

	class PtcM2C_ResWarGuildBriefNtf : Protocol
	{
		public DataType Data = new DataType();

		public PtcM2C_ResWarGuildBriefNtf()
		{

		}

		public override uint GetProtoType()
		{
			return 35338;
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
			Process_PtcM2C_ResWarGuildBriefNtf.Process(this);
		}

	}
}
