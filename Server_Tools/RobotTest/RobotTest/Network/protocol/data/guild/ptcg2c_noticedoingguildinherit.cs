using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/11/18 21:08:43

namespace XMainClient
{
    using DataType = KKSG.NoticeDoingGuildInherit;

	class PtcG2C_NoticeDoingGuildInherit : Protocol
	{
		public DataType Data = new DataType();

		public PtcG2C_NoticeDoingGuildInherit()
		{

		}

		public override uint GetProtoType()
		{
			return 61639;
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
			Process_PtcG2C_NoticeDoingGuildInherit.Process(this);
		}

	}
}
