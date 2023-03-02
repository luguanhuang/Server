using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2017/7/28 10:37:37

namespace XMainClient
{
    using DataType = KKSG.GroupChatApplyNtf;

	class PtcM2C_GroupChatApply : Protocol
	{
		public DataType Data = new DataType();

		public PtcM2C_GroupChatApply()
		{

		}

		public override uint GetProtoType()
		{
			return 34424;
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
			Process_PtcM2C_GroupChatApply.Process(this);
		}

	}
}
