using System;
using System.Collections.Generic;
using System.Linq;
using System.Security.Cryptography;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Shapes;
using MahApps.Metro.Controls;

namespace HM5ResourceTool
{
    /// <summary>
    /// Interaction logic for FilePathGeneratorWindow.xaml
    /// </summary>
    public partial class FilePathGeneratorWindow : MetroWindow
    {
        public FilePathGeneratorWindow()
        {
            InitializeComponent();
        }

        private void TxtAssemblyPaths_TextChanged(object sender, TextChangedEventArgs e)
        {
            List<string> lines = txtAssemblyPaths.Text.Split('\n').ToList();

			txtFilePaths.Clear();

			foreach (string line in lines)
            {
				txtFilePaths.Text += string.Format("{0}\n", ConvertResourceIDToFilePath(line));
            }
        }

		private string ConvertResourceIDToFilePath(string resourceID)
		{
			string resourceID2 = resourceID.ToLower();
			string extension = resourceID2.Substring(resourceID2.LastIndexOf('.') + 1);
			int index = resourceID2.IndexOf(':');
			int index2 = resourceID2.IndexOf('?');
			int index3 = resourceID2.IndexOf(']');
			string folderPath = "..";

			if (index2 != -1 && index < index2)
			{
				folderPath += resourceID2.Substring(index + 1, index2 - index - 1).Replace("/", @"\");
			}
			else
			{
				folderPath += resourceID2.Substring(index + 1, index3 - index - 1).Replace("/", @"\");
			}

			resourceID2 = resourceID2.Substring(0, resourceID2.LastIndexOf('.') + 1);

			string fileName = CreateMD5(resourceID2);

			return string.Format(@"{0}\{1}.{2}", folderPath, fileName, extension);
		}

		private string CreateMD5(string input)
		{
			using (MD5 md5 = MD5.Create())
			{
				byte[] inputBytes = Encoding.ASCII.GetBytes(input);
				byte[] hashBytes = md5.ComputeHash(inputBytes);
				StringBuilder sb = new StringBuilder();

				for (int i = 0; i < hashBytes.Length; i++)
				{
					sb.Append(hashBytes[i].ToString("X2"));
				}

				return sb.ToString();
			}
		}
	}
}
