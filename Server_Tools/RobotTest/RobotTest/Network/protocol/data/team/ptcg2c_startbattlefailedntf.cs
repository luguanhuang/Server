using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/7/8 21:51:09

namespace XMainClient
{
    using DataType = KKSG.StartBattleFailedRes;

	class PtcG2C_StartBattleFailedNtf : Protocol
	{
		public DataType Data = new DataType();

		public PtcG2C_StartBattleFailedNtf()
		{

		}

		public override uint GetProtoType()
		{
			return 54098;
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
			Process_PtcG2C_StartBattleFailedNtf.Process(this);
		}

	}
}
