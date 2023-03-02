using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2017/6/6 18:05:19

namespace XMainClient
{
    using DataType = KKSG.LeagueBattleStopMatchNtf;

	class PtcM2C_LeagueBattleStopMatchNtf : Protocol
	{
		public DataType Data = new DataType();

		public PtcM2C_LeagueBattleStopMatchNtf()
		{

		}

		public override uint GetProtoType()
		{
			return 53912;
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
			Process_PtcM2C_LeagueBattleStopMatchNtf.Process(this);
		}

	}
}
