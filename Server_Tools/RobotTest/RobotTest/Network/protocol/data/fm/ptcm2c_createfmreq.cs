using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/11/23 11:37:50

namespace XMainClient
{
    using DataType = KKSG.CreateFM;

	class PtcM2C_CreateFmReq : Protocol
	{
		public DataType Data = new DataType();

		public PtcM2C_CreateFmReq()
		{

		}

		public override uint GetProtoType()
		{
			return 60545;
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
			Process_PtcM2C_CreateFmReq.Process(this);
		}

	}
}
