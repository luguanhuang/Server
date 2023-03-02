using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2017/5/2 15:42:16

namespace XMainClient
{
    using DataType = KKSG.ReplyPartyExchangeItemArg;

	class PtcC2G_ReplyPartyExchangeItem : Protocol
	{
		public DataType Data = new DataType();

		public PtcC2G_ReplyPartyExchangeItem()
		{

		}

		public override uint GetProtoType()
		{
			return 62563;
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
