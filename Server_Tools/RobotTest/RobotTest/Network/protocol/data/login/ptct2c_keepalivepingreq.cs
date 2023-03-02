using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/12/16 16:00:45

namespace XMainClient
{
    //using DataType = KKSG.;

	class PtcT2C_KeepAlivePingReq : Protocol
	{
		//public DataType Data = new DataType();

		public PtcT2C_KeepAlivePingReq()
		{

		}

		public override uint GetProtoType()
		{
			return 49142;
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
			Process_PtcT2C_KeepAlivePingReq.Process(this);
		}

	}
}
