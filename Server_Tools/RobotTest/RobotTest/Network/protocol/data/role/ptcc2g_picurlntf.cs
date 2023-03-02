using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/12/10 14:15:13

namespace XMainClient
{
    using DataType = KKSG.PicUrlInfo;

	class PtcC2G_PicUrlNtf : Protocol
	{
		public DataType Data = new DataType();

		public PtcC2G_PicUrlNtf()
		{

		}

		public override uint GetProtoType()
		{
			return 30863;
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
