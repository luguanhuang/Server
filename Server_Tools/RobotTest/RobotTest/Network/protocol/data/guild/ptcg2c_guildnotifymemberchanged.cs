using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2015/10/1 11:49:30

namespace XMainClient
{
    using DataType = KKSG.GuildMemberInfo;

	class PtcG2C_GuildNotifyMemberChanged : Protocol
	{
		public DataType Data = new DataType();

		public PtcG2C_GuildNotifyMemberChanged()
		{

		}

		public override uint GetProtoType()
		{
			return 5957;
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
			Process_PtcG2C_GuildNotifyMemberChanged.Process(this);
		}

	}
}
