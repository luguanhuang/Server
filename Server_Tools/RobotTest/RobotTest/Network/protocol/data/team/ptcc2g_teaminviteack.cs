using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2015/11/30 19:33:14

namespace XMainClient
{
    using DataType = KKSG.TeamInviteAck;

	class PtcC2G_TeamInviteAck : Protocol
	{
		public DataType Data = new DataType();

		public PtcC2G_TeamInviteAck()
		{

		}

		public override uint GetProtoType()
		{
			return 38721;
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
