using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2017/3/15 10:56:36

namespace XMainClient
{
    using DataType = KKSG.MakePartnerResult;

	class PtcM2C_MakePartnerResultNtf : Protocol
	{
		public DataType Data = new DataType();

		public PtcM2C_MakePartnerResultNtf()
		{

		}

		public override uint GetProtoType()
		{
			return 49652;
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
			Process_PtcM2C_MakePartnerResultNtf.Process(this);
		}

	}
}
