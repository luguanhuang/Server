using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2017/7/15 11:52:56

namespace XMainClient
{
    using DataType = KKSG.GroupChatManagerPtc;

	class PtcM2C_GroupChatManager : Protocol
	{
		public DataType Data = new DataType();

		public PtcM2C_GroupChatManager()
		{

		}

		public override uint GetProtoType()
		{
			return 17710;
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
			Process_PtcM2C_GroupChatManager.Process(this);
		}

	}
}
