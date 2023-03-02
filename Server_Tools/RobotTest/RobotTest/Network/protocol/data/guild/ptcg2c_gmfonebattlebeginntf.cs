using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/7/21 1:13:02

namespace XMainClient
{
    using DataType = KKSG.GmfOneBattleBegin;

	class PtcG2C_GmfOneBattleBeginNtf : Protocol
	{
		public DataType Data = new DataType();

		public PtcG2C_GmfOneBattleBeginNtf()
		{

		}

		public override uint GetProtoType()
		{
			return 40696;
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
			Process_PtcG2C_GmfOneBattleBeginNtf.Process(this);
		}

	}
}
