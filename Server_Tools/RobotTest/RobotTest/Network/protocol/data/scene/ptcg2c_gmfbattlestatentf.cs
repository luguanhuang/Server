using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/10/10 23:24:55

namespace XMainClient
{
    using DataType = KKSG.GmfBatlleStatePara;

	class PtcG2C_GmfBattleStateNtf : Protocol
	{
		public DataType Data = new DataType();

		public PtcG2C_GmfBattleStateNtf()
		{

		}

		public override uint GetProtoType()
		{
			return 21747;
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
			Process_PtcG2C_GmfBattleStateNtf.Process(this);
		}

	}
}
