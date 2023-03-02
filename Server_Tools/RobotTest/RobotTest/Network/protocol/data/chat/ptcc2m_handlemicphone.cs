using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/11/29 16:36:19

namespace XMainClient
{
    using DataType = KKSG.HandleMicphoneArg;

	class PtcC2M_HandleMicphone : Protocol
	{
		public DataType Data = new DataType();

		public PtcC2M_HandleMicphone()
		{

		}

		public override uint GetProtoType()
		{
			return 50175;
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
