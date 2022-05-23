using System;
using System.Collections.Generic;
using System.IO;
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
using System.Windows.Navigation;
using System.Windows.Shapes;
using System.Diagnostics;
using MahApps.Metro.Controls;
using MahApps.Metro.Controls.Dialogs;
using Microsoft.Win32;
using HM5ResourceToolCLI;
using ControlzEx.Theming;
using Newtonsoft.Json;

namespace HM5ResourceTool
{
	/// <summary>
	/// Interaction logic for MainWindow.xaml
	/// </summary>
	public partial class MainWindow : MetroWindow
	{
		private string? runtimeFolderPath;
		private string? currentFolderPath;
		private Dictionary<string, string> headerLibraries;
		private Dictionary<string, string> packageLists;
		private Dictionary<string, string> idMaps;
		private Dictionary<string, string> videos;
		private ZHeaderLibrary? globalHeaderLibrary;
		private ZHeaderLibrary? headerLibrary;
		private List<string> textureFilters;
		private bool backupResourceLibrary;
		private bool isPlatformPS3;
		private Settings? settings;

		public MainWindow()
		{
			InitializeComponent();

			headerLibraries = new Dictionary<string, string>();
			packageLists = new Dictionary<string, string>();
			idMaps = new Dictionary<string, string>();
			videos = new Dictionary<string, string>();
			textureFilters = new List<string>();
			backupResourceLibrary = true;

			textureFilters.Add(".dds");
			textureFilters.Add(".tga");
			textureFilters.Add(".bmp");
			textureFilters.Add(".png");

			if (File.Exists("HM5ResourceTool.json"))
			{
				string json = File.ReadAllText("HM5ResourceTool.json");
				settings = JsonConvert.DeserializeObject<Settings>(json);

				if (settings != null)
				{
					Utility.ChangeTheme(settings, this);
				}
			}
			else
			{
				settings = new Settings
				{
					Theme = "Dark/Red"
				};

				JsonSerializer serializer = new JsonSerializer();
				using StreamWriter streamWriter = new StreamWriter("HM5ResourceTool.json");
				using JsonWriter writer = new JsonTextWriter(streamWriter);

				serializer.Serialize(streamWriter, settings);
			}

			//TemplateEntityEditor templateEntityEditor = new TemplateEntityEditor();

			//templateEntityEditor.Show();

			//ExtractTypeNames();
		}

		private string ConvertResourceIDToHash(string resourceID)
		{
			string resourceID2 = resourceID.ToLower();

			resourceID2 = resourceID2.Substring(0, resourceID2.LastIndexOf('.') + 1);

			string fileName = CreateMD5(resourceID2);

			return fileName;
		}

