using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/10/8 22:42:35

namespace XMainClient
{
    using DataType = KKSG.CommitAnswerNtf;

	class PtcC2M_CommitAnswerNtf : Protocol
	{
		public DataType Data = new DataType();

		public PtcC2M_CommitAnswerNtf()
		{

		}

		public override uint GetProtoType()
		{
			return 12159;
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
			throw new Exception("Send only protocol can not call process");
		}

	}
}
