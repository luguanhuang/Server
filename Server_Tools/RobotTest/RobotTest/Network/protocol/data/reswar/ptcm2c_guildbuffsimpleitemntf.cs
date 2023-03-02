using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/11/12 15:27:06

namespace XMainClient
{
    using DataType = KKSG.GuildBuffSimpleItem;

	class PtcM2C_GuildBuffSimpleItemNtf : Protocol
	{
		public DataType Data = new DataType();

		public PtcM2C_GuildBuffSimpleItemNtf()
		{

		}

		public override uint GetProtoType()
		{
			return 63964;
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
			Process_PtcM2C_GuildBuffSimpleItemNtf.Process(this);
		}

	}
}
