using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/9/2 22:11:35

namespace XMainClient
{
    using DataType = KKSG.AllSynCardAttr;

	class PtcG2C_SynAtlasAttr : Protocol
	{
		public DataType Data = new DataType();

		public PtcG2C_SynAtlasAttr()
		{

		}

		public override uint GetProtoType()
		{
			return 1285;
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
			Process_PtcG2C_SynAtlasAttr.Process(this);
		}

	}
}
