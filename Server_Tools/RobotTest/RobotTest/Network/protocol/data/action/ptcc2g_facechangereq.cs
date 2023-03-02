using System;
using System.Collections.Generic;

using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2014/12/17 10:41:12

namespace XMainClient
{
    using DataType = KKSG.LocationInfo;

	class PtcC2G_FaceChangeReq : Protocol
	{
		public DataType Data = new DataType();

		public PtcC2G_FaceChangeReq()
		{

		}

		public override uint GetProtoType()
		{
			return 7975;
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
