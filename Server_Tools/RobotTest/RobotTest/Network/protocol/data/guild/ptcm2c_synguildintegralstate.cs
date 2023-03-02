using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/10/25 19:33:46

namespace XMainClient
{
    using DataType = KKSG.SynGuildIntegralState;

	class PtcM2C_SynGuildIntegralState : Protocol
	{
		public DataType Data = new DataType();

		public PtcM2C_SynGuildIntegralState()
		{

		}

		public override uint GetProtoType()
		{
			return 4104;
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
			Process_PtcM2C_SynGuildIntegralState.Process(this);
		}

	}
}
