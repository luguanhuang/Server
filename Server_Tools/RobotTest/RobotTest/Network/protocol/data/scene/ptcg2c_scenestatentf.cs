using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/12/28 11:09:05

namespace XMainClient
{
    using DataType = KKSG.SceneStateNtf;

	class PtcG2C_SceneStateNtf : Protocol
	{
		public DataType Data = new DataType();

		public PtcG2C_SceneStateNtf()
		{

		}

		public override uint GetProtoType()
		{
			return 4376;
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
			Process_PtcG2C_SceneStateNtf.Process(this);
		}

	}
}
