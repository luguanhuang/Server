using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/11/1 16:16:34

namespace XMainClient
{
    using DataType = KKSG.ResWarAllTeamBaseInfo;

	class PtcG2C_ResWarTeamNtf : Protocol
	{
		public DataType Data = new DataType();

		public PtcG2C_ResWarTeamNtf()
		{

		}

		public override uint GetProtoType()
		{
			return 40701;
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
			Process_PtcG2C_ResWarTeamNtf.Process(this);
		}

	}
}
