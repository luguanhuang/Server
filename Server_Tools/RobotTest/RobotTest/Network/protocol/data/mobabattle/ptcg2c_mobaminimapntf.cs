using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2017/6/20 22:13:22

namespace XMainClient
{
    using DataType = KKSG.MobaMiniMapData;

	class PtcG2C_MobaMiniMapNtf : Protocol
	{
		public DataType Data = new DataType();

		public PtcG2C_MobaMiniMapNtf()
		{

		}

		public override uint GetProtoType()
		{
			return 32069;
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
			Process_PtcG2C_MobaMiniMapNtf.Process(this);
		}

	}
}