		private string ConvertResourceIDToFilePath(string resourceID)
		{
			string resourceID2 = resourceID.ToLower();
			string extension = resourceID2.Substring(resourceID2.LastIndexOf('.') + 1);
			int index = resourceID2.IndexOf(':');
			int index2 = resourceID2.IndexOf('?');
			int index3 = resourceID2.IndexOf(']');
			string folderPath = "";

			if (runtimeFolderPath != null)
			{
				folderPath = runtimeFolderPath;
			}

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

		private void ListDirectories()
		{
			treeView.Items.Clear();

			if (runtimeFolderPath != null)
			{
				DirectoryInfo rootDirectoryInfo = new DirectoryInfo(runtimeFolderPath);

				foreach (var directory in rootDirectoryInfo.GetDirectories())
				{
					treeView.Items.Add(CreateDirectoryNode(directory));
				}
			}
		}

		private TreeViewItem CreateDirectoryNode(DirectoryInfo directoryInfo)
		{
			TreeViewItem directoryNode = new TreeViewItem { Header = directoryInfo.Name };

			foreach (DirectoryInfo directory in directoryInfo.GetDirectories())
			{
				directoryNode.Items.Add(CreateDirectoryNode(directory));
			}

			return directoryNode;
		}

		private void OpenRuntimeFolder_Click(object sender, RoutedEventArgs e)
		{
			OpenFileDialog openFileDialog = new OpenFileDialog
			{
				ValidateNames = false,
				CheckFileExists = false,
				CheckPathExists = true,
				FileName = "Folder Selection."
			};

			if (openFileDialog.ShowDialog() == true)
			{
				runtimeFolderPath = System.IO.Path.GetDirectoryName(openFileDialog.FileName);

				string filePath = Directory.GetFiles(string.Format(@"{0}\common\globalresources.ini", runtimeFolderPath))[0];
				string extension = System.IO.Path.GetExtension(filePath);
				isPlatformPS3 = extension.StartsWith(".ps3");

				ListDirectories();
				LoadResourceIDs(isPlatformPS3);

				string json = File.ReadAllText("HM5ResourceTool.json");
				Settings? settings = JsonConvert.DeserializeObject<Settings>(json);

				if (settings != null && runtimeFolderPath != null)
				{
					settings.GamePath = runtimeFolderPath.Substring(0, runtimeFolderPath.LastIndexOf("runtime"));

					JsonSerializer serializer = new JsonSerializer();
					using StreamWriter streamWriter = new StreamWriter("HM5ResourceTool.json");
					using JsonWriter writer = new JsonTextWriter(streamWriter);

					serializer.Serialize(streamWriter, settings);
				}
			}
		}

		private Item[] GetItems(string folderPath, string extension, Dictionary<string, string> resourceIDs)
        {
			if (runtimeFolderPath == null)
			{
				return new Item[0];
			}

			string path = System.IO.Path.Combine(runtimeFolderPath, folderPath);
			List<string> files = Directory.GetFiles(path, string.Format("*{0}*", extension), SearchOption.TopDirectoryOnly).ToList();

			int count = files.Count;
			Item[] listViewItems = new Item[count];

			for (int i = 0; i < count; i++)
			{
				string fileName = System.IO.Path.GetFileNameWithoutExtension(files[i]);
				string resourceID = resourceIDs[fileName];
				uint resourceLibrariesCount = GetResourceLibrariesCount(files[i]);

				listViewItems[i] = new Item()
				{
					ResourceID = resourceID,
					NumberOfResourceLibraries = resourceLibrariesCount,
					ToolTip = files[i]
				};
			}

			return listViewItems;
		}

		private void LoadResourceIDs(bool loadPS3ResourceIDs = false)
		{
			headerLibraries.Clear();

			if (loadPS3ResourceIDs)
            {
				string[] lines = File.ReadAllLines("HeaderLibraries_PS3.txt");

				foreach (string line in lines)
				{
					headerLibraries.Add(ConvertResourceIDToHash(line), line);
				}

				lines = File.ReadAllLines("PackageLists_PS3.txt");

				foreach (string line in lines)
				{
					packageLists.Add(ConvertResourceIDToHash(line), line);
				}

				lines = File.ReadAllLines("IDMaps_PS3.txt");

				foreach (string line in lines)
				{
					idMaps.Add(ConvertResourceIDToHash(line), line);
				}

				lines = File.ReadAllLines("Videos_PS3.txt");

				foreach (string line in lines)
				{
					videos.Add(ConvertResourceIDToHash(line), line);
				}
			}
            else
            {
				string[] lines = File.ReadAllLines("HeaderLibraries_PC.txt");

				foreach (string line in lines)
				{
					headerLibraries.Add(ConvertResourceIDToHash(line), line);
				}

				lines = File.ReadAllLines("PackageLists_PC.txt");

				foreach (string line in lines)
				{
					packageLists.Add(ConvertResourceIDToHash(line), line);
				}

				lines = File.ReadAllLines("Videos_PC.txt");

				foreach (string line in lines)
				{
					videos.Add(ConvertResourceIDToHash(line), line);
				}
			}
		}

		private uint GetResourceLibrariesCount(string path)
		{
			using BinaryReader2 headerBinaryReader = new BinaryReader2(File.Open(path, FileMode.Open), !isPlatformPS3);

			headerBinaryReader.ReadInt32();
			headerBinaryReader.BaseStream.Seek(headerBinaryReader.ReadInt32() + 64, SeekOrigin.Current);

			return headerBinaryReader.ReadUInt32();
		}

		private void TreeView_SelectedItemChanged(object sender, RoutedPropertyChangedEventArgs<object> e)
		{
			TreeViewItem? treeViewItem = e.NewValue as TreeViewItem;

			if (treeViewItem != null)
			{
				currentFolderPath = GetFullPath(treeViewItem);
				lblCurrentDirectory.Content = string.Format("Current Directory: {0}", currentFolderPath);

				if (currentFolderPath != null)
				{
					List<Item> listViewItems = new List<Item>();
					Item[] headerLibraryItems, packageListItems, idMapItems, videoItems;

					if (isPlatformPS3)
					{
						headerLibraryItems = GetItems(currentFolderPath, "ps3_headerlib", headerLibraries);
						packageListItems = GetItems(currentFolderPath, "ps3_packagelist", packageLists);
						idMapItems = GetItems(currentFolderPath, "ps3_idmap", idMaps);
						videoItems = GetItems(currentFolderPath, "ps3_binkvid", videos);

						listViewItems.AddRange(headerLibraryItems);
						listViewItems.AddRange(packageListItems);
						listViewItems.AddRange(idMapItems);
						listViewItems.AddRange(videoItems);
					}
					else
					{
						headerLibraryItems = GetItems(currentFolderPath, "pc_headerlib", headerLibraries);
						packageListItems = GetItems(currentFolderPath, "pc_packagelist", packageLists);
						videoItems = GetItems(currentFolderPath, "pc_binkvid", videos);

						listViewItems.AddRange(headerLibraryItems);
						listViewItems.AddRange(packageListItems);
						listViewItems.AddRange(videoItems);
					}

					lvResources.ItemsSource = listViewItems;
				}
			}
		}

		private string? GetFullPath(TreeViewItem node)
		{
			if (node == null)
			{
				throw new ArgumentNullException();
			}

			string? result = Convert.ToString(node.Header);

			for (var i = GetParentItem(node); i != null; i = GetParentItem(i))
			{
				result = i.Header + "\\" + result;
			}

			return result;
		}

		private TreeViewItem? GetParentItem(TreeViewItem item)
		{
			for (var i = VisualTreeHelper.GetParent(item); i != null; i = VisualTreeHelper.GetParent(i))
			{
				if (i is TreeViewItem)
				{
					return (TreeViewItem)i;
				}
			}

			return null;
		}

		private async void ViewResourcesMenuItem_Click(object sender, RoutedEventArgs e)
		{
			Item? listViewResource = (Item?)lvResources.SelectedItems[0];

			if (listViewResource != null)
			{
				string? resourceID = listViewResource.ResourceID;

				if (resourceID != null && runtimeFolderPath != null)
				{
					string headerLibraryPath = ConvertResourceIDToFilePath(resourceID);
					string gamePath = runtimeFolderPath.Substring(0, runtimeFolderPath.LastIndexOf("runtime"));

					ProgressWindow progressWindow = new ProgressWindow(settings);

					progressWindow.Show();

					await Task.Run(() => AddResourceLibraries(headerLibraryPath, gamePath));

					progressWindow.Close();

					if (headerLibrary != null && globalHeaderLibrary != null)
					{
						ResourcesWindow resourcesWindow = new ResourcesWindow(headerLibrary, globalHeaderLibrary, settings);

						resourcesWindow.Show();
					}
				}
			}
		}

		private void AddResourceLibraries(string headerLibraryPath, string gamePath)
		{
			if (globalHeaderLibrary == null)
			{
				string globalHeaderLibraryFolderPath = string.Format("{0}/runtime/common/globalresources.ini", gamePath);
				string globalHeaderLibraryFilePath = Directory.GetFiles(globalHeaderLibraryFolderPath, "*.pc_headerlib*")[0];

				globalHeaderLibrary = new ZHeaderLibrary(globalHeaderLibraryFilePath);

				globalHeaderLibrary.ParseHeaderLibrary(gamePath, null, true);
			}

			//HeaderLibrary headerLibrary = new HeaderLibrary(headerLibraryPath);
			headerLibrary = new ZHeaderLibrary(headerLibraryPath);

			if (headerLibraryPath.Contains("pc.layoutconfig"))
			{
				headerLibrary.ParseHeaderLibrary(gamePath, globalHeaderLibrary, false);
			}
			else
			{
				headerLibrary.ParseHeaderLibrary(gamePath, globalHeaderLibrary, true);
			}

			//resourceLibraries = headerLibrary.GetResourceLibraries();
		}

		public void ExtractPropertyNames()
        {
			string gamePath = "D:\\SteamLibrary\\steamapps\\common\\Hitman Absolution";

			if (globalHeaderLibrary == null)
			{
				string globalHeaderLibraryFolderPath = string.Format("{0}/runtime/common/globalresources.ini", gamePath);
				string globalHeaderLibraryFilePath = Directory.GetFiles(globalHeaderLibraryFolderPath, "*.pc_headerlib*")[0];

				globalHeaderLibrary = new ZHeaderLibrary(globalHeaderLibraryFilePath);

				globalHeaderLibrary.ParseHeaderLibrary(gamePath, null, true);
			}

			string[] headerLibraryPaths = Directory.GetFiles("D:\\SteamLibrary\\steamapps\\common\\Hitman Absolution\\runtime", "*pc_headerlib*", SearchOption.AllDirectories);
			int count = headerLibraryPaths.Length;

			for (int i = 901; i < count; ++i)
            {
				string headerLibraryPath = headerLibraryPaths[i];
				bool closeFile = i == count - 1;

				using ZHeaderLibrary headerLibrary = new ZHeaderLibrary(headerLibraryPath);

				if (headerLibraryPath.Contains("pc.layoutconfig"))
				{
					headerLibrary.ParseHeaderLibrary(gamePath, globalHeaderLibrary, false);
				}
				else
				{
					headerLibrary.ParseHeaderLibrary(gamePath, globalHeaderLibrary, true);
				}

				headerLibrary.ExtractPropertyNames(closeFile, false);

                //if (i == 799)
                //{
                //	headerLibrary.ExtractPropertyNames(true, true);

                //	break;
                //}

                //if (i == 900)
                //{
                //    headerLibrary.ExtractPropertyNames(true, true);

                //    break;
                //}
            }
		}

		public void ExtractTypeNames()
		{
			string gamePath = "D:\\SteamLibrary\\steamapps\\common\\Hitman Absolution";

			if (globalHeaderLibrary == null)
			{
				string globalHeaderLibraryFolderPath = string.Format("{0}/runtime/common/globalresources.ini", gamePath);
				string globalHeaderLibraryFilePath = Directory.GetFiles(globalHeaderLibraryFolderPath, "*.pc_headerlib*")[0];

				globalHeaderLibrary = new ZHeaderLibrary(globalHeaderLibraryFilePath);

				globalHeaderLibrary.ParseHeaderLibrary(gamePath, null, true);
			}

			string[] headerLibraryPaths = Directory.GetFiles("D:\\SteamLibrary\\steamapps\\common\\Hitman Absolution\\runtime", "*pc_headerlib*", SearchOption.AllDirectories);
			int count = headerLibraryPaths.Length;

			for (int i = 900; i < count; ++i)
			{
				string headerLibraryPath = headerLibraryPaths[i];
				bool closeFile = i == count - 1;

				using ZHeaderLibrary headerLibrary = new ZHeaderLibrary(headerLibraryPath);

				if (headerLibraryPath.Contains("pc.layoutconfig"))
				{
					headerLibrary.ParseHeaderLibrary(gamePath, globalHeaderLibrary, false);
				}
				else
				{
					headerLibrary.ParseHeaderLibrary(gamePath, globalHeaderLibrary, true);
				}

				headerLibrary.ExtractTypeNames(closeFile, false);

                //if (i == 899)
                //{
                //    headerLibrary.ExtractTypeNames(true, true);

                //    break;
                //}

                //if (i == 799)
                //{
                //	headerLibrary.ExtractPropertyNames(true, true);

                //	break;
                //}

                //if (i == 900)
                //{
                //    headerLibrary.ExtractPropertyNames(true, true);

                //    break;
                //}
            }
		}

		private void ViewVideoMenuItem_Click(object sender, RoutedEventArgs e)
		{
			Item? listViewResource = (Item?)lvResources.SelectedItems[0];

			if (listViewResource != null)
			{
				string? resourceID = listViewResource.ResourceID;

				if (resourceID != null)
				{
					string filePath = ConvertResourceIDToFilePath(resourceID);
					VideoPlayer videoPlayer = new VideoPlayer(filePath);

					videoPlayer.ShowDialog();
				}
			}
		}

		private void ViewPackageListMenuItem_Click(object sender, RoutedEventArgs e)
		{
			Item? listViewResource = (Item?)lvResources.SelectedItems[0];

			if (listViewResource != null)
			{
				string? resourceID = listViewResource.ResourceID;

				if (resourceID != null)
				{
					string filePath = ConvertResourceIDToFilePath(resourceID);
					PackageListWindow packageListWindow = new PackageListWindow(filePath, settings);

					packageListWindow.ShowDialog();
				}
			}
		}

		private void ViewIDMapMenuItem_Click(object sender, RoutedEventArgs e)
		{
			Item? listViewResource = (Item?)lvResources.SelectedItems[0];

			if (listViewResource != null)
			{
				string? resourceID = listViewResource.ResourceID;

				if (resourceID != null)
				{
					string filePath = ConvertResourceIDToFilePath(resourceID);
					IDMapWindow iDMapWindow = new IDMapWindow(filePath, settings);

					iDMapWindow.ShowDialog();
				}
			}
		}

		private async void ExportHeaderLibraryMenuItem_Click(object sender, RoutedEventArgs e)
		{
			Item? listViewResource = (Item?)lvResources.SelectedItems[0];

			if (listViewResource != null)
			{
				string? resourceID = listViewResource.ResourceID;

				if (resourceID != null && runtimeFolderPath != null)
				{
					string filePath = ConvertResourceIDToFilePath(resourceID);
					string gamePath = runtimeFolderPath.Substring(0, runtimeFolderPath.LastIndexOf("runtime"));

					if (globalHeaderLibrary == null)
					{
						string globalHeaderLibraryFolderPath = string.Format("{0}/runtime/common/globalresources.ini", gamePath);
						string globalHeaderLibraryFilePath = Directory.GetFiles(globalHeaderLibraryFolderPath, "*.pc_headerlib*")[0];

						globalHeaderLibrary = new ZHeaderLibrary(globalHeaderLibraryFilePath);

						globalHeaderLibrary.ParseHeaderLibrary(gamePath, null, true);
					}

					headerLibrary = new ZHeaderLibrary(filePath);

					headerLibrary.ParseHeaderLibrary(gamePath, globalHeaderLibrary, false);
					headerLibrary.SerializeToJSON(out string jsonOutput);

					if (DisplayFileDialog(out string outputFilePath, out uint filterIndex, true))
					{
						outputFilePath = string.Format(@"{0}\{1}.json", outputFilePath, System.IO.Path.GetFileNameWithoutExtension(filePath));

						File.WriteAllText(outputFilePath, jsonOutput);

						await this.ShowMessageAsync("HM5 Resource Tool", "Header library successfully converted to JSON.");
					}
				}
			}
		}

		private async void ExportPackageListMenuItem_Click(object sender, RoutedEventArgs e)
		{
			Item? listViewResource = (Item?)lvResources.SelectedItems[0];

			if (listViewResource != null)
			{
				string? resourceID = listViewResource.ResourceID;

				if (resourceID != null)
				{
					string inputFilePath = ConvertResourceIDToFilePath(resourceID);

					if (DisplayFileDialog(out string outputFilePath, out uint filterIndex, true))
					{
						outputFilePath = string.Format(@"{0}\{1}.json", outputFilePath, System.IO.Path.GetFileNameWithoutExtension(inputFilePath));

						if (ZResourceExporter.ExportPackageList(inputFilePath, outputFilePath))
						{
							await this.ShowMessageAsync("HM5 Resource Tool", "Package list successfully converted to JSON.");
						}
						else
						{
							await this.ShowMessageAsync("HM5 Resource Tool", "Failed to convert package list to JSON.");
						}
					}
				}
			}
		}

		private async void ExportIDMapMenuItem_Click(object sender, RoutedEventArgs e)
		{
			Item? listViewResource = (Item?)lvResources.SelectedItems[0];

			if (listViewResource != null)
			{
				string? resourceID = listViewResource.ResourceID;

				if (resourceID != null)
				{
					string inputFilePath = ConvertResourceIDToFilePath(resourceID);

					if (DisplayFileDialog(out string outputFilePath, out uint filterIndex, true))
					{
						outputFilePath = string.Format(@"{0}\{1}.txt", outputFilePath, System.IO.Path.GetFileNameWithoutExtension(inputFilePath));

                        if (ZResourceExporter.ExportPackageList(inputFilePath, outputFilePath))
                        {
                            await this.ShowMessageAsync("HM5 Resource Tool", "Package list successfully converted to JSON.");
                        }
                        else
                        {
                            await this.ShowMessageAsync("HM5 Resource Tool", "Failed to convert package list to JSON.");
                        }
                    }
				}
			}
		}

		private async void ExportVideoMenuItem_Click(object sender, RoutedEventArgs e)
		{
			Item? listViewResource = (Item?)lvResources.SelectedItems[0];

			if (listViewResource != null)
			{
				string? resourceID = listViewResource.ResourceID;

				if (resourceID != null)
				{
					string inputFilePath = ConvertResourceIDToFilePath(resourceID);

					if (DisplayFileDialog(out string outputFilePath, out uint filterIndex, true))
					{
						outputFilePath = string.Format(@"{0}\{1}.json", outputFilePath, System.IO.Path.GetFileNameWithoutExtension(inputFilePath));

						if (ZResourceExporter.ExportPCBinkVid(inputFilePath, outputFilePath))
						{
							await this.ShowMessageAsync("HM5 Resource Tool", "pc_binkvid successfully converted to BINK.");
						}
						else
						{
							await this.ShowMessageAsync("HM5 Resource Tool", "Failed to convert pc_binkvid to BINK.");
						}
					}
				}
			}
		}

		private void ShowInFileExplorerMenuItem_Click(object sender, RoutedEventArgs e)
		{
			Item? listViewResource = (Item?)lvResources.SelectedItems[0];

			if (listViewResource != null && listViewResource.ResourceID != null)
			{
				string filePath = ConvertResourceIDToFilePath(listViewResource.ResourceID);
				string argument = "/select, \"" + filePath + "\"";

				Process.Start("explorer.exe", argument);
			}
		}

		private async void ExtractResourcesFromHeaderLibraryMenuItem_Click(object sender, RoutedEventArgs e)
		{
			Item? listViewResource = (Item?)lvResources.SelectedItems[0];

			if (listViewResource != null)
			{
				string? resourceID = listViewResource.ResourceID;

				if (resourceID != null && runtimeFolderPath != null)
				{
					string inputFilePath = ConvertResourceIDToFilePath(resourceID);
					string gamePath = runtimeFolderPath.Substring(0, runtimeFolderPath.LastIndexOf("runtime"));

					ZHeaderLibrary headerLibrary = new ZHeaderLibrary(inputFilePath);

					if (DisplayFileDialog(out string exportFolderPath, out uint filterIndex, true))
					{
						if (globalHeaderLibrary == null)
						{
							string globalHeaderLibraryFolderPath = string.Format("{0}/runtime/common/globalresources.ini", gamePath);
							string globalHeaderLibraryFilePath = Directory.GetFiles(globalHeaderLibraryFolderPath, "*.pc_headerlib*")[0];

							globalHeaderLibrary = new ZHeaderLibrary(globalHeaderLibraryFilePath);

							globalHeaderLibrary.ParseHeaderLibrary(gamePath, null, true);
						}

						headerLibrary.ExtractResourceLibraries(gamePath, globalHeaderLibrary, exportFolderPath);

						await this.ShowMessageAsync("HM5 Resource Tool", "Resources successfully exported.");
					}
				}
			}
		}

		private void ContextMenu_ContextMenuOpening(object sender, ContextMenuEventArgs e)
		{
			if (lvResources.SelectedItems.Count == 0)
			{
				return;
			}

			Item? listViewResource = (Item?)lvResources.SelectedItems[0];

			if (listViewResource != null && listViewResource.ResourceID != null)
			{
				if (listViewResource.ResourceID.EndsWith("headerlib"))
				{
					viewResourcesMenuItem.Visibility = Visibility.Visible;
					viewVideoMenuItem.Visibility = Visibility.Collapsed;
					viewPackageListMenuItem.Visibility = Visibility.Collapsed;
					exportHeaderLibraryMenuItem.Visibility = Visibility.Visible;
					exportVideoMenuItem.Visibility = Visibility.Collapsed;
					exportPackageListMenuItem.Visibility = Visibility.Collapsed;
					showInFileExplorerMenuItem.Visibility = Visibility.Visible;
					extractResourcesFromHeaderLibraryMenuItem.Visibility = Visibility.Visible;
				}
				else if (listViewResource.ResourceID.EndsWith("packagelist"))
				{
					viewResourcesMenuItem.Visibility = Visibility.Collapsed;
					viewVideoMenuItem.Visibility = Visibility.Collapsed;
					viewPackageListMenuItem.Visibility = Visibility.Visible;
					exportHeaderLibraryMenuItem.Visibility = Visibility.Collapsed;
					exportVideoMenuItem.Visibility = Visibility.Collapsed;
					exportPackageListMenuItem.Visibility = Visibility.Visible;
					showInFileExplorerMenuItem.Visibility = Visibility.Visible;
					extractResourcesFromHeaderLibraryMenuItem.Visibility = Visibility.Collapsed;
				}
				else if (listViewResource.ResourceID.EndsWith("idmap"))
				{
					viewResourcesMenuItem.Visibility = Visibility.Collapsed;
					viewVideoMenuItem.Visibility = Visibility.Collapsed;
					viewPackageListMenuItem.Visibility = Visibility.Collapsed;
					viewIDMapMenuItem.Visibility = Visibility.Visible;
					exportHeaderLibraryMenuItem.Visibility = Visibility.Collapsed;
					exportVideoMenuItem.Visibility = Visibility.Collapsed;
					exportPackageListMenuItem.Visibility = Visibility.Collapsed;
					exportIDMapMenuItem.Visibility = Visibility.Visible;
					showInFileExplorerMenuItem.Visibility = Visibility.Visible;
					extractResourcesFromHeaderLibraryMenuItem.Visibility = Visibility.Collapsed;
				}
				else if (listViewResource.ResourceID.EndsWith("binkvid"))
				{
					viewResourcesMenuItem.Visibility = Visibility.Collapsed;
					viewVideoMenuItem.Visibility = Visibility.Visible;
					viewPackageListMenuItem.Visibility = Visibility.Collapsed;
					exportHeaderLibraryMenuItem.Visibility = Visibility.Collapsed;
					exportPackageListMenuItem.Visibility = Visibility.Collapsed;
					exportVideoMenuItem.Visibility = Visibility.Visible;
					showInFileExplorerMenuItem.Visibility = Visibility.Visible;
					extractResourcesFromHeaderLibraryMenuItem.Visibility = Visibility.Collapsed;
				}
			}
		}

		private void ChangeThemeMenuItem_Click(object sender, RoutedEventArgs e)
		{
			MenuItem? menuItem = sender as MenuItem;

			if (menuItem != null)
			{
				string? theme = menuItem.Header as string;

				if (settings != null && theme != null)
				{
					settings.Theme = theme;

					Utility.ChangeTheme(settings, this);
				}
			}
		}

		private static bool DisplayFileDialog(out string path, out uint filterIndex, bool save = false, string filter = "")
		{
			OpenFileDialog openFileDialog = new OpenFileDialog();

			openFileDialog.Filter = filter;

			if (save)
			{
				openFileDialog.ValidateNames = false;
				openFileDialog.CheckFileExists = false;
				openFileDialog.CheckPathExists = true;
				openFileDialog.FileName = "Folder Selection.";
			}

			if (openFileDialog.ShowDialog() ?? true)
			{
				if (save)
				{
					string? path2 = System.IO.Path.GetDirectoryName(openFileDialog.FileName);

					if (path2 != null)
					{
						path = path2;
					}
					else
					{
						path = "";
					}
				}
				else
				{
					path = openFileDialog.FileName;
				}

				filterIndex = (uint)openFileDialog.FilterIndex;

				return true;
			}

			path = "";
			filterIndex = 0;

			return false;
		}

		private void ExportResourceMenuItem_Click(object sender, RoutedEventArgs e)
		{
			MenuItem? menuItem = sender as MenuItem;

			if (menuItem == null)
			{
				return;
			}

			string? fileFormat = menuItem.Tag as string;

			switch (fileFormat)
			{
				case "TEMP":
					ExportTEMP();
					break;
				case "TBLU":
					ExportTBLU();
					break;
				case "CPPT":
					ExportCPPT();
					break;
				case "CBLU":
					ExportCBLU();
					break;
				case "AIBB":
					ExportAIBB();
					break;
				case "TEXT":
					ExportTEXT();
					break;
				case "FSBF":
					ExportFSBF();
					break;
				case "FSBM":
					ExportFSBM();
					break;
				case "FSBS":
					ExportFSBS();
					break;
				case "WAVB":
					ExportWAVB();
					break;
				case "MUCB":
					ExportMUCB();
					break;
				case "SWFF":
					ExportSWFF();
					break;
				case "GFXF":
					ExportGFXF();
					break;
				case "TELI":
					ExportTELI();
					break;
				case "PRIM":
					ExportPRIM();
					break;
				case "MATI":
					ExportMATI();
					break;
				case "MATT":
					ExportMATT();
					break;
				case "MATB":
					ExportMATB();
					break;
				case "pc_binkvid":
					ExportPCBinkVid();
					break;
				case "pc_packagelist":
					ExportPCPackageList();
					break;
				default:
					break;
			}
		}

		private async void ExportTEMP()
		{
			if (DisplayFileDialog(out string inputFilePath, out uint filterIndex, false, "TEMP Files|*.TEMP;"))
			{
				if (DisplayFileDialog(out string outputFilePath, out uint filterIndex2, true))
				{
					outputFilePath = string.Format(@"{0}\{1}.json", outputFilePath, System.IO.Path.GetFileNameWithoutExtension(inputFilePath));

					if (ZResourceExporter.ExportTEMP(inputFilePath, outputFilePath))
					{
						await this.ShowMessageAsync("HM5 Resource Tool", "TEMP successfully exported.");
					}
					else
					{
						await this.ShowMessageAsync("HM5 Resource Tool", "Failed to export TEMP.");
					}
				}
			}
		}

		private async void ExportTBLU()
		{
			if (DisplayFileDialog(out string inputFilePath, out uint filterIndex, false, "TBLU Files|*.TBLU;"))
			{
				if (DisplayFileDialog(out string outputFilePath, out uint filterIndex2, true))
				{
					outputFilePath = string.Format(@"{0}\{1}.json", outputFilePath, System.IO.Path.GetFileNameWithoutExtension(inputFilePath));

					if (ZResourceExporter.ExportTBLU(inputFilePath, outputFilePath))
					{
						await this.ShowMessageAsync("HM5 Resource Tool", "TBLU successfully exported.");
					}
					else
					{
						await this.ShowMessageAsync("HM5 Resource Tool", "Failed to export TBLU.");
					}
				}
			}
		}

		private async void ExportCPPT()
		{
			if (DisplayFileDialog(out string inputFilePath, out uint filterIndex, false, "CPPT Files|*.CPPT;"))
			{
				if (DisplayFileDialog(out string outputFilePath, out uint filterIndex2, true))
				{
					outputFilePath = string.Format(@"{0}\{1}.json", outputFilePath, System.IO.Path.GetFileNameWithoutExtension(inputFilePath));

					if (ZResourceExporter.ExportCPPT(inputFilePath, outputFilePath))
					{
						await this.ShowMessageAsync("HM5 Resource Tool", "CPPT successfully exported.");
					}
					else
					{
						await this.ShowMessageAsync("HM5 Resource Tool", "Failed to export CPPT.");
					}
				}
			}
		}

		private async void ExportCBLU()
		{
			if (DisplayFileDialog(out string inputFilePath, out uint filterIndex, false, "CBLU Files|*.CBLU;"))
			{
				if (DisplayFileDialog(out string outputFilePath, out uint filterIndex2, true))
				{
					outputFilePath = string.Format(@"{0}\{1}.json", outputFilePath, System.IO.Path.GetFileNameWithoutExtension(inputFilePath));

					if (ZResourceExporter.ExportCBLU(inputFilePath, outputFilePath))
					{
						await this.ShowMessageAsync("HM5 Resource Tool", "CBLU successfully exported.");
					}
					else
					{
						await this.ShowMessageAsync("HM5 Resource Tool", "Failed to export CBLU.");
					}
				}
			}
		}

		private async void ExportAIBB()
		{
			if (DisplayFileDialog(out string inputFilePath, out uint filterIndex, false, "AIBB Files|*.AIBB;"))
			{
				if (DisplayFileDialog(out string outputFilePath, out uint filterIndex2, true))
				{
					outputFilePath = string.Format(@"{0}\{1}.json", outputFilePath, System.IO.Path.GetFileNameWithoutExtension(inputFilePath));

					if (ZResourceExporter.ExportAIBB(inputFilePath, outputFilePath))
					{
						await this.ShowMessageAsync("HM5 Resource Tool", "AIBB successfully exported.");
					}
					else
					{
						await this.ShowMessageAsync("HM5 Resource Tool", "Failed to export AIBB.");
					}
				}
			}
		}

		private async void ExportTEXT()
		{
			if (DisplayFileDialog(out string inputFilePath, out uint filterIndex, false, "TEXT Files|*.TEXT"))
			{
				if (DisplayFileDialog(out string outputFilePath, out uint filterIndex2, true))
				{
					List<string> extensions = new List<string>();

					extensions.Add("DDS");
					extensions.Add("TGA");
					extensions.Add("BMP");
					extensions.Add("PNG");

					ExportDialog exportDialog = new ExportDialog(extensions);

					if (exportDialog.ShowDialog() ?? true)
					{
						outputFilePath = string.Format(@"{0}\{1}{2}.", outputFilePath, System.IO.Path.GetFileNameWithoutExtension(inputFilePath), extensions[(int)exportDialog.Option]);

						if (ZResourceExporter.ExportTEXT(inputFilePath, outputFilePath, filterIndex2 - 1))
						{
							await this.ShowMessageAsync("HM5 Resource Tool", "TEXT successfully exported.");
						}
						else
						{
							await this.ShowMessageAsync("HM5 Resource Tool", "Failed to export TEXT.");
						}
					}
				}
			}
		}

		private async void ExportFSBF()
		{
			if (DisplayFileDialog(out string inputFilePath, out uint filterIndex, false, "FSBF Files|*.FSBF;"))
			{
				if (DisplayFileDialog(out string outputFilePath, out uint filterIndex2, true))
				{
					outputFilePath = string.Format(@"{0}\{1}", outputFilePath, System.IO.Path.GetFileNameWithoutExtension(inputFilePath));

					if (ZResourceExporter.ExportFSBF(inputFilePath, outputFilePath))
					{
						await this.ShowMessageAsync("HM5 Resource Tool", "FSBF successfully exported.");
					}
					else
					{
						await this.ShowMessageAsync("HM5 Resource Tool", "Failed to export FSBF.");
					}
				}
			}
		}

		private async void ExportFSBM()
		{
			if (DisplayFileDialog(out string inputFilePath, out uint filterIndex, false, "FSBM Files|*.FSBM;"))
			{
				if (DisplayFileDialog(out string outputFilePath, out uint filterIndex2, true))
				{
					outputFilePath = string.Format(@"{0}\{1}", outputFilePath, System.IO.Path.GetFileNameWithoutExtension(inputFilePath));

					if (ZResourceExporter.ExportFSBM(inputFilePath, outputFilePath))
					{
						await this.ShowMessageAsync("HM5 Resource Tool", "FSBM successfully exported.");
					}
					else
					{
						await this.ShowMessageAsync("HM5 Resource Tool", "Failed to export FSBM.");
					}
				}
			}
		}

		private async void ExportFSBS()
		{
			if (DisplayFileDialog(out string inputFilePath, out uint filterIndex, false, "FSBS Files|*.FSBS;"))
			{
				if (DisplayFileDialog(out string outputFilePath, out uint filterIndex2, true))
				{
					outputFilePath = string.Format(@"{0}\{1}", outputFilePath, System.IO.Path.GetFileNameWithoutExtension(inputFilePath));

					if (ZResourceExporter.ExportFSBS(inputFilePath, outputFilePath))
					{
						await this.ShowMessageAsync("HM5 Resource Tool", "FSBS successfully exported.");
					}
					else
					{
						await this.ShowMessageAsync("HM5 Resource Tool", "Failed to export FSBS.");
					}
				}
			}
		}

		private async void ExportWAVB()
		{
			if (DisplayFileDialog(out string inputFilePath, out uint filterIndex, false, "WAVB Files|*.WAVB;"))
			{
				if (DisplayFileDialog(out string outputFilePath, out uint filterIndex2, true))
				{
					outputFilePath = string.Format(@"{0}\{1}.json", outputFilePath, System.IO.Path.GetFileNameWithoutExtension(inputFilePath));

					if (ZResourceExporter.ExportWAVB(inputFilePath, outputFilePath))
					{
						await this.ShowMessageAsync("HM5 Resource Tool", "WAVB successfully exported.");
					}
					else
					{
						await this.ShowMessageAsync("HM5 Resource Tool", "Failed to export WAVB.");
					}
				}
			}
		}

		private async void ExportMUCB()
		{
			if (DisplayFileDialog(out string inputFilePath, out uint filterIndex, false, "MUCB Files|*.MUCB;"))
			{
				if (DisplayFileDialog(out string outputFilePath, out uint filterIndex2, true))
				{
					outputFilePath = string.Format(@"{0}\{1}.json", outputFilePath, System.IO.Path.GetFileNameWithoutExtension(inputFilePath));

					if (ZResourceExporter.ExportMUCB(inputFilePath, outputFilePath))
					{
						await this.ShowMessageAsync("HM5 Resource Tool", "MUCB successfully exported.");
					}
					else
					{
						await this.ShowMessageAsync("HM5 Resource Tool", "Failed to export MUCB.");
					}
				}
			}
		}

		private async void ExportSWFF()
		{
			if (DisplayFileDialog(out string inputFilePath, out uint filterIndex, false, "SWFF Files|*.SWFF;"))
			{
				if (DisplayFileDialog(out string outputFilePath, out uint filterIndex2, true))
				{
					outputFilePath = string.Format(@"{0}\{1}", outputFilePath, System.IO.Path.GetFileNameWithoutExtension(inputFilePath));

					if (ZResourceExporter.ExportSWFF(inputFilePath, outputFilePath))
					{
						await this.ShowMessageAsync("HM5 Resource Tool", "SWFF successfully exported.");
					}
					else
					{
						await this.ShowMessageAsync("HM5 Resource Tool", "Failed to export SWFF.");
					}
				}
			}
		}

		private async void ExportGFXF()
		{
			if (DisplayFileDialog(out string inputFilePath, out uint filterIndex, false, "SWFF Files|*.SWFF;"))
			{
				if (DisplayFileDialog(out string outputFilePath, out uint filterIndex2, true))
				{
					outputFilePath = string.Format(@"{0}\{1}.gfx", outputFilePath, System.IO.Path.GetFileNameWithoutExtension(inputFilePath));

					if (ZResourceExporter.ExportGFXF(inputFilePath, outputFilePath))
					{
						await this.ShowMessageAsync("HM5 Resource Tool", "GFXF successfully exported.");
					}
					else
					{
						await this.ShowMessageAsync("HM5 Resource Tool", "Failed to export GFXF.");
					}
				}
			}
		}

		private async void ExportTELI()
		{
			if (DisplayFileDialog(out string inputFilePath, out uint filterIndex, false, "TELI Files|*.TELI;"))
			{
				if (DisplayFileDialog(out string outputFilePath, out uint filterIndex2, true))
				{
					outputFilePath = string.Format(@"{0}\{1}.txt", outputFilePath, System.IO.Path.GetFileNameWithoutExtension(inputFilePath));

					if (ZResourceExporter.ExportTELI(inputFilePath, outputFilePath))
					{
						await this.ShowMessageAsync("HM5 Resource Tool", "TELI successfully exported.");
					}
					else
					{
						await this.ShowMessageAsync("HM5 Resource Tool", "Failed to export TELI.");
					}
				}
			}
		}

		private async void ExportPRIM()
		{
			if (DisplayFileDialog(out string inputFilePath, out uint filterIndex, false, "PRIM Files|*.PRIM;"))
			{
				if (DisplayFileDialog(out string outputFilePath, out uint filterIndex2, true))
				{
					outputFilePath = string.Format(@"{0}\{1}.gltf", outputFilePath, System.IO.Path.GetFileNameWithoutExtension(inputFilePath));

					if (ZResourceExporter.ExportPRIM(inputFilePath, outputFilePath))
					{
						await this.ShowMessageAsync("HM5 Resource Tool", "PRIM successfully exported.");
					}
					else
					{
						await this.ShowMessageAsync("HM5 Resource Tool", "Failed to export PRIM.");
					}
				}
			}
		}

		private async void ExportMATI()
		{
			if (DisplayFileDialog(out string inputFilePath, out uint filterIndex, false, "MATI Files|*.MATI;"))
			{
				if (DisplayFileDialog(out string outputFilePath, out uint filterIndex2, true))
				{
					outputFilePath = string.Format(@"{0}\{1}.json", outputFilePath, System.IO.Path.GetFileNameWithoutExtension(inputFilePath));

					if (ZResourceExporter.ExportMATI(inputFilePath, outputFilePath))
					{
						await this.ShowMessageAsync("HM5 Resource Tool", "MATI successfully exported.");
					}
					else
					{
						await this.ShowMessageAsync("HM5 Resource Tool", "Failed to export MATI.");
					}
				}
			}
		}

		private async void ExportMATT()
		{
			if (DisplayFileDialog(out string inputFilePath, out uint filterIndex, false, "MATT Files|*.MATT;"))
			{
				if (DisplayFileDialog(out string outputFilePath, out uint filterIndex2, true))
				{
					outputFilePath = string.Format(@"{0}\{1}.json", outputFilePath, System.IO.Path.GetFileNameWithoutExtension(inputFilePath));

					if (ZResourceExporter.ExportMATT(inputFilePath, outputFilePath))
					{
						await this.ShowMessageAsync("HM5 Resource Tool", "MATT successfully exported.");
					}
					else
					{
						await this.ShowMessageAsync("HM5 Resource Tool", "Failed to export MATT.");
					}
				}
			}
		}

		private async void ExportMATB()
		{
			if (DisplayFileDialog(out string inputFilePath, out uint filterIndex, false, "MATB Files|*.MATB;"))
			{
				if (DisplayFileDialog(out string outputFilePath, out uint filterIndex2, true))
				{
					outputFilePath = string.Format(@"{0}\{1}.json", outputFilePath, System.IO.Path.GetFileNameWithoutExtension(inputFilePath));

					if (ZResourceExporter.ExportMATB(inputFilePath, outputFilePath))
					{
						await this.ShowMessageAsync("HM5 Resource Tool", "MATB successfully exported.");
					}
					else
					{
						await this.ShowMessageAsync("HM5 Resource Tool", "Failed to export MATB.");
					}
				}
			}
		}

		private async void ExportPCBinkVid()
		{
			if (DisplayFileDialog(out string inputFilePath, out uint filterIndex, false, "pc_binkvid Files|*.pc_binkvid;"))
			{
				if (DisplayFileDialog(out string outputFilePath, out uint filterIndex2, true))
				{
					outputFilePath = string.Format(@"{0}\{1}.bik", outputFilePath, System.IO.Path.GetFileNameWithoutExtension(inputFilePath));

					if (ZResourceExporter.ExportPCBinkVid(inputFilePath, outputFilePath))
					{
						await this.ShowMessageAsync("HM5 Resource Tool", "pc_binkvid successfully exported.");
					}
					else
					{
						await this.ShowMessageAsync("HM5 Resource Tool", "Failed to export pc_binkvid.");
					}
				}
			}
		}

		private async void ExportPCPackageList()
		{
			if (DisplayFileDialog(out string inputFilePath, out uint filterIndex, false, "pc_packagelist Files|*.pc_packagelist;"))
			{
				if (DisplayFileDialog(out string outputFilePath, out uint filterIndex2, true))
				{
					outputFilePath = string.Format(@"{0}\{1}.json", outputFilePath, System.IO.Path.GetFileNameWithoutExtension(inputFilePath));

					if (ZResourceExporter.ExportPackageList(inputFilePath, outputFilePath))
					{
						await this.ShowMessageAsync("HM5 Resource Tool", "pc_packagelist successfully exported.");
					}
					else
					{
						await this.ShowMessageAsync("HM5 Resource Tool", "Failed to export pc_packagelist.");
					}
				}
			}
		}

		private async void ImportResourceMenuItem_Click(object sender, RoutedEventArgs e)
		{
			MenuItem? menuItem = sender as MenuItem;

			if (menuItem == null)
			{
				return;
			}

			string? fileFormat = menuItem.Tag as string;
			bool result = false;
			string? inputFilePath = null;
			uint filterIndex;

			switch (fileFormat)
			{
				case "TEMP":
					result = DisplayFileDialog(out inputFilePath, out filterIndex, false, "JSON Files|*.json;");
					break;
				case "TBLU":
					result = DisplayFileDialog(out inputFilePath, out filterIndex, false, "JSON Files|*.json;");
					break;
				case "CPPT":
					result = DisplayFileDialog(out inputFilePath, out filterIndex, false, "JSON Files|*.json;");
					break;
				case "CBLU":
					result = DisplayFileDialog(out inputFilePath, out filterIndex, false, "JSON Files|*.json;");
					break;
				case "AIBB":
					result = DisplayFileDialog(out inputFilePath, out filterIndex, false, "JSON Files|*.json;");
					break;
				case "TEXT":
					result = DisplayFileDialog(out inputFilePath, out filterIndex, false, "DDS Files|*.dds;TGA Files|*.tga;BMP Files|*.bmp;PNG Files|*.png;");
					break;
			}

			if (!result || inputFilePath == null)
			{
				return;
			}

			string? directoryName = System.IO.Path.GetDirectoryName(inputFilePath);

			if (directoryName == null)
			{
				return;
			}

			string metadataFilePath = string.Format("{0}.meta.json", System.IO.Path.Combine(directoryName, System.IO.Path.GetFileNameWithoutExtension(inputFilePath)));
			string json = File.ReadAllText("HM5ResourceTool.json");
			Settings? settings = JsonConvert.DeserializeObject<Settings>(json);

			if (settings == null || settings.GamePath == null)
			{
				return;
			}

			if (!backupResourceLibrary)
			{
				MessageDialogResult messageDialogResult = await this.ShowMessageAsync("HM5 Resource Tool", "Do you want to backup header library and resource library?",
					MessageDialogStyle.AffirmativeAndNegative);

				backupResourceLibrary = messageDialogResult == MessageDialogResult.Affirmative;
			}

			ZResourceImporter resourceImporter = new ZResourceImporter(metadataFilePath, settings.GamePath);

			resourceImporter.ImportResource(inputFilePath, backupResourceLibrary);
		}

		private void ConvertAssemblyPathToFilePathMenuItem_Click(object sender, RoutedEventArgs e)
		{
			FilePathGeneratorWindow filePathGeneratorWindow = new FilePathGeneratorWindow();

			filePathGeneratorWindow.ShowDialog();
		}

		private void ConvertRuntimeResourceIDToAssemblyPathMenuItem_Click(object sender, RoutedEventArgs e)
		{
			AssemblyPathGeneratorWindow assemblyPathGeneratorWindow = new AssemblyPathGeneratorWindow();

			assemblyPathGeneratorWindow.ShowDialog();
		}

		private void MainWindow_Closed(object sender, EventArgs e)
		{
			if (headerLibrary != null)
			{
				headerLibrary.Dispose();
			}

			if (globalHeaderLibrary != null)
			{
				globalHeaderLibrary.Dispose();
			}
		}
    }
}
