using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2017/9/24 19:38:51

namespace XMainClient
{
    using DataType = KKSG.SurviveResultPara;

	class PtcG2C_SurviveResultNtf : Protocol
	{
		public DataType Data = new DataType();

		public PtcG2C_SurviveResultNtf()
		{

		}

		public override uint GetProtoType()
		{
			return 55674;
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
			Process_PtcG2C_SurviveResultNtf.Process(this);
		}

	}
}
