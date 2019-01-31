using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using System.IO;
using System.Numerics;

namespace ConsoleApp
{
    class MyMult_tb
    {
        /// <summary>
        /// Generates input and output files, in binary format, for testing "MyMult32P".
        /// </summary>
        public static void MyMult32_io_files(string in_file_name, string out_file_name,
            int test_length)
        {
            //4 bytes of dataa, then 4 bytes of datab            
            int[] dataa_b = new int[test_length * 2];

            long[] dataout = new long[test_length];
            Random rand = new Random();
            for (int i = 0; i < test_length; i++)
            {
                dataa_b[2 * i] = rand.Next(Int32.MinValue, Int32.MaxValue);
                dataa_b[2 * i + 1] = rand.Next(Int32.MinValue, Int32.MaxValue);
                dataout[i] = (long)dataa_b[2 * i] * (long)dataa_b[2 * i + 1];
            }
            write_b_array(dataa_b, in_file_name);
            write_b_array(dataout, out_file_name);
        }

        /// <summary>
        /// Generates input and output files, in binary format, for testing "MyMult48P".
        /// </summary>       
        public static void MyMult48_io_files(string in_file_name, string out_file_name,
            int test_length)
        {
            //dataa_b[] array is 6 bytes of dataa, then 6 bytes of datab
            byte[] dataa_b = new byte[test_length * 12];
            byte[] dataaout = new byte[test_length * 12];

            Random rand = new Random();
            rand.NextBytes(dataa_b);
            
            //compute dataout using the "BigInteger" class
            for (int i = 0; i < test_length; i++)
            {
                byte[] b_array1 = new byte[6];
                byte[] b_array2 = new byte[6];
                for (int j = 0; j < b_array1.Length; j++)
                {
                    b_array1[j] = dataa_b[i * 12 + j];
                    b_array2[j] = dataa_b[i * 12 + 6 + j];
                }
                BigInteger a = new BigInteger(b_array1);
                BigInteger b = new BigInteger(b_array2);
                BigInteger ab = a * b;

                byte[] result = ab.ToByteArray();
                if (result.Length > 12) throw new Exception("Result is longer than 12 bytes!");

                // Load result into "dataout" using the big Endian format
                // padd the array as needed to make it 12 bytes long
                for (int j = 0; j < result.Length; j++)
                {
                    // if little Endian format: dataaout[i * 12 + j] = result[j];
                    dataaout[(i+1) * 12 - 1 - j] = result[j];
                }

                if (result.Length < 12)
                {
                    int pad_length = 12 - result.Length;
                    // padd "dataout" till it's 12 bytes long
                    if (ab.Sign == -1)
                    {
                        // padd 0xFF
                        for (int j = 0; j < pad_length; j++)
                            dataaout[i * 12 + j] = 0xFF;
                    }
                    else
                    {
                        // padd 0 - not really needed since default value is 0
                        for (int j = 0; j < pad_length; j++)
                            dataaout[i * 12 + j] = 0;
                    }
                }
            }

            //up to this point, dataa_b is in little Endian format
            //reverse things to be in big Endian format
            for (int i = 0; i < test_length; i++)
            {
                byte temp;
                for (int j = 0; j < 3; j++)
                {
                    // each 48 bit number is 6 bytes; swap bytes 0 and 5, 1 and 4, 2 and 3
                    temp = dataa_b[i * 12 + j];
                    dataa_b[i * 12 + j] = dataa_b[i * 12 + 5 - j];
                    dataa_b[i * 12 + 5 - j] = temp;

                    temp = dataa_b[i * 12 + 6 + j];
                    dataa_b[i * 12 + 6 + j] = dataa_b[i * 12 + 6 + (5 - j)];
                    dataa_b[i * 12 + 6 + (5 - j)] = temp;
                }
            }

            File.WriteAllBytes(in_file_name, dataa_b);
            File.WriteAllBytes(out_file_name, dataaout);
        }

