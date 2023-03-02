using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2017/9/1 15:43:50

namespace XMainClient
{
    using DataType = KKSG.GoBackReadyScene;

	class PtcC2M_GoBackReadySceneNtf : Protocol
	{
		public DataType Data = new DataType();

		public PtcC2M_GoBackReadySceneNtf()
		{

		}

		public override uint GetProtoType()
		{
			return 10491;
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
