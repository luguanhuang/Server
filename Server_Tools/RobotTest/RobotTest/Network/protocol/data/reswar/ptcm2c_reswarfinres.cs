using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/11/1 20:25:08

namespace XMainClient
{
    using DataType = KKSG.ResWarFinalInfo;

	class PtcM2C_ResWarFinRes : Protocol
	{
		public DataType Data = new DataType();

		public PtcM2C_ResWarFinRes()
		{

		}

		public override uint GetProtoType()
		{
			return 16904;
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
			Process_PtcM2C_ResWarFinRes.Process(this);
		}

	}
}
