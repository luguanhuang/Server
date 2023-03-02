using System;
using System.Collections.Generic;

using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2015/1/23 11:18:07

namespace XMainClient
{
    using DataType = KKSG.LevelChanged;

	class PtcG2C_LevelChangeNtf : Protocol
	{
		public DataType Data = new DataType();

		public PtcG2C_LevelChangeNtf()
		{

		}

		public override uint GetProtoType()
		{
			return 38651;
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
			Process_PtcG2C_LevelChangeNtf.Process(this);
		}

	}
}
