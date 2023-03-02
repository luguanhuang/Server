using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/8/26 15:01:41

namespace XMainClient
{
    using DataType = KKSG.GuildSkillAllData;

	class PtcM2C_SendGuildSkillInfo : Protocol
	{
		public DataType Data = new DataType();

		public PtcM2C_SendGuildSkillInfo()
		{

		}

		public override uint GetProtoType()
		{
			return 55907;
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
			Process_PtcM2C_SendGuildSkillInfo.Process(this);
		}

	}
}
