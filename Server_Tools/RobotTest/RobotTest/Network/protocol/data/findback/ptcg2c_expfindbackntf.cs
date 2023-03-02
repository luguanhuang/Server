using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/5/10 10:51:35

namespace XMainClient
{
    using DataType = KKSG.ExpFindBackData;

	class PtcG2C_ExpFindBackNtf : Protocol
	{
		public DataType Data = new DataType();

		public PtcG2C_ExpFindBackNtf()
		{

		}

		public override uint GetProtoType()
		{
			return 4933;
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
			Process_PtcG2C_ExpFindBackNtf.Process(this);
		}

	}
}
