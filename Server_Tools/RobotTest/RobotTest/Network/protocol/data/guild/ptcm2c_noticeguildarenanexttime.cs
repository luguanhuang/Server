using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/10/17 14:11:48

namespace XMainClient
{
    using DataType = KKSG.NoticeGuildArenaNextTime;

	class PtcM2C_NoticeGuildArenaNextTime : Protocol
	{
		public DataType Data = new DataType();

		public PtcM2C_NoticeGuildArenaNextTime()
		{

		}

		public override uint GetProtoType()
		{
			return 21612;
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
			Process_PtcM2C_NoticeGuildArenaNextTime.Process(this);
		}

	}
}
