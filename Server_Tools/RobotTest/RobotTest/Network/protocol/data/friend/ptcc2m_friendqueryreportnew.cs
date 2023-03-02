using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2017/3/15 10:55:57

namespace XMainClient
{
    using DataType = KKSG.FriendQueryReportNew;

	class PtcC2M_FriendQueryReportNew : Protocol
	{
		public DataType Data = new DataType();

		public PtcC2M_FriendQueryReportNew()
		{

		}

		public override uint GetProtoType()
		{
			return 15079;
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
