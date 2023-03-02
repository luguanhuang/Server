using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/12/9 23:10:49

namespace XMainClient
{
    using DataType = KKSG.HorseFinal;

	class PtcG2C_HorseFinalNtf : Protocol
	{
		public DataType Data = new DataType();

		public PtcG2C_HorseFinalNtf()
		{

		}

		public override uint GetProtoType()
		{
			return 57969;
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
			Process_PtcG2C_HorseFinalNtf.Process(this);
		}

	}
}
