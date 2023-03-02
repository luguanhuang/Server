using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2015/7/30 18:16:17

namespace XMainClient
{
    using DataType = KKSG.EnterSceneCoolDownNotify;

	class PtcG2C_EnterSceneCoolDownNotify : Protocol
	{
		public DataType Data = new DataType();

		public PtcG2C_EnterSceneCoolDownNotify()
		{

		}

		public override uint GetProtoType()
		{
			return 38664;
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
			Process_PtcG2C_EnterSceneCoolDownNotify.Process(this);
		}

	}
}
