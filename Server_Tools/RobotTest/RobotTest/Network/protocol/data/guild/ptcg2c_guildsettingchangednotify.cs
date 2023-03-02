using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2015/10/1 11:52:58

namespace XMainClient
{
    using DataType = KKSG.GuildSettingChanged;

	class PtcG2C_GuildSettingChangedNotify : Protocol
	{
		public DataType Data = new DataType();

		public PtcG2C_GuildSettingChangedNotify()
		{

		}

		public override uint GetProtoType()
		{
			return 63721;
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
			Process_PtcG2C_GuildSettingChangedNotify.Process(this);
		}

	}
}
