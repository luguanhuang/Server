using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/6/23 11:41:05

namespace XMainClient
{
    using DataType = KKSG.PvpBattleBeginData;

	class PtcG2C_PvpBattleBeginNtf : Protocol
	{
		public DataType Data = new DataType();

		public PtcG2C_PvpBattleBeginNtf()
		{

		}

		public override uint GetProtoType()
		{
			return 53763;
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
			Process_PtcG2C_PvpBattleBeginNtf.Process(this);
		}

	}
}
