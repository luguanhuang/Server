using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/6/25 15:42:19

namespace XMainClient
{
    using DataType = KKSG.ChatReport;

	class PtcC2G_ChatReport : Protocol
	{
		public DataType Data = new DataType();

		public PtcC2G_ChatReport()
		{
			Data.chatinfo = new KKSG.ChatInfo();

		}

		public override uint GetProtoType()
		{
			return 13571;
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
