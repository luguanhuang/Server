using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/12/27 23:45:50

namespace XMainClient
{
    using DataType = KKSG.HeroBattleStopMatchData;

	class PtcM2C_HeroBattleStopMatch : Protocol
	{
		public DataType Data = new DataType();

		public PtcM2C_HeroBattleStopMatch()
		{

		}

		public override uint GetProtoType()
		{
			return 40094;
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
			Process_PtcM2C_HeroBattleStopMatch.Process(this);
		}

	}
}
