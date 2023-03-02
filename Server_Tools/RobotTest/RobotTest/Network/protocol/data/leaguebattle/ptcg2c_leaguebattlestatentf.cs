using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2017/1/10 18:13:41

namespace XMainClient
{
    using DataType = KKSG.LeagueBattleStateNtf;

	class PtcG2C_LeagueBattleStateNtf : Protocol
	{
		public DataType Data = new DataType();

		public PtcG2C_LeagueBattleStateNtf()
		{

		}

		public override uint GetProtoType()
		{
			return 59496;
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
			Process_PtcG2C_LeagueBattleStateNtf.Process(this);
		}

	}
}
