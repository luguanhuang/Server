using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/10/12 10:39:44

namespace XMainClient
{
    using DataType = KKSG.SpriteAppear;

	class PtcM2C_SpriteAppearNtf : Protocol
	{
		public DataType Data = new DataType();

		public PtcM2C_SpriteAppearNtf()
		{

		}

		public override uint GetProtoType()
		{
			return 28343;
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
			Process_PtcM2C_SpriteAppearNtf.Process(this);
		}

	}
}
