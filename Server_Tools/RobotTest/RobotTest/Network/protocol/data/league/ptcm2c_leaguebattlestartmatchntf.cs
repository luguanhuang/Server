using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2017/1/6 15:25:40

namespace XMainClient
{
    using DataType = KKSG.LeagueBattleStartMatchNtf;

	class PtcM2C_LeagueBattleStartMatchNtf : Protocol
	{
		public DataType Data = new DataType();

		public PtcM2C_LeagueBattleStartMatchNtf()
		{

		}

		public override uint GetProtoType()
		{
			return 61870;
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
			Process_PtcM2C_LeagueBattleStartMatchNtf.Process(this);
		}

	}
}
