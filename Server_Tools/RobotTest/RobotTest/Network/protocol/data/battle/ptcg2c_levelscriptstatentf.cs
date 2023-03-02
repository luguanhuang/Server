using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/12/28 10:09:38

namespace XMainClient
{
    using DataType = KKSG.LevelScriptStateData;

	class PtcG2C_LevelScriptStateNtf : Protocol
	{
		public DataType Data = new DataType();

		public PtcG2C_LevelScriptStateNtf()
		{

		}

		public override uint GetProtoType()
		{
			return 12789;
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
			Process_PtcG2C_LevelScriptStateNtf.Process(this);
		}

	}
}
