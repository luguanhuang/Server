using System;
using System.Collections.Generic;

using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2014/11/8 14:19:55

namespace XMainClient
{
    using DataType = KKSG.SyncTimeTest;

	class PtcC2G_TestSyncTime : Protocol
	{
		public DataType Data = new DataType();

		public PtcC2G_TestSyncTime()
		{
		}

		public override uint GetProtoType()
		{
			return 54999;
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
