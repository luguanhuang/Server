using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/10/9 18:08:06

namespace XMainClient
{
    using DataType = KKSG.SkyCityAllInfo;

	class PtcG2C_SkyCityBattleDataNtf : Protocol
	{
		public DataType Data = new DataType();

		public PtcG2C_SkyCityBattleDataNtf()
		{

		}

		public override uint GetProtoType()
		{
			return 51753;
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
			Process_PtcG2C_SkyCityBattleDataNtf.Process(this);
		}

	}
}
