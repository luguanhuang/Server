using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2017/1/9 8:27:20

namespace XMainClient
{
    using DataType = KKSG.LeagueBattleResultNtf;

	class PtcG2C_LeagueBattleResultNtf : Protocol
	{
		public DataType Data = new DataType();

		public PtcG2C_LeagueBattleResultNtf()
		{

		}

		public override uint GetProtoType()
		{
			return 29255;
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
			Process_PtcG2C_LeagueBattleResultNtf.Process(this);
		}

	}
}
