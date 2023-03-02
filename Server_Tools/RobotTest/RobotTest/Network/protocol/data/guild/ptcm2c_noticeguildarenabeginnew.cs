using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/9/8 15:16:56

namespace XMainClient
{
    using DataType = KKSG.NoticeGuildArenaBegin;

	class PtcM2C_NoticeGuildArenaBeginNew : Protocol
	{
		public DataType Data = new DataType();

		public PtcM2C_NoticeGuildArenaBeginNew()
		{

		}

		public override uint GetProtoType()
		{
			return 12290;
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
			Process_PtcM2C_NoticeGuildArenaBeginNew.Process(this);
		}

	}
}
