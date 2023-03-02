using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2017/4/26 19:27:55

namespace XMainClient
{
    //using DataType = KKSG.;

	class PtcC2G_LoginActivityAwardReq : Protocol
	{
		//public DataType Data = new DataType();

		public PtcC2G_LoginActivityAwardReq()
		{

		}

		public override uint GetProtoType()
		{
			return 16542;
		}

		public override void Serialize(MemoryStream stream)
		{
			//Serializer.Serialize(stream, Data);
		}

		public override void DeSerialize(MemoryStream stream)
		{
			//Data = Serializer.Deserialize<DataType>(stream);
		}

		public override void Process()
		{
			throw new Exception("Send only protocol can not call process");
		}

	}
}
