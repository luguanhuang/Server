using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2017/9/25 9:41:26

namespace XMainClient
{
    using DataType = KKSG.SurviveKillPara;

	class PtcG2C_SurviveKillNtf : Protocol
	{
		public DataType Data = new DataType();

		public PtcG2C_SurviveKillNtf()
		{

		}

		public override uint GetProtoType()
		{
			return 51982;
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
			Process_PtcG2C_SurviveKillNtf.Process(this);
		}

	}
}
