using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/1/5 13:27:56

namespace XMainClient
{
    using DataType = KKSG.SceneLoadedNtf;

	class PtcC2G_SceneLoadedNtf : Protocol
	{
		public DataType Data = new DataType();

		public PtcC2G_SceneLoadedNtf()
		{

		}

		public override uint GetProtoType()
		{
			return 15964;
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
			throw new Exception("Send only protocol can not call process");
		}

	}
}
