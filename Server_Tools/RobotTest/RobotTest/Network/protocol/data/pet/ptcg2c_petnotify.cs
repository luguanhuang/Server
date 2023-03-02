using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/4/15 11:13:43

namespace XMainClient
{
    using DataType = KKSG.PetNotify;

	class PtcG2C_PetNotify : Protocol
	{
		public DataType Data = new DataType();

		public PtcG2C_PetNotify()
		{

		}

		public override uint GetProtoType()
		{
			return 17722;
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
			Process_PtcG2C_PetNotify.Process(this);
		}

	}
}
