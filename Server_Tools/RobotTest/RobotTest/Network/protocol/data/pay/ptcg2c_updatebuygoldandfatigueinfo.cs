using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2015/7/20 10:16:11

namespace XMainClient
{
    using DataType = KKSG.BuyGoldFatInfo;

	class PtcG2C_UpdateBuyGoldAndFatigueInfo : Protocol
	{
		public DataType Data = new DataType();

		public PtcG2C_UpdateBuyGoldAndFatigueInfo()
		{

		}

		public override uint GetProtoType()
		{
			return 2587;
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
			Process_PtcG2C_UpdateBuyGoldAndFatigueInfo.Process(this);
		}

	}
}
