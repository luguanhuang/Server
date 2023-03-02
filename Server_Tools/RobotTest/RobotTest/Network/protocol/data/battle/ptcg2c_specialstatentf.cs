using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/12/26 17:55:52

namespace XMainClient
{
    using DataType = KKSG.SpecialStateNtf;

	class PtcG2C_SpecialStateNtf : Protocol
	{
		public DataType Data = new DataType();

		public PtcG2C_SpecialStateNtf()
		{

		}

		public override uint GetProtoType()
		{
			return 11703;
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
			Process_PtcG2C_SpecialStateNtf.Process(this);
		}

	}
}
