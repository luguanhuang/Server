using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/9/14 14:37:07

namespace XMainClient
{
    using DataType = KKSG.OpenSpriteEgg;

	class PtcC2G_OpenSpriteEggNtf : Protocol
	{
		public DataType Data = new DataType();

		public PtcC2G_OpenSpriteEggNtf()
		{

		}

		public override uint GetProtoType()
		{
			return 47965;
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
