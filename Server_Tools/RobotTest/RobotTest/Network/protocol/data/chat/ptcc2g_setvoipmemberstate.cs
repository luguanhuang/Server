using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/9/9 15:44:18

namespace XMainClient
{
    using DataType = KKSG.SetVoipMemberState;

	class PtcC2G_SetVoipMemberState : Protocol
	{
		public DataType Data = new DataType();

		public PtcC2G_SetVoipMemberState()
		{

		}

		public override uint GetProtoType()
		{
			return 3881;
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
