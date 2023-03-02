using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/9/9 20:46:29

namespace XMainClient
{
    using DataType = KKSG.RiskRecoverDiceData;

	class PtcG2C_RiskRecoverDiceNtf : Protocol
	{
		public DataType Data = new DataType();

		public PtcG2C_RiskRecoverDiceNtf()
		{

		}

		public override uint GetProtoType()
		{
			return 45917;
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
			Process_PtcG2C_RiskRecoverDiceNtf.Process(this);
		}

	}
}
