using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/12/15 20:06:36

namespace XMainClient
{
    using DataType = KKSG.InvFightBefESpara;

	class PtcG2C_InvFightBefEnterSceneNtf : Protocol
	{
		public DataType Data = new DataType();

		public PtcG2C_InvFightBefEnterSceneNtf()
		{

		}

		public override uint GetProtoType()
		{
			return 7135;
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
			Process_PtcG2C_InvFightBefEnterSceneNtf.Process(this);
		}

	}
}
