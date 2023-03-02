using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/8/30 16:46:42

namespace XMainClient
{
    using DataType = KKSG.ItemFindBackData;

	class PtcG2C_ItemFindBackNtf : Protocol
	{
		public DataType Data = new DataType();

		public PtcG2C_ItemFindBackNtf()
		{

		}

		public override uint GetProtoType()
		{
			return 28509;
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
			Process_PtcG2C_ItemFindBackNtf.Process(this);
		}

	}
}
