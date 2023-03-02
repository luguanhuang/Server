using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2017/11/8 14:10:06

namespace XMainClient
{
    using DataType = KKSG.UpdateStageInfoNtf;

	class PtcG2C_UpdateStageInfoNtf : Protocol
	{
		public DataType Data = new DataType();

		public PtcG2C_UpdateStageInfoNtf()
		{

		}

		public override uint GetProtoType()
		{
			return 21189;
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
			Process_PtcG2C_UpdateStageInfoNtf.Process(this);
		}

	}
}
