using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2015/10/21 23:17:45

namespace XMainClient
{
    using DataType = KKSG.OnlineRewardReport;

	class PtcC2G_OnlineRewardReport : Protocol
	{
		public DataType Data = new DataType();

		public PtcC2G_OnlineRewardReport()
		{

		}

		public override uint GetProtoType()
		{
			return 36178;
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