        /// <summary>
        /// Generates input and output files, in binary format, for testing "MyMult40P".
        /// </summary>       
        public static void MyMult40_io_files(string in_file_name, string out_file_name,
            int test_length)
        {
            //dataa_b[] array is 4 bytes of dataa, then 6 bytes of datab
            byte[] dataa_b = new byte[test_length * 10];
            byte[] dataaout = new byte[test_length * 10];

            Random rand = new Random();
            rand.NextBytes(dataa_b);

            //compute dataout using the "BigInteger" class
            for (int i = 0; i < test_length; i++)
            {
                byte[] b_array1 = new byte[4];
                byte[] b_array2 = new byte[6];
                for (int j = 0; j < b_array1.Length; j++)
                    b_array1[j] = dataa_b[i * 10 + j];
                for (int j = 0; j < b_array2.Length; j++)
                    b_array2[j] = dataa_b[i * 10 + 4 + j];

                BigInteger a = new BigInteger(b_array1);
                BigInteger b = new BigInteger(b_array2);
                BigInteger ab = a * b;

                byte[] result = ab.ToByteArray();
                if (result.Length > 10) throw new Exception("Result is longer than 10 bytes!");

                // Load result into "dataout" using the big Endian format
                // padd the array as needed to make it 10 bytes long
                for (int j = 0; j < result.Length; j++)
                {
                    // if little Endian format: dataaout[i * 10 + j] = result[j];
                    dataaout[(i + 1) * 10 - 1 - j] = result[j];
                }

                if (result.Length < 10)
                {
                    int pad_length = 10 - result.Length;
                    // padd "dataout" till it's 10 bytes long
                    if (ab.Sign == -1)
                    {
                        // padd 0xFF
                        for (int j = 0; j < pad_length; j++)
                            dataaout[i * 10 + j] = 0xFF;
                    }
                    else
                    {
                        // padd 0 - not really needed since default value is 0
                        for (int j = 0; j < pad_length; j++)
                            dataaout[i * 10 + j] = 0;
                    }
                }
            }

            //up to this point, dataa_b is in little Endian format
            //reverse things to be in big Endian format
            for (int i = 0; i < test_length; i++)
            {
                byte temp;
                // bytes 0 through 3 is dataa
                temp = dataa_b[i * 10];
                dataa_b[i * 10] = dataa_b[i * 10 + 3]; dataa_b[i * 10 + 3] = temp;
                temp = dataa_b[i * 10 + 1];
                dataa_b[i * 10 + 1] = dataa_b[i * 10 + 2]; dataa_b[i * 10 + 2] = temp;

                // bytes 4 through 9 is datab
                for (int j = 0; j < 3; j++)
                {
                    // swap bytes 0 and 5, 1 and 4, 2 and 3
                    temp = dataa_b[i * 10 + 4 + j];
                    dataa_b[i * 10 + 4 + j] = dataa_b[i * 10 + 4 + (5 - j)];
                    dataa_b[i * 10 + 4 + (5 - j)] = temp;
                }
            }

            File.WriteAllBytes(in_file_name, dataa_b);
            File.WriteAllBytes(out_file_name, dataaout);
        }

        #region Helper Functions

        /// <summary>
        /// Writes an array of integers to file, following the Big Endian
        /// convention.
        /// </summary>
        private static void write_b_array(int[] i_array, string file_name)
        {            
            byte[] b_array = new byte[i_array.Length * 4];
            for (int i = 0; i < i_array.Length; i++)
            {
                byte[] temp = BitConverter.GetBytes(i_array[i]);
                for (int j = 0; j < temp.Length; j++)
                    b_array[i * 4 + j] = temp[3 - j];
            }
            File.WriteAllBytes(file_name, b_array);
        }

        /// <summary>
        /// Writes an array of LONG integers to file, following the Big Endian
        /// convention.
        /// </summary>
        private static void write_b_array(long[] L_array, string file_name)
        {
            byte[] b_array = new byte[L_array.Length * 8];
            for (int i = 0; i < L_array.Length; i++)
            {
                byte[] temp = BitConverter.GetBytes(L_array[i]);
                for (int j = 0; j < temp.Length; j++)
                    b_array[i * 8 + j] = temp[7 - j];
            }
            File.WriteAllBytes(file_name, b_array);
        }

        #endregion
    }
}
