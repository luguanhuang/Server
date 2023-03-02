using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/10/14 19:13:29

namespace XMainClient
{
    using DataType = KKSG.SpriteState;

	class PtcM2C_SpriteStateChangeNtf : Protocol
	{
		public DataType Data = new DataType();

		public PtcM2C_SpriteStateChangeNtf()
		{

		}

		public override uint GetProtoType()
		{
			return 38584;
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
			Process_PtcM2C_SpriteStateChangeNtf.Process(this);
		}

	}
}
