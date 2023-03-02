using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/8/2 8:13:02

namespace XMainClient
{
    using DataType = KKSG.ChatOfflineNotify;

	class PtcM2C_MCChatOffLineNotify : Protocol
	{
		public DataType Data = new DataType();

		public PtcM2C_MCChatOffLineNotify()
		{

		}

		public override uint GetProtoType()
		{
			return 35008;
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
			Process_PtcM2C_MCChatOffLineNotify.Process(this);
		}

	}
}
