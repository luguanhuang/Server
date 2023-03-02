using System;
using System.Collections.Generic;

using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2015/2/2 15:10:52

namespace XMainClient
{
    using DataType = KKSG.Systems;

	class PtcG2C_OpenSystemNtf : Protocol
	{
		public DataType Data = new DataType();

		public PtcG2C_OpenSystemNtf()
		{

		}

		public override uint GetProtoType()
		{
			return 41168;
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
			Process_PtcG2C_OpenSystemNtf.Process(this);
		}

	}
}
