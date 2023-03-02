using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2017/2/1 18:04:18

namespace XMainClient
{
    using DataType = KKSG.GuildAuctItemTime;

	class PtcM2C_GuildAuctItemTimeFresh : Protocol
	{
		public DataType Data = new DataType();

		public PtcM2C_GuildAuctItemTimeFresh()
		{

		}

		public override uint GetProtoType()
		{
			return 49239;
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
			Process_PtcM2C_GuildAuctItemTimeFresh.Process(this);
		}

	}
}
