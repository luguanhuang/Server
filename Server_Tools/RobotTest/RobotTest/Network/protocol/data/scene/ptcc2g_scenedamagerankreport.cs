using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2015/12/24 17:43:22

namespace XMainClient
{
    //using DataType = KKSG.;

	class PtcC2G_SceneDamageRankReport : Protocol
	{
		//public DataType Data = new DataType();

		public PtcC2G_SceneDamageRankReport()
		{

		}

		public override uint GetProtoType()
		{
			return 53015;
		}

		public override void Serialize(MemoryStream stream)
		{
			//Serializer.Serialize(stream, Data);
		}

		public override void DeSerialize(MemoryStream stream)
		{
			//Data = Serializer.Deserialize<DataType>(stream);
		}

		public override void Process()
		{
			throw new Exception("Send only protocol can not call process");
		}

	}
}
