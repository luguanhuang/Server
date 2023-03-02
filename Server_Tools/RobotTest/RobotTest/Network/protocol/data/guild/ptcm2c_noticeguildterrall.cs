using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/12/21 15:07:46

namespace XMainClient
{
    using DataType = KKSG.NoticeGuildTerrall;

	class PtcM2C_NoticeGuildTerrall : Protocol
	{
		public DataType Data = new DataType();

		public PtcM2C_NoticeGuildTerrall()
		{

		}

		public override uint GetProtoType()
		{
			return 7704;
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
			Process_PtcM2C_NoticeGuildTerrall.Process(this);
		}

	}
}
