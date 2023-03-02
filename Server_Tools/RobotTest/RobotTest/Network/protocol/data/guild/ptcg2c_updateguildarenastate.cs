using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/7/18 11:58:28

namespace XMainClient
{
    using DataType = KKSG.UpdateGuildArenaState;

	class PtcG2C_UpdateGuildArenaState : Protocol
	{
		public DataType Data = new DataType();

		public PtcG2C_UpdateGuildArenaState()
		{

		}

		public override uint GetProtoType()
		{
			return 21909;
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
			Process_PtcG2C_UpdateGuildArenaState.Process(this);
		}

	}
}
