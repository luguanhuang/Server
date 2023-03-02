using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/12/15 17:32:50

namespace XMainClient
{
    //using DataType = KKSG.;

	class PtcC2T_KeepAlivePingAck : Protocol
	{
		//public DataType Data = new DataType();

		public PtcC2T_KeepAlivePingAck()
		{

		}

		public override uint GetProtoType()
		{
			return 29192;
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
