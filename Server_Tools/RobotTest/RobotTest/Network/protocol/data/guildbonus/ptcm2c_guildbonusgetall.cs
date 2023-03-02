using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/12/24 11:49:51

namespace XMainClient
{
    using DataType = KKSG.GuildBonusGetAllData;

	class PtcM2C_GuildBonusGetAll : Protocol
	{
		public DataType Data = new DataType();

		public PtcM2C_GuildBonusGetAll()
		{

		}

		public override uint GetProtoType()
		{
			return 55177;
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
			Process_PtcM2C_GuildBonusGetAll.Process(this);
		}

	}
}
