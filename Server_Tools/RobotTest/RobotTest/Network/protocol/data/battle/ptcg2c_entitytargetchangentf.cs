using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2017/6/13 11:51:13

namespace XMainClient
{
    using DataType = KKSG.EntityTargetData;

	class PtcG2C_EntityTargetChangeNtf : Protocol
	{
		public DataType Data = new DataType();

		public PtcG2C_EntityTargetChangeNtf()
		{

		}

		public override uint GetProtoType()
		{
			return 9303;
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
			Process_PtcG2C_EntityTargetChangeNtf.Process(this);
		}

	}
}
