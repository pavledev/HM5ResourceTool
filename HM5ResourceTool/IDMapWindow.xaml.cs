using System;
using System.Collections.Generic;
using System.Linq;
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
using System.IO;
using MahApps.Metro.Controls;
using ControlzEx.Theming;

namespace HM5ResourceTool
{
	/// <summary>
	/// Interaction logic for IDMapWindow.xaml
	/// </summary>
	public partial class IDMapWindow : MetroWindow
	{
		private string idMapPath;

		public IDMapWindow(string idMapPath, Settings? settings)
		{
			InitializeComponent();

			this.idMapPath = idMapPath;

			if (settings != null && settings.Theme != null)
			{
				Utility.ChangeTheme(settings, this, false);
			}
		}

		private void IDMapWindow_ContentRendered(object sender, EventArgs e)
		{
			using BinaryReader2 binaryReader = new BinaryReader2(File.Open(idMapPath, FileMode.Open));

			binaryReader.BaseStream.Seek(24, SeekOrigin.Begin);

			int pos = 0;
			int length = (int)binaryReader.BaseStream.Length;

			while (pos < length)
			{
				char c = binaryReader.ReadChar();
				string line = "";

				if (c == '|')
				{
					line += c;

					while (true)
					{
						c = binaryReader.ReadChar();

						if (c == '|')
						{
							line += c;

							break;
						}

						line += c;
						pos += sizeof(char);
					}

					while (!line.EndsWith("\r"))
					{
						line += binaryReader.ReadChar();
						pos += sizeof(char);
					}

					txtIDMap.Text += string.Format("{0}\n", line);
				}

				pos += line.Length;
			}
		}
	}
}
