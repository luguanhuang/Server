using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2017/7/15 15:15:19

namespace XMainClient
{
    using DataType = KKSG.HeroBattleAncientPowerData;

	class PtcG2C_HeroBattleAncientPower : Protocol
	{
		public DataType Data = new DataType();

		public PtcG2C_HeroBattleAncientPower()
		{

		}

		public override uint GetProtoType()
		{
			return 37102;
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
			Process_PtcG2C_HeroBattleAncientPower.Process(this);
		}

	}
}
