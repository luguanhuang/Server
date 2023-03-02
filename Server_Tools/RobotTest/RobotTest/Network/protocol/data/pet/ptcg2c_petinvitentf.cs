using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2017/6/16 14:24:53

namespace XMainClient
{
    using DataType = KKSG.PetInviteNtf;

	class PtcG2C_PetInviteNtf : Protocol
	{
		public DataType Data = new DataType();

		public PtcG2C_PetInviteNtf()
		{

		}

		public override uint GetProtoType()
		{
			return 19818;
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
			Process_PtcG2C_PetInviteNtf.Process(this);
		}

	}
}
