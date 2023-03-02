using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/7/4 16:09:01

namespace XMainClient
{
    using DataType = KKSG.FMDArg;

	class PtcG2C_fastMBDismissNtf : Protocol
	{
		public DataType Data = new DataType();

		public PtcG2C_fastMBDismissNtf()
		{

		}

		public override uint GetProtoType()
		{
			return 49087;
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
			Process_PtcG2C_fastMBDismissNtf.Process(this);
		}

	}
}
