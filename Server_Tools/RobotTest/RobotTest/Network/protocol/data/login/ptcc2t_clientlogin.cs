using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/2/22 19:49:07

namespace XMainClient
{
    using DataType = KKSG.LoginRequestArg;

	class PtcC2T_ClientLogin : Protocol
	{
		public DataType Data = new DataType();

		public PtcC2T_ClientLogin()
		{

		}

		public override uint GetProtoType()
		{
			return 35711;
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
