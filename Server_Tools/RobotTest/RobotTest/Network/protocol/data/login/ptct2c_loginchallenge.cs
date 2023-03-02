using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2017/2/6 23:18:30

namespace XMainClient
{
    using DataType = KKSG.LoginChallenge;

	class PtcT2C_LoginChallenge : Protocol
	{
		public DataType Data = new DataType();

		public PtcT2C_LoginChallenge()
		{

		}

		public override uint GetProtoType()
		{
			return 58495;
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
			Process_PtcT2C_LoginChallenge.Process(this);
		}

	}
}
