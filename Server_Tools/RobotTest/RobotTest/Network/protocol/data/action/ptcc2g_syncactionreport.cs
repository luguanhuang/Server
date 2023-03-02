using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2015/12/14 11:09:08

namespace XMainClient
{
    using DataType = KKSG.ActionSyncData;

	class PtcC2G_SyncActionReport : Protocol
	{
		public DataType Data = new DataType();

		public PtcC2G_SyncActionReport()
		{

		}

		public override uint GetProtoType()
		{
			return 8028;
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
