using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2017/5/4 18:01:28

namespace XMainClient
{
    using DataType = KKSG.GiftIBBackInfo;

	class PtcM2C_GiftIBSucessNtf : Protocol
	{
		public DataType Data = new DataType();

		public PtcM2C_GiftIBSucessNtf()
		{

		}

		public override uint GetProtoType()
		{
			return 29707;
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
			Process_PtcM2C_GiftIBSucessNtf.Process(this);
		}

	}
}
