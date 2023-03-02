using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2017/1/11 12:08:00

namespace XMainClient
{
    using DataType = KKSG.HeroBattleCanUseHeroData;

	class PtcG2C_HeroBattleCanUseHero : Protocol
	{
		public DataType Data = new DataType();

		public PtcG2C_HeroBattleCanUseHero()
		{

		}

		public override uint GetProtoType()
		{
			return 20354;
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
			Process_PtcG2C_HeroBattleCanUseHero.Process(this);
		}

	}
}
