using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2017/5/6 22:16:17

namespace XMainClient
{
    using DataType = KKSG.GuildCampPartyTradeNotifyArg;

	class PtcG2C_GuildCampPartyTradeNotify : Protocol
	{
		public DataType Data = new DataType();

		public PtcG2C_GuildCampPartyTradeNotify()
		{

		}

		public override uint GetProtoType()
		{
			return 62988;
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
			Process_PtcG2C_GuildCampPartyTradeNotify.Process(this);
		}

	}
}
