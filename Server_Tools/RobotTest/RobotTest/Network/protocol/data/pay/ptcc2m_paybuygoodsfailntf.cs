using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2017/3/16 22:45:50

namespace XMainClient
{
    using DataType = KKSG.PayBuyGoodsFail;

	class PtcC2M_PayBuyGoodsFailNtf : Protocol
	{
		public DataType Data = new DataType();

		public PtcC2M_PayBuyGoodsFailNtf()
		{

		}

		public override uint GetProtoType()
		{
			return 23670;
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
			throw new Exception("Send only protocol can not call process");
		}

	}
}
