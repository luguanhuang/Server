using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2017/2/7 21:48:58

namespace XMainClient
{
    using DataType = KKSG.GCFG2CSynPara;

	class PtcG2C_GCFSynG2CNtf : Protocol
	{
		public DataType Data = new DataType();

		public PtcG2C_GCFSynG2CNtf()
		{

		}

		public override uint GetProtoType()
		{
			return 31469;
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
			Process_PtcG2C_GCFSynG2CNtf.Process(this);
		}

	}
}
