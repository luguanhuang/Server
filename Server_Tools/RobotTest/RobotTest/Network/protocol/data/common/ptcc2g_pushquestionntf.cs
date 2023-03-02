using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/6/23 12:55:30

namespace XMainClient
{
    using DataType = KKSG.PushQuestionNtf;

	class PtcC2G_PushQuestionNtf : Protocol
	{
		public DataType Data = new DataType();

		public PtcC2G_PushQuestionNtf()
		{

		}

		public override uint GetProtoType()
		{
			return 64446;
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
