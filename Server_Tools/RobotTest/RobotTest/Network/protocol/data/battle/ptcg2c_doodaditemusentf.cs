using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/12/24 10:23:12

namespace XMainClient
{
    using DataType = KKSG.DoodadItemUseNtf;

	class PtcG2C_DoodadItemUseNtf : Protocol
	{
		public DataType Data = new DataType();

		public PtcG2C_DoodadItemUseNtf()
		{

		}

		public override uint GetProtoType()
		{
			return 13498;
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
			Process_PtcG2C_DoodadItemUseNtf.Process(this);
		}

	}
}
