using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/12/25 18:50:47

namespace XMainClient
{
    using DataType = KKSG.HeroBattleSyncData;

	class PtcG2C_HeroBattleSyncNtf : Protocol
	{
		public DataType Data = new DataType();

		public PtcG2C_HeroBattleSyncNtf()
		{

		}

		public override uint GetProtoType()
		{
			return 33024;
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
			Process_PtcG2C_HeroBattleSyncNtf.Process(this);
		}

	}
}
