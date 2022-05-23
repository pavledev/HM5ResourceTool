using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace HM5ResourceTool
{
    internal class BinaryReader2 : BinaryReader
    {
        private bool littleEndian;

        public BinaryReader2(System.IO.Stream stream, bool littleEndian = true) : base(stream)
        {
            this.littleEndian = littleEndian;
        }

        public override int ReadInt32()
        {
            var data = base.ReadBytes(4);

            if (!littleEndian)
            {
                Array.Reverse(data);
            }

            return BitConverter.ToInt32(data, 0);
        }

        public override short ReadInt16()
        {
            var data = base.ReadBytes(2);

            if (!littleEndian)
            {
                Array.Reverse(data);
            }

            return BitConverter.ToInt16(data, 0);
        }

        public override long ReadInt64()
        {
            var data = base.ReadBytes(8);

            if (!littleEndian)
            {
                Array.Reverse(data);
            }

            return BitConverter.ToInt64(data, 0);
        }

        public override uint ReadUInt32()
        {
            var data = base.ReadBytes(4);

            if (!littleEndian)
            {
                Array.Reverse(data);
            }

            return BitConverter.ToUInt32(data, 0);
        }
    }
}
