using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/10/9 11:18:43

namespace XMainClient
{
    using DataType = KKSG.PushQuestionNtf;

	class PtcM2C_PushQuestionNtf : Protocol
	{
		public DataType Data = new DataType();

		public PtcM2C_PushQuestionNtf()
		{

		}

		public override uint GetProtoType()
		{
			return 45138;
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
			Process_PtcM2C_PushQuestionNtf.Process(this);
		}

	}
}
