using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2017/7/15 11:55:43

namespace XMainClient
{
    using DataType = KKSG.GroupChatDismissPtc;

	class PtcM2C_GroupChatDismiss : Protocol
	{
		public DataType Data = new DataType();

		public PtcM2C_GroupChatDismiss()
		{

		}

		public override uint GetProtoType()
		{
			return 18973;
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
			Process_PtcM2C_GroupChatDismiss.Process(this);
		}

	}
}
