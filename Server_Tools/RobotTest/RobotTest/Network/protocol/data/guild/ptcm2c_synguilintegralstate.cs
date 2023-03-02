using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/11/7 21:22:00

namespace XMainClient
{
    using DataType = KKSG.SynGuilIntegralState;

	class PtcM2C_SynGuilIntegralState : Protocol
	{
		public DataType Data = new DataType();

		public PtcM2C_SynGuilIntegralState()
		{

		}

		public override uint GetProtoType()
		{
			return 28075;
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
			Process_PtcM2C_SynGuilIntegralState.Process(this);
		}

	}
}
