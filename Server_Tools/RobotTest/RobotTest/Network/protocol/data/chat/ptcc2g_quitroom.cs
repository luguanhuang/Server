using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/8/9 15:30:47

namespace XMainClient
{
    using DataType = KKSG.QuitRoom;

	class PtcC2G_QuitRoom : Protocol
	{
		public DataType Data = new DataType();

		public PtcC2G_QuitRoom()
		{

		}

		public override uint GetProtoType()
		{
			return 44925;
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
