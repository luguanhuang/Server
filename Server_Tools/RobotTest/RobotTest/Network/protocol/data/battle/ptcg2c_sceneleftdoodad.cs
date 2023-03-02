using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2015/12/29 19:51:23

namespace XMainClient
{
    using DataType = KKSG.SceneLeftDoodad;

	class PtcG2C_SceneLeftDoodad : Protocol
	{
		public DataType Data = new DataType();

		public PtcG2C_SceneLeftDoodad()
		{

		}

		public override uint GetProtoType()
		{
			return 18028;
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
			Process_PtcG2C_SceneLeftDoodad.Process(this);
		}

	}
}
