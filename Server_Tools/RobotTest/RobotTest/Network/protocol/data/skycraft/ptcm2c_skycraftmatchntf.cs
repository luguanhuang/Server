using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2017/4/1 14:35:50

namespace XMainClient
{
    using DataType = KKSG.SkyCraftMatchNtf;

	class PtcM2C_SkyCraftMatchNtf : Protocol
	{
		public DataType Data = new DataType();

		public PtcM2C_SkyCraftMatchNtf()
		{

		}

		public override uint GetProtoType()
		{
			return 4938;
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
			Process_PtcM2C_SkyCraftMatchNtf.Process(this);
		}

	}
}
