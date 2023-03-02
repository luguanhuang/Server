using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/9/7 21:20:42

namespace XMainClient
{
    using DataType = KKSG.RemoveIBShopIcon;

	class PtcC2G_RemoveIBShopIcon : Protocol
	{
		public DataType Data = new DataType();

		public PtcC2G_RemoveIBShopIcon()
		{

		}

		public override uint GetProtoType()
		{
			return 33988;
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
