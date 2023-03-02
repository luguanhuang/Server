using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/9/15 22:39:17

namespace XMainClient
{
    using DataType = KKSG.GuildSettingChanged;

	class PtcM2C_GuildSetingNotify : Protocol
	{
		public DataType Data = new DataType();

		public PtcM2C_GuildSetingNotify()
		{

		}

		public override uint GetProtoType()
		{
			return 21944;
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
			Process_PtcM2C_GuildSetingNotify.Process(this);
		}

	}
}
