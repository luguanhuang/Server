using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/7/4 11:52:17

namespace XMainClient
{
    using DataType = KKSG.FMDArg;

	class PtcG2C_FashMBDismissNtf : Protocol
	{
		public DataType Data = new DataType();

		public PtcG2C_FashMBDismissNtf()
		{

		}

		public override uint GetProtoType()
		{
			return 8301;
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
			Process_PtcG2C_FashMBDismissNtf.Process(this);
		}

	}
}
