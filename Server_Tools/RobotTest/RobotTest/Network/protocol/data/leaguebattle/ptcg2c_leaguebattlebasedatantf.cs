using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2017/1/9 7:51:58

namespace XMainClient
{
    using DataType = KKSG.LeagueBattleBaseDataNtf;

	class PtcG2C_LeagueBattleBaseDataNtf : Protocol
	{
		public DataType Data = new DataType();

		public PtcG2C_LeagueBattleBaseDataNtf()
		{

		}

		public override uint GetProtoType()
		{
			return 19581;
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
			Process_PtcG2C_LeagueBattleBaseDataNtf.Process(this);
		}

	}
}
