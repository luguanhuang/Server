using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/10/8 23:11:39

namespace XMainClient
{
    using DataType = KKSG.GiveUpQuestionNtf;

	class PtcC2M_GiveUpQAQuestionNtf : Protocol
	{
		public DataType Data = new DataType();

		public PtcC2M_GiveUpQAQuestionNtf()
		{

		}

		public override uint GetProtoType()
		{
			return 17022;
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
