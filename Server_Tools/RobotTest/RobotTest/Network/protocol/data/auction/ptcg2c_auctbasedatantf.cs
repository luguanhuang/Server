using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/4/14 11:36:34

namespace XMainClient
{
    using DataType = KKSG.AuctBaseData;

	class PtcG2C_AuctBaseDataNtf : Protocol
	{
		public DataType Data = new DataType();

		public PtcG2C_AuctBaseDataNtf()
		{

		}

		public override uint GetProtoType()
		{
			return 24793;
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
			Process_PtcG2C_AuctBaseDataNtf.Process(this);
		}

	}
}
