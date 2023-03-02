using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2017/1/14 21:42:09

namespace XMainClient
{
    using DataType = KKSG.CloseLeagueEleNtf;

	class PtcC2M_CloseLeagueEleNtf : Protocol
	{
		public DataType Data = new DataType();

		public PtcC2M_CloseLeagueEleNtf()
		{

		}

		public override uint GetProtoType()
		{
			return 8195;
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
			throw new Exception("Send only protocol can not call process");
		}

	}
}
