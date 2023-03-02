using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/10/3 16:25:41

namespace XMainClient
{
    using DataType = KKSG.GmfInspirePara;

	class PtcG2C_GmfInpireNtf : Protocol
	{
		public DataType Data = new DataType();

		public PtcG2C_GmfInpireNtf()
		{

		}

		public override uint GetProtoType()
		{
			return 16122;
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
			Process_PtcG2C_GmfInpireNtf.Process(this);
		}

	}
}
