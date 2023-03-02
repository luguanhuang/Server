using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/3/15 15:14:23

namespace XMainClient
{
    using DataType = KKSG.GaoJiGuBaseInfoChangeData;

	class PtcG2C_GaoJiGuBaseInfoChangeNtf : Protocol
	{
		public DataType Data = new DataType();

		public PtcG2C_GaoJiGuBaseInfoChangeNtf()
		{

		}

		public override uint GetProtoType()
		{
			return 35761;
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
			Process_PtcG2C_GaoJiGuBaseInfoChangeNtf.Process(this);
		}

	}
}
