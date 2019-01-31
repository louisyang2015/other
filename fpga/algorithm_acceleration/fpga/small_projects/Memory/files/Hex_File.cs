using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using System.IO;

namespace ConsoleApp
{
    /// <summary>
    /// This class holds some routines for generating an Intel Hex format file
    /// from an array of "int" or "long" values.
    /// </summary>
    class Hex_File
    {
        /// <summary>
        /// Writes an array of bytes to a file.
        /// </summary>
        public static void write_bytes_to_file(byte[] b_array, string file_name, int bytes_per_line,
            int bytes_per_word, int start_address)
        {
            if (bytes_per_line > 255) throw new Exception("bytes per line > 255");
            if (bytes_per_line < 1) throw new Exception("bytes per line < 1");
            if (bytes_per_line % bytes_per_word != 0)
                throw new Exception("bytes per word does not divide into bytes per line");

            StringBuilder sb = new StringBuilder();
            int address = start_address;
            int words_per_line = bytes_per_line / bytes_per_word;

            // render the lines one at a time
            int current_line_index = 0; // index of the b_array element at the start of the current line

            while (current_line_index < b_array.Length)
            {
                // start of a line
                sb.Append(":");

                // byte count field - first figure out the byte count
                int byte_count = bytes_per_line;
                if (current_line_index + bytes_per_line > b_array.Length - 1)
                    byte_count = b_array.Length - current_line_index;
                byte byte_count_b = (byte)byte_count;
                sb.Append(byte_count_b.ToString("X2"));
                uint sum = (uint)byte_count; // for checksum of this line; "20hex" = 32 decimal

                // 2 byte address field
                byte[] addr_array = BitConverter.GetBytes(address);
                sb.Append(addr_array[1].ToString("X2"));
                sb.Append(addr_array[0].ToString("X2"));
                sum += (uint)(addr_array[1]);
                sum += (uint)(addr_array[0]);

                // record type field: 00 = data record
                sb.Append("00");

                // data values:
                for (int i = 0; i < byte_count; i++)
                {
                    sb.Append(b_array[current_line_index + i].ToString("X2"));
                    sum += (uint)(b_array[current_line_index + i]);
                }

                //checksum:
                int sum2 = -1 * (int)sum;
                byte[] checksum = BitConverter.GetBytes(sum2);
                sb.Append(checksum[0].ToString("X2"));

                sb.Append("\r\n");

                // update variables for the next line
                current_line_index += bytes_per_line;
                address += words_per_line;
            }
            sb.Append(":00000001FF \r\n"); // final line in the hex file
            File.WriteAllText(file_name, sb.ToString());
        }

        /// <summary>
        /// Writes an array of bytes to a file. The starting address is assumed to be 0.
        /// </summary>
        public static void write_bytes_to_file(byte[] b_array, string file_name, int bytes_per_line,
            int bytes_per_word)
        {
            write_bytes_to_file(b_array, file_name, bytes_per_line, bytes_per_word, 0);
        }

        /// <summary>
        /// Writes an array of 16 bit integers to file. Only the lower 16 bits of 
        /// the integers are used.
        /// </summary>
        public static void write_int16_to_file(int[] i_array, string file_name)
        {
            byte[] b_array = new byte[i_array.Length * 2];
            for (int i = 0; i < i_array.Length; i++)
            {
                byte[] b_values = BitConverter.GetBytes(i_array[i]);
                b_array[i * 2] = b_values[1];
                b_array[i * 2 + 1] = b_values[0];
            }
            write_bytes_to_file(b_array, file_name, 32, 2);
        }


        /// <summary>
        /// Writes an array of 32 bit integers to file.
        /// </summary>
        public static void write_int32_to_file(int[] i_array, string file_name)
        {
            byte[] b_array = new byte[i_array.Length * 4];
            for (int i = 0; i < i_array.Length; i++)
            {
                byte[] b_values = BitConverter.GetBytes(i_array[i]);
                b_array[i * 4] = b_values[3]; // MSB of i_array[i]
                b_array[i * 4 + 1] = b_values[2];
                b_array[i * 4 + 2] = b_values[1];
                b_array[i * 4 + 3] = b_values[0];
            }
            write_bytes_to_file(b_array, file_name, 32, 4);
        }

        /// <summary>
        /// Writes an array of 48 bit long integers to file. Only the lower 48 bits
        /// of each long integer is encoded to the file.
        /// </summary>
        public static void write_int48_to_file(long[] i_array, string file_name)
        {
            byte[] b_array = new byte[i_array.Length * 6];
            for (int i = 0; i < i_array.Length; i++)
            {
                byte[] b_values = BitConverter.GetBytes(i_array[i]);
                b_array[i * 6] = b_values[5]; // MSB of i_array[i]
                b_array[i * 6 + 1] = b_values[4];
                b_array[i * 6 + 2] = b_values[3];
                b_array[i * 6 + 3] = b_values[2];
                b_array[i * 6 + 4] = b_values[1];
                b_array[i * 6 + 5] = b_values[0];
            }
            write_bytes_to_file(b_array, file_name, 48, 6);
        }

        /// <summary>
        /// Writes an array of 64 bit long integers to file.
        /// </summary>
        public static void write_int64_to_file(long[] i_array, string file_name)
        {
            byte[] b_array = new byte[i_array.Length * 8];
            for (int i = 0; i < i_array.Length; i++)
            {
                byte[] b_values = BitConverter.GetBytes(i_array[i]);
                b_array[i * 8] = b_values[7]; // MSB of i_array[i]
                b_array[i * 8 + 1] = b_values[6];
                b_array[i * 8 + 2] = b_values[5];
                b_array[i * 8 + 3] = b_values[4];
                b_array[i * 8 + 4] = b_values[3];
                b_array[i * 8 + 5] = b_values[2];
                b_array[i * 8 + 6] = b_values[1];
                b_array[i * 8 + 7] = b_values[0];
            }
            write_bytes_to_file(b_array, file_name, 32, 8);
        }
    }
}
