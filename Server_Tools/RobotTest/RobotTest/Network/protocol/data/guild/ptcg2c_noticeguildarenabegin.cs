using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/7/23 19:32:57

namespace XMainClient
{
    using DataType = KKSG.NoticeGuildArenaBegin;

	class PtcG2C_NoticeGuildArenaBegin : Protocol
	{
		public DataType Data = new DataType();

		public PtcG2C_NoticeGuildArenaBegin()
		{

		}

		public override uint GetProtoType()
		{
			return 11695;
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
			Process_PtcG2C_NoticeGuildArenaBegin.Process(this);
		}

	}
}
