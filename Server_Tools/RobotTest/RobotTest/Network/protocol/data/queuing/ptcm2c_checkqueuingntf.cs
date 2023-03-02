using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/8/23 14:29:59

namespace XMainClient
{
    using DataType = KKSG.CheckQueuingNtf;

	class PtcM2C_CheckQueuingNtf : Protocol
	{
		public DataType Data = new DataType();

		public PtcM2C_CheckQueuingNtf()
		{

		}

		public override uint GetProtoType()
		{
			return 25553;
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
			Process_PtcM2C_CheckQueuingNtf.Process(this);
		}

	}
}
