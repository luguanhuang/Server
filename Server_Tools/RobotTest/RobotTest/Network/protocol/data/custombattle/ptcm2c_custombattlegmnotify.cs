using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2017/5/15 20:41:12

namespace XMainClient
{
    using DataType = KKSG.CustomBattleGMNotify;

	class PtcM2C_CustomBattleGMNotify : Protocol
	{
		public DataType Data = new DataType();

		public PtcM2C_CustomBattleGMNotify()
		{

		}

		public override uint GetProtoType()
		{
			return 65108;
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
			Process_PtcM2C_CustomBattleGMNotify.Process(this);
		}

	}
}
