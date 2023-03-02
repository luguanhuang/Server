using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/10/1 21:54:26

namespace XMainClient
{
    using DataType = KKSG.NoticeGuildLadderStart;

	class PtcM2C_NoticeGuildLadderStart : Protocol
	{
		public DataType Data = new DataType();

		public PtcM2C_NoticeGuildLadderStart()
		{

		}

		public override uint GetProtoType()
		{
			return 49782;
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
			Process_PtcM2C_NoticeGuildLadderStart.Process(this);
		}

	}
}
