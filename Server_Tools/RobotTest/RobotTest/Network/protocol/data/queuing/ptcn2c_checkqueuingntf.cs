using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/11/16 16:37:23

namespace XMainClient
{
    using DataType = KKSG.CheckQueuingNtf;

	class PtcN2C_CheckQueuingNtf : Protocol
	{
		public DataType Data = new DataType();

		public PtcN2C_CheckQueuingNtf()
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
			Process_PtcN2C_CheckQueuingNtf.Process(this);
		}

	}
}
