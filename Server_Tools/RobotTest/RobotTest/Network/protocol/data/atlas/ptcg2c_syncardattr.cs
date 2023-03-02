using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/9/2 21:48:59

namespace XMainClient
{
    using DataType = KKSG.AllSynCardAttr;

	class PtcG2C_SynCardAttr : Protocol
	{
		public DataType Data = new DataType();

		public PtcG2C_SynCardAttr()
		{

		}

		public override uint GetProtoType()
		{
			return 5344;
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
			Process_PtcG2C_SynCardAttr.Process(this);
		}

	}
}
