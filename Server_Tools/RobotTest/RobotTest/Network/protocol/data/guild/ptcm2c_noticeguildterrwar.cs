using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/12/21 15:35:30

namespace XMainClient
{
    using DataType = KKSG.NoticeGuildTerrWar;

	class PtcM2C_NoticeGuildTerrWar : Protocol
	{
		public DataType Data = new DataType();

		public PtcM2C_NoticeGuildTerrWar()
		{

		}

		public override uint GetProtoType()
		{
			return 17274;
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
			Process_PtcM2C_NoticeGuildTerrWar.Process(this);
		}

	}
}
