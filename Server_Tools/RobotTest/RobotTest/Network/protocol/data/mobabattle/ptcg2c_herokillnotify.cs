using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2017/6/9 17:10:16

namespace XMainClient
{
    using DataType = KKSG.HeroKillNotifyData;

	class PtcG2C_HeroKillNotify : Protocol
	{
		public DataType Data = new DataType();

		public PtcG2C_HeroKillNotify()
		{

		}

		public override uint GetProtoType()
		{
			return 58962;
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
			Process_PtcG2C_HeroKillNotify.Process(this);
		}

	}
}
