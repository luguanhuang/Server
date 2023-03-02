using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/7/4 11:50:20

namespace XMainClient
{
    using DataType = KKSG.FMBArg;

	class PtcG2C_FastMBConfirmNtf : Protocol
	{
		public DataType Data = new DataType();

		public PtcG2C_FastMBConfirmNtf()
		{

		}

		public override uint GetProtoType()
		{
			return 51623;
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
			Process_PtcG2C_FastMBConfirmNtf.Process(this);
		}

	}
}
