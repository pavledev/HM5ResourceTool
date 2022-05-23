using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
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
using System.Xml;
using System.Collections.ObjectModel;
using System.Threading;
using System.Reflection;
using System.Windows.Media.Media3D;
using Microsoft.Win32;
using MahApps.Metro.Controls;
using MahApps.Metro.Controls.Dialogs;
using HM5ResourceToolCLI;
using ICSharpCode.AvalonEdit.Highlighting;
using ICSharpCode.AvalonEdit.Highlighting.Xshd;
using HelixToolkit.Wpf;
using Newtonsoft.Json;

namespace HM5ResourceTool
{
    /// <summary>
    /// Interaction logic for ResourcesWindow.xaml
    /// </summary>
    public partial class ResourcesWindow : MetroWindow
    {
        private ZHeaderLibrary headerLibrary;
        private ZHeaderLibrary globalHeaderLibrary;
        private List<ZResourceLibrary> resourceLibraries;
        private List<ZResource>? resources;
        private LibVLCSharp.Shared.LibVLC? libVLC;
        private LibVLCSharp.Shared.MediaPlayer? mediaPlayer;
        private LibVLCSharp.Shared.StreamMediaInput? mediaInput;
        private MemoryStream? memoryStream;
        private List<SAudioSample>? audioSamples = null;
        private Dictionary<string, string> materialPropertyNames;
        private bool backupResourceLibrary;
        private Settings? settings;

        //public ResourcesWindow(List<ResourceLibrary> resourceLibraries)
        //{
        //    InitializeComponent();

        //    //Grid DynamicGrid = new Grid();

        //    //viewTab.Content = DynamicGrid;

        //    this.resourceLibraries = resourceLibraries;

        //    SetupTextEditor();
        //    SetupVLCPlayer();
        //}

        public ResourcesWindow(ZHeaderLibrary headerLibrary, ZHeaderLibrary globalHeaderLibrary, Settings? settings)
        {
            InitializeComponent();

            this.headerLibrary = headerLibrary;
            this.globalHeaderLibrary = globalHeaderLibrary;
            this.settings = settings;
            resourceLibraries = headerLibrary.GetResourceLibraries();
            materialPropertyNames = new Dictionary<string, string>();
            backupResourceLibrary = true;

            materialPropertyNames.Add("AREF", "Alpha Reference");
            materialPropertyNames.Add("ATST", "Alpha Test Enabled");
            materialPropertyNames.Add("BENA", "Blend Enabled");
            materialPropertyNames.Add("BIND", "Binder");
            materialPropertyNames.Add("BMOD", "Blend Mode");
            materialPropertyNames.Add("COLO", "Color");
            materialPropertyNames.Add("CULL", "Culling Mode");
            materialPropertyNames.Add("DBDE", "Decal Blend Diffuse");
            materialPropertyNames.Add("DBEE", "Decal Blend Emission");
            materialPropertyNames.Add("DBNE", "Decal Blend Normal");
            materialPropertyNames.Add("DBRE", "Decal Blend Roughness");
            materialPropertyNames.Add("DBSE", "Decal Blend Specular");
            materialPropertyNames.Add("ENAB", "Enabled");
            materialPropertyNames.Add("FENA", "Fog Enabled");
            materialPropertyNames.Add("FLTV", "Float Value");
            materialPropertyNames.Add("INST", "Instance");
            materialPropertyNames.Add("NAME", "Name");
            materialPropertyNames.Add("OPAC", "Opacity");
            materialPropertyNames.Add("RSTA", "Render State");
            materialPropertyNames.Add("SSBW", "Subsurface Value");
            materialPropertyNames.Add("SSVB", "Subsurface Blue");
            materialPropertyNames.Add("SSVG", "Subsurface Green");
            materialPropertyNames.Add("SSVR", "Subsurface Red");
            materialPropertyNames.Add("TAGS", "Tags");
            materialPropertyNames.Add("TEXT", "Texture");
            materialPropertyNames.Add("TILU", "Tiling U");
            materialPropertyNames.Add("TILV", "Tiling V");
            materialPropertyNames.Add("TXID", "Texture Id");
            materialPropertyNames.Add("TYPE", "Type");
            materialPropertyNames.Add("VALU", "Value");
            materialPropertyNames.Add("ZBIA", "Z Bias");
            materialPropertyNames.Add("ZOFF", "Z Offset");

            textEditor.Options.IndentationSize = 4;

            LoadJSONSyntaxHighlighter();
            SetupVLCPlayer();

            if (settings != null && settings.Theme != null)
            {
                Utility.ChangeTheme(settings, this, false);
            }
        }

        private void ResourcesWindow_Loaded(object sender, RoutedEventArgs e)
        {
            AddResourceLibraries();
        }

        private void CbResourceLibraries_SelectionChanged(object sender, SelectionChangedEventArgs e)
        {
            AddResourcesToTable(cbResourceLibraries.SelectedIndex);
        }

        private void AddResourceLibraries()
        {
            int resourceLibrariesCount = resourceLibraries.Count;

            for (int i = 0; i < resourceLibrariesCount; ++i)
            {
                string resourceID = resourceLibraries[i].GetResourceID();

                cbResourceLibraries.Items.Add(resourceID);
            }
        }

        private void AddResourcesToTable(int index)
        {
            resources = resourceLibraries[index].GetResources();

            int count = resources.Count;
            ResourceItem[] listViewItems = new ResourceItem[count];

            for (int i = 0; i < count; ++i)
            {
                listViewItems[i] = new ResourceItem()
                {
                    IndexInLibrary = i,
                    Name = resources[i].GetName(),
                    FileFormat = resources[i].GetFileFormat()
                };
            }

            lvResources.ItemsSource = null;
            lvResources.ItemsSource = listViewItems;
        }

        private void LVResources_SelectionChanged(object sender, SelectionChangedEventArgs e)
        {
            switch (tabControl.SelectedIndex)
            {
                case 0:
                    DisplayResourceDetails();
                    DisplayResource();
                    break;
                case 1:
                    DisplayHexBytesFromHeaderLibrary();
                    break;
                case 2:
                    DisplayHexBytesFromResourceLibrary();
                    break;
                default:
                    DisplayResource();
                    break;
            }
        }

        private void DisplayResourceDetails()
        {
            ResourceItem? resourceItem = (ResourceItem?)lvResources.SelectedItem;

            if (resourceItem == null || resources == null)
            {
                return;
            }

            ZResource resource = resources[resourceItem.IndexInLibrary];

            if (!resource.IsResourceDataLoaded())
            {
                resourceLibraries[cbResourceLibraries.SelectedIndex].LoadResourceData((uint)resourceItem.IndexInLibrary);
                headerLibrary.LoadResourceReferences((uint)cbResourceLibraries.SelectedIndex, (uint)resourceItem.IndexInLibrary, globalHeaderLibrary);
            }

            txtDetails.Text = resource.GetResourceDetails();
        }

        private void DisplayHexBytesFromHeaderLibrary()
        {
            ResourceItem? resourceItem = (ResourceItem?)lvResources.SelectedItem;

            if (resourceItem == null || resources == null)
            {
                return;
            }

            ZResource resource = resources[resourceItem.IndexInLibrary];

            txtHeaderLibraryHex.Text = resource.GetHexBytesFromHeaderLibrary();
        }

        private void DisplayHexBytesFromResourceLibrary()
        {
            ResourceItem? resourceItem = (ResourceItem?)lvResources.SelectedItem;

            if (resourceItem == null || resources == null)
            {
                return;
            }

            ZResource resource = resources[resourceItem.IndexInLibrary];

            txtResourceLibraryHex.Text = resource.GetHexBytesFromResourceLibrary();
        }

        private void DisplayResource()
        {
            ResourceItem? resourceItem = (ResourceItem?)lvResources.SelectedItem;

            if (resourceItem == null || resources == null)
            {
                return;
            }

            ZResource resource = resources[resourceItem.IndexInLibrary];
            string? fileFormat = resourceItem.FileFormat;
            uint resourceDataSize = resource.GetResourceDataSize();

            if (!resource.IsResourceDataLoaded())
            {
                resourceLibraries[cbResourceLibraries.SelectedIndex].LoadResourceData((uint)resourceItem.IndexInLibrary);
                headerLibrary.LoadResourceReferences((uint)cbResourceLibraries.SelectedIndex, (uint)resourceItem.IndexInLibrary, globalHeaderLibrary);
            }

            byte[] resourceData = resource.GetResourceData();

            if (fileFormat == null)
            {
                return;
            }

            switch (fileFormat)
            {
                case "TEMP":
                    DisplayTemplateEntity(resource);
                    break;
                case "TBLU":
                    DisplayTemplateEntityBlueprint(resourceData, resourceDataSize);
                    break;
                case "CPPT":
                    DisplayCppEntity(resource);
                    break;
                case "CBLU":
                    DisplayCppEntityBlueprint(resourceData, resourceDataSize);
                    break;
                case "AIBB":
                    DisplayBehaviorTreeEntityBlueprint(resourceData, resourceDataSize);
                    break;
                case "TEXT":
                    DisplayTexture(resourceData, resourceDataSize);
                    break;
                case "FSBF":
                    DisplayAudioSamples(resourceData, resourceDataSize);
                    break;
                case "FSBM":
                    DisplayAudioSamples(resourceData, resourceDataSize);
                    break;
                case "FSBS":
                    DisplayAudioSamples(resourceData, resourceDataSize);
                    break;
                case "WAVB":
                    DisplayWaveBankData(resourceData, resourceDataSize);
                    break;
                case "MUCB":
                    DisplayMusicCompositionData(resourceData, resourceDataSize);
                    break;
                case "SWFF":
                    DisplaySWFF(resourceData, resourceDataSize);
                    break;
                case "TELI":
                    DisplayTextList(resourceData, resourceDataSize);
                    break;
                case "PRIM":
                    DisplayModel(resourceData, resourceDataSize);
                    break;
                case "MATI":
                    DisplayMaterialInstance(resourceData, resourceDataSize);
                    break;
                case "MATT":
                    DisplayMaterialEntity(resourceData, resourceDataSize);
                    break;
                case "MATB":
                    DisplayMaterialEntityBlueprint(resourceData, resourceDataSize);
                    break;
                case "ChrT":
                    DisplayChartType(resourceData, resourceDataSize);
                    break;
                case "GIDX":
                    DisplayResourceIndex(resourceData, resourceDataSize);
                    break;
                default:
                    break;
            }
        }

        private async void DisplayTemplateEntity(ZResource resource)
        {
            //if (jsonViewerTabItem.Visibility == Visibility.Collapsed)
            //{
            //    textureViewerTabItem.Visibility = Visibility.Collapsed;
            //    mediaPlayerTabItem.Visibility = Visibility.Collapsed;
            //    modelViewerTabItem.Visibility = Visibility.Collapsed;
            //    materialViewerTabItem.Visibility = Visibility.Collapsed;
            //    textViewerTabItem.Visibility = Visibility.Collapsed;
            //    jsonViewerTabItem.Visibility = Visibility.Visible;
            //}

            //if (!ZEntityTemplate.ConvertTemplateEntityToJSON(resource, out string jsonOutput))
            //{
            //    await this.ShowMessageAsync("HM5 Resource Tool", "Failed to convert resource to JSON.");

            //    return;
            //}

            //textEditor.Text = jsonOutput;
        }

        private async void DisplayTemplateEntityBlueprint(byte[] resourceData, uint resourceDataSize)
        {
            if (jsonViewerTabItem.Visibility == Visibility.Collapsed)
            {
                textureViewerTabItem.Visibility = Visibility.Collapsed;
                mediaPlayerTabItem.Visibility = Visibility.Collapsed;
                modelViewerTabItem.Visibility = Visibility.Collapsed;
                materialViewerTabItem.Visibility = Visibility.Collapsed;
                textViewerTabItem.Visibility = Visibility.Collapsed;
                jsonViewerTabItem.Visibility = Visibility.Visible;
            }

            if (!ZEntityTemplate.ConvertTemplateEntityBlueprintToJSON(resourceData, resourceDataSize, out string jsonOutput))
            {
                await this.ShowMessageAsync("HM5 Resource Tool", "Failed to convert resource to JSON.");

                return;
            }

            textEditor.Text = jsonOutput;
        }

        private async void DisplayCppEntity(ZResource resource)
        {
            if (jsonViewerTabItem.Visibility == Visibility.Collapsed)
            {
                textureViewerTabItem.Visibility = Visibility.Collapsed;
                mediaPlayerTabItem.Visibility = Visibility.Collapsed;
                modelViewerTabItem.Visibility = Visibility.Collapsed;
                materialViewerTabItem.Visibility = Visibility.Collapsed;
                textViewerTabItem.Visibility = Visibility.Collapsed;
                jsonViewerTabItem.Visibility = Visibility.Visible;
            }

            if (ZEntityTemplate.ConvertCppEntityToJSON(resource, out string jsonOutput))
            {
                await this.ShowMessageAsync("HM5 Resource Tool", "Failed to convert resource to JSON.");

                return;
            }

            textEditor.Text = jsonOutput;
        }

        private async void DisplayCppEntityBlueprint(byte[] resourceData, uint resourceDataSize)
        {
            if (jsonViewerTabItem.Visibility == Visibility.Collapsed)
            {
                textureViewerTabItem.Visibility = Visibility.Collapsed;
                mediaPlayerTabItem.Visibility = Visibility.Collapsed;
                modelViewerTabItem.Visibility = Visibility.Collapsed;
                materialViewerTabItem.Visibility = Visibility.Collapsed;
                textViewerTabItem.Visibility = Visibility.Collapsed;
                jsonViewerTabItem.Visibility = Visibility.Visible;
            }

            if (!ZEntityTemplate.ConvertCppEntityBlueprintToJSON(resourceData, resourceDataSize, out string jsonOutput))
            {
                await this.ShowMessageAsync("HM5 Resource Tool", "Failed to convert resource to JSON.");

                return;
            }

            textEditor.Text = jsonOutput;
        }

        private async void DisplayBehaviorTreeEntityBlueprint(byte[] resourceData, uint resourceDataSize)
        {
            if (jsonViewerTabItem.Visibility == Visibility.Collapsed)
            {
                textureViewerTabItem.Visibility = Visibility.Collapsed;
                mediaPlayerTabItem.Visibility = Visibility.Collapsed;
                modelViewerTabItem.Visibility = Visibility.Collapsed;
                materialViewerTabItem.Visibility = Visibility.Collapsed;
                textViewerTabItem.Visibility = Visibility.Collapsed;
                jsonViewerTabItem.Visibility = Visibility.Visible;
            }

            if (!ZBehaviorTree.ConvertBehaviorTreeEntityBlueprintToJSON(resourceData, resourceDataSize, out string jsonOutput))
            {
                await this.ShowMessageAsync("HM5 Resource Tool", "Failed to convert resource to JSON.");

                return;
            }

            textEditor.Text = jsonOutput;
        }

        private async void DisplayTexture(byte[] resourceData, uint resourceDataSize)
        {
            if (textureViewerTabItem.Visibility == Visibility.Collapsed)
            {
                jsonViewerTabItem.Visibility = Visibility.Collapsed;
                mediaPlayerTabItem.Visibility = Visibility.Collapsed;
                modelViewerTabItem.Visibility = Visibility.Collapsed;
                materialViewerTabItem.Visibility = Visibility.Collapsed;
                textViewerTabItem.Visibility = Visibility.Collapsed;
                textureViewerTabItem.Visibility = Visibility.Visible;
            }

            if (!ZTexture.ConvertTextureMapToBMP(resourceData, resourceDataSize, out byte[] bmpData, out uint bmpDataSize))
            {
                await this.ShowMessageAsync("HM5 Resource Tool", "Failed to convert resource to BMP.");

                return;
            }

            BitmapSource? bitmap = (BitmapSource?)new ImageSourceConverter().ConvertFrom(bmpData);

            if (bitmap == null)
            {
                return;
            }

            double bitmapWidth = bitmap.Width;
            double bitmapHeight = bitmap.Height;
            double tabControlWidth = tabControl.ActualWidth;
            double tabControlHeight = tabControl.ActualHeight;

            while (bitmapWidth > tabControlWidth && bitmapHeight > tabControlHeight)
            {
                bitmapWidth /= 2;
                bitmapHeight /= 2;
            }

            image.Source = bitmap;
            image.Width = bitmapWidth;
            image.Height = bitmapHeight;
        }

        private async void DisplayAudioSamples(byte[] resourceData, uint resourceDataSize)
        {
            if (mediaPlayerTabItem.Visibility == Visibility.Collapsed)
            {
                jsonViewerTabItem.Visibility = Visibility.Collapsed;
                textureViewerTabItem.Visibility = Visibility.Collapsed;
                modelViewerTabItem.Visibility = Visibility.Collapsed;
                materialViewerTabItem.Visibility = Visibility.Collapsed;
                textViewerTabItem.Visibility = Visibility.Collapsed;
                mediaPlayerTabItem.Visibility = Visibility.Visible;
            }

            if (ZAudio.IsOGGFormat(resourceData, resourceDataSize, 16))
            {
                if (!ZAudio.ConvertFSBFToOGG(resourceData, resourceDataSize, out audioSamples))
                {
                    await this.ShowMessageAsync("HM5 Resource Tool", "Failed to convert resource to OGG.");

                    return;
                }
            }

            if (audioSamples == null)
            {
                return;
            }

            lbAudioSamples.Items.Clear();

            int count = audioSamples.Count;

            for (int i = 0; i < count; ++i)
            {
                lbAudioSamples.Items.Add(audioSamples[i].GetName());
            }
        }

        private async void DisplayWaveBankData(byte[] resourceData, uint resourceDataSize)
        {
            if (jsonViewerTabItem.Visibility == Visibility.Collapsed)
            {
                textureViewerTabItem.Visibility = Visibility.Collapsed;
                mediaPlayerTabItem.Visibility = Visibility.Collapsed;
                modelViewerTabItem.Visibility = Visibility.Collapsed;
                materialViewerTabItem.Visibility = Visibility.Collapsed;
                textViewerTabItem.Visibility = Visibility.Collapsed;
                jsonViewerTabItem.Visibility = Visibility.Visible;
            }

            if (!ZAudio.ConvertWAVBToJSON(resourceData, resourceDataSize, out string jsonOutput))
            {
                await this.ShowMessageAsync("HM5 Resource Tool", "Failed to convert resource to JSON.");

                return;
            }

            textEditor.Text = jsonOutput;
        }

        private async void DisplayMusicCompositionData(byte[] resourceData, uint resourceDataSize)
        {
            if (jsonViewerTabItem.Visibility == Visibility.Collapsed)
            {
                textureViewerTabItem.Visibility = Visibility.Collapsed;
                mediaPlayerTabItem.Visibility = Visibility.Collapsed;
                modelViewerTabItem.Visibility = Visibility.Collapsed;
                materialViewerTabItem.Visibility = Visibility.Collapsed;
                textViewerTabItem.Visibility = Visibility.Collapsed;
                jsonViewerTabItem.Visibility = Visibility.Visible;
            }

            if (!ZAudio.ConvertMUCBToJSON(resourceData, resourceDataSize, out string jsonOutput))
            {
                await this.ShowMessageAsync("HM5 Resource Tool", "Failed to convert resource to JSON.");

                return;
            }

            textEditor.Text = jsonOutput;
        }

        private async void DisplaySWFF(byte[] resourceData, uint resourceDataSize)
        {
            if (textureViewerTabItem.Visibility == Visibility.Collapsed)
            {
                jsonViewerTabItem.Visibility = Visibility.Collapsed;
                mediaPlayerTabItem.Visibility = Visibility.Collapsed;
                modelViewerTabItem.Visibility = Visibility.Collapsed;
                materialViewerTabItem.Visibility = Visibility.Collapsed;
                textViewerTabItem.Visibility = Visibility.Collapsed;
                textureViewerTabItem.Visibility = Visibility.Visible;
            }

            string outputFilePath = string.Format(@"{0}\SWFF.png", System.IO.Path.GetDirectoryName(Assembly.GetExecutingAssembly().GetName().CodeBase));

            if (!ZResourceExporter.ExportSWFF(resourceData, resourceDataSize, outputFilePath))
            {
                await this.ShowMessageAsync("HM5 Resource Tool", "Failed to convert resource to PNG.");

                return;
            }

            byte[] bmpData = File.ReadAllBytes(outputFilePath);
            BitmapSource? bitmap = (BitmapSource?)new ImageSourceConverter().ConvertFrom(bmpData);

            if (bitmap == null)
            {
                return;
            }

            image.Source = bitmap;
            image.Width = bitmap.Width;
            image.Height = bitmap.Height;

            File.Delete(outputFilePath);
        }

        private async void DisplayTextList(byte[] resourceData, uint resourceDataSize)
        {
            if (textViewerTabItem.Visibility == Visibility.Collapsed)
            {
                jsonViewerTabItem.Visibility = Visibility.Collapsed;
                textureViewerTabItem.Visibility = Visibility.Collapsed;
                mediaPlayerTabItem.Visibility = Visibility.Collapsed;
                modelViewerTabItem.Visibility = Visibility.Collapsed;
                materialViewerTabItem.Visibility = Visibility.Collapsed;
                textViewerTabItem.Visibility = Visibility.Visible;
            }

            if (!ZTextList.ConvertTextListToText(resourceData, resourceDataSize, out string output))
            {
                await this.ShowMessageAsync("HM5 Resource Tool", "Failed to convert resource.");

                return;
            }

            txtTextList.Text = output;
        }

        private async void DisplayModel(byte[] resourceData, uint resourceDataSize)
        {
            if (modelViewerTabItem.Visibility == Visibility.Collapsed)
            {
                jsonViewerTabItem.Visibility = Visibility.Collapsed;
                textureViewerTabItem.Visibility = Visibility.Collapsed;
                mediaPlayerTabItem.Visibility = Visibility.Collapsed;
                materialViewerTabItem.Visibility = Visibility.Collapsed;
                textViewerTabItem.Visibility = Visibility.Collapsed;
                modelViewerTabItem.Visibility = Visibility.Visible;
            }

            Assembly? assembly = Assembly.GetExecutingAssembly();

            if (assembly == null)
            {
                return;
            }

            string objFilePath = string.Format(@"{0}\Model.obj", System.IO.Path.GetDirectoryName(assembly.Location));

            if (!ZModel.ConvertRenderPrimitiveToOBJ(resourceData, resourceDataSize, objFilePath))
            {
                await this.ShowMessageAsync("HM5 Resource Tool", "Failed to convert resource to OBJ.");

                return;
            }

            ModelImporter modelImporter = new ModelImporter();
            Model3DGroup model3DGroup = modelImporter.Load(objFilePath);
            Material material = MaterialHelper.CreateMaterial(new SolidColorBrush(Color.FromRgb(200, 200, 200)));
            
            foreach (GeometryModel3D geometryModel in model3DGroup.Children)
            {
                geometryModel.Material = material;
                geometryModel.BackMaterial = material;
            }

            model.Content = model3DGroup;

            CameraHelper.ZoomExtents(helixViewport.Camera, helixViewport.Viewport, 1000);

            File.Delete(objFilePath);
        }

        private void DisplayMaterialInstance(byte[] resourceData, uint resourceDataSize)
        {
            if (materialViewerTabItem.Visibility == Visibility.Collapsed)
            {
                jsonViewerTabItem.Visibility = Visibility.Collapsed;
                textureViewerTabItem.Visibility = Visibility.Collapsed;
                mediaPlayerTabItem.Visibility = Visibility.Collapsed;
                modelViewerTabItem.Visibility = Visibility.Collapsed;
                textViewerTabItem.Visibility = Visibility.Collapsed;
                materialViewerTabItem.Visibility = Visibility.Visible;
            }

            ZRenderMaterialInstance renderMaterialInstance = new ZRenderMaterialInstance();

            renderMaterialInstance.Deserialize(resourceData, resourceDataSize);

            ZRenderMaterialInstance.ZProperty instanceProperty = renderMaterialInstance.GetInstanceProperty();
            MaterialProperty materialProperty = new MaterialProperty();

            AddProperties(instanceProperty, materialProperty);

            MaterialPropertyModel materialPropertyModel = new MaterialPropertyModel();

            if (materialPropertyModel.MaterialProperties != null)
            {
                materialPropertyModel.MaterialProperties.Add(materialProperty);
            }

            DataContext = materialPropertyModel;
        }

        private async void DisplayMaterialEntity(byte[] resourceData, uint resourceDataSize)
        {
            if (jsonViewerTabItem.Visibility == Visibility.Collapsed)
            {
                textureViewerTabItem.Visibility = Visibility.Collapsed;
                mediaPlayerTabItem.Visibility = Visibility.Collapsed;
                modelViewerTabItem.Visibility = Visibility.Collapsed;
                materialViewerTabItem.Visibility = Visibility.Collapsed;
                textViewerTabItem.Visibility = Visibility.Collapsed;
                jsonViewerTabItem.Visibility = Visibility.Visible;
            }

            if (!ZMaterial.ConvertRenderMaterialEntityToJSON(resourceData, resourceDataSize, out string jsonOutput))
            {
                await this.ShowMessageAsync("HM5 Resource Tool", "Failed to convert resource to JSON.");

                return;
            }

            textEditor.Text = jsonOutput;
        }

        private async void DisplayMaterialEntityBlueprint(byte[] resourceData, uint resourceDataSize)
        {
            if (jsonViewerTabItem.Visibility == Visibility.Collapsed)
            {
                textureViewerTabItem.Visibility = Visibility.Collapsed;
                mediaPlayerTabItem.Visibility = Visibility.Collapsed;
                modelViewerTabItem.Visibility = Visibility.Collapsed;
                materialViewerTabItem.Visibility = Visibility.Collapsed;
                textViewerTabItem.Visibility = Visibility.Collapsed;
                jsonViewerTabItem.Visibility = Visibility.Visible;
            }

            if (!ZMaterial.ConvertRenderMaterialEntityBlueprintToJSON(resourceData, resourceDataSize, out string jsonOutput))
            {
                await this.ShowMessageAsync("HM5 Resource Tool", "Failed to convert resource to JSON.");

                return;
            }

            textEditor.Text = jsonOutput;
        }

        private async void DisplayChartType(byte[] resourceData, uint resourceDataSize)
        {
            if (textViewerTabItem.Visibility == Visibility.Collapsed)
            {
                jsonViewerTabItem.Visibility = Visibility.Collapsed;
                textureViewerTabItem.Visibility = Visibility.Collapsed;
                mediaPlayerTabItem.Visibility = Visibility.Collapsed;
                modelViewerTabItem.Visibility = Visibility.Collapsed;
                materialViewerTabItem.Visibility = Visibility.Collapsed;
                textViewerTabItem.Visibility = Visibility.Visible;
            }

            if (!ZChartType.ConvertChartTypeToText(resourceData, resourceDataSize, out string output))
            {
                await this.ShowMessageAsync("HM5 Resource Tool", "Failed to convert resource.");

                return;
            }

            txtTextList.Text = output;
        }

        private async void DisplayResourceIndex(byte[] resourceData, uint resourceDataSize)
        {
            if (jsonViewerTabItem.Visibility == Visibility.Collapsed)
            {
                textureViewerTabItem.Visibility = Visibility.Collapsed;
                mediaPlayerTabItem.Visibility = Visibility.Collapsed;
                modelViewerTabItem.Visibility = Visibility.Collapsed;
                materialViewerTabItem.Visibility = Visibility.Collapsed;
                textViewerTabItem.Visibility = Visibility.Collapsed;
                jsonViewerTabItem.Visibility = Visibility.Visible;
            }

            if (!ZResourceIndex.ConvertResourceIndexToJSON(resourceData, resourceDataSize, out string jsonOutput))
            {
                await this.ShowMessageAsync("HM5 Resource Tool", "Failed to convert resource to JSON.");

                return;
            }

            textEditor.Text = jsonOutput;
        }

        private void AddProperties(ZRenderMaterialInstance.ZProperty property, MaterialProperty materialProperty)
        {
            ZRenderMaterialInstance.SProperty propertyInfo = property.GetPropertyInfo();
            List<ZRenderMaterialInstance.ZProperty> childProperties = property.GetChildProperties();
            int count = childProperties.Count;
            PropertyType propertyType = (PropertyType)propertyInfo.GetType();
            string propertyName = propertyInfo.GetName();

            materialProperty.Name = string.Format("{0} ({1})", propertyName, materialPropertyNames[propertyName]);
            materialProperty.Type = ConvertPropertyTypeToString(propertyType);

            switch (propertyType)
            {
                case PropertyType.Float:
                    materialProperty.Value = ConvertByteArrayToFloat(property.GetValue()).ToString();
                    break;
                case PropertyType.Char:
                    materialProperty.Value = Encoding.Default.GetString(property.GetValue());
                    break;
                case PropertyType.UInt32:
                    materialProperty.Value = BitConverter.ToUInt32(property.GetValue(), 0).ToString();
                    break;
                default:
                    break;
            }

            materialProperty.Children = new List<MaterialProperty>(count);

            for (int i = 0; i < count; ++i)
            {
                MaterialProperty materialProperty2 = new MaterialProperty();

                AddProperties(childProperties[i], materialProperty2);

                materialProperty.Children.Add(materialProperty2);
            }
        }

        private static string ConvertPropertyTypeToString(PropertyType propertyType)
        {
            string result = "";

            switch (propertyType)
            {
                case PropertyType.Float:
                    result = "Float";
                    break;
                case PropertyType.Char:
                    result = "Char";
                    break;
                case PropertyType.UInt32:
                    result = "UInt32";
                    break;
                case PropertyType.List:
                    result = "List";
                    break;
            }

            return result;
        }

        private static float ConvertByteArrayToFloat(byte[] data)
        {
            byte tmp = data[0];

            data[0] = data[3];
            data[3] = tmp;
            tmp = data[1];
            data[1] = data[2];
            data[2] = tmp;

            return BitConverter.ToSingle(data, 0);
        }

        private void BtnTogglePlay_Click(object sender, RoutedEventArgs e)
        {
            if (mediaPlayer == null)
            {
                return;
            }

            if (btnTogglePlay.Content.Equals("Play"))
            {
                if (mediaPlayer.Length == -1)
                {
                    int index = lbAudioSamples.SelectedIndex;

                    if (audioSamples != null && index >= 0 && libVLC != null)
                    {
                        memoryStream = new MemoryStream(audioSamples[index].GetData());
                        mediaInput = new LibVLCSharp.Shared.StreamMediaInput(memoryStream);
                        mediaPlayer.Media = new LibVLCSharp.Shared.Media(libVLC, mediaInput);
                    }
                }

                mediaPlayer.Play();

                btnTogglePlay.Content = "Pause";
            }
            else
            {
                mediaPlayer.Pause();

                btnTogglePlay.Content = "Play";
            }
        }

        private void MediaPlayer_LengthChanged(object? sender, LibVLCSharp.Shared.MediaPlayerLengthChangedEventArgs e)
        {
            if (mediaPlayer == null)
            {
                return;
            }

            TimeSpan timeSpan = TimeSpan.FromMilliseconds(mediaPlayer.Length);
            string duration = string.Format("{0:D2}:{1:D2}:{2:D2}", timeSpan.Hours, timeSpan.Minutes, timeSpan.Seconds);

            lblDuration.Dispatcher.Invoke(() =>
            {
                lblDuration.Content = duration;
            });

            progressBar.Dispatcher.Invoke(() =>
            {
                progressBar.Minimum = 0;
                progressBar.Maximum = mediaPlayer.Length;
            });
        }

        private void MediaPlayer_TimeChanged(object? sender, LibVLCSharp.Shared.MediaPlayerTimeChangedEventArgs e)
        {
            TimeSpan timeSpan = TimeSpan.FromMilliseconds(e.Time);
            string time = string.Format("{0:D2}:{1:D2}:{2:D2}", timeSpan.Hours, timeSpan.Minutes, timeSpan.Seconds);

            lblTime.Dispatcher.Invoke(() =>
            {
                lblTime.Content = time;
            });

            progressBar.Dispatcher.Invoke(() =>
            {
                progressBar.Value = e.Time;
            });
        }

        private void ProgressBar_MouseLeftButtonUp(object sender, MouseButtonEventArgs e)
        {
            if (mediaPlayer == null)
            {
                return;
            }

            double mousePosition = e.GetPosition(progressBar).X;
            double ratio = mousePosition / progressBar.ActualWidth;
            TimeSpan timeSpan = TimeSpan.FromMilliseconds(ratio * progressBar.Maximum);

            mediaPlayer.SeekTo(timeSpan);
        }

        private void MediaPlayer_EndReached(object? sender, EventArgs e)
        {
            if (mediaPlayer == null)
            {
                return;
            }

            ThreadPool.QueueUserWorkItem(state => mediaPlayer.Stop());

            lblTime.Dispatcher.Invoke(() =>
            {
                lblTime.Content = "00:00:00";
            });

            progressBar.Dispatcher.Invoke(() =>
            {
                progressBar.Value = 0;
            });

            btnTogglePlay.Dispatcher.Invoke(() =>
            {
                btnTogglePlay.Content = "Play";
            });
        }

        private void LoadJSONSyntaxHighlighter()
        {
            using (FileStream fileStream = new FileStream("Json.xshd", FileMode.Open, FileAccess.Read))
            {
                using (XmlTextReader reader = new XmlTextReader(fileStream))
                {
                    textEditor.SyntaxHighlighting = HighlightingLoader.Load(reader, HighlightingManager.Instance);
                }
            }
        }

        private void SetupVLCPlayer()
        {
            LibVLCSharp.Shared.Core.Initialize();

            libVLC = new LibVLCSharp.Shared.LibVLC();
            mediaPlayer = new LibVLCSharp.Shared.MediaPlayer(libVLC);

            mediaPlayer.LengthChanged += MediaPlayer_LengthChanged;
            mediaPlayer.TimeChanged += MediaPlayer_TimeChanged;
            mediaPlayer.EndReached += MediaPlayer_EndReached;
        }

        private void ScrollViewer_PreviewMouseWheel(object sender, MouseWheelEventArgs e)
        {
            ScrollViewer scrollViewer = (ScrollViewer)sender;

            scrollViewer.ScrollToVerticalOffset(scrollViewer.VerticalOffset - e.Delta);

            e.Handled = true;
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

        private void ContextMenu_ContextMenuOpening(object sender, ContextMenuEventArgs e)
        {
            if (lvResources.SelectedItems.Count == 0)
            {
                return;
            }

            ResourceItem? resourceItem = (ResourceItem?)lvResources.SelectedItems[0];

            if (resourceItem != null && resourceItem.FileFormat != null)
            {
                if (resourceItem.FileFormat.Equals("TEMP"))
                {
                    displayTEMPAndTBLUMenuItem.Visibility = Visibility.Visible;
                    displayMATTAndMATBMenuItem.Visibility = Visibility.Collapsed;
                }
                else if (resourceItem.FileFormat.Equals("MATT"))
                {
                    displayTEMPAndTBLUMenuItem.Visibility = Visibility.Collapsed;
                    displayMATTAndMATBMenuItem.Visibility = Visibility.Visible;
                }
                else
                {
                    displayTEMPAndTBLUMenuItem.Visibility = Visibility.Collapsed;
                    displayMATTAndMATBMenuItem.Visibility = Visibility.Collapsed;
                }
            }
        }

        private async void ExportCurrentResourceLibraryMenuItem_Click(object sender, RoutedEventArgs e)
        {
            string? resourceID = cbResourceLibraries.SelectedItem as string;

            if (resourceID != null)
            {
                string json = File.ReadAllText("HM5ResourceTool.json");
                Settings? settings = JsonConvert.DeserializeObject<Settings>(json);

                if (settings == null || settings.GamePath == null)
                {
                    return;
                }

                if (DisplayFileDialog(out string exportFolderPath, out uint filterIndex, true))
                {
                    if (globalHeaderLibrary == null)
                    {
                        string globalHeaderLibraryPath = string.Format("{0}/runtime/common/globalresources.ini/3133BD95C91CCB418964E1B8BA3BB23D.pc_headerlib", settings.GamePath);

                        globalHeaderLibrary = new ZHeaderLibrary(globalHeaderLibraryPath);

                        globalHeaderLibrary.ParseHeaderLibrary(settings.GamePath, null, true);
                    }

                    headerLibrary.ExtractResourceLibrary(settings.GamePath, globalHeaderLibrary, exportFolderPath, resourceID);

                    await this.ShowMessageAsync("HM5 Resource Tool", "Resources successfully exported.");
                }
            }
        }

        private void DisplayTEMPAndTBLUMenuItem_Click(object sender, RoutedEventArgs e)
        {
            ResourceItem? resourceItem = (ResourceItem?)lvResources.SelectedItem;

            if (resourceItem == null || resources == null)
            {
                return;
            }

            ZResource resource = resources[resourceItem.IndexInLibrary];

            TemplateEntityEditor templateEntityEditor = new TemplateEntityEditor(resource, headerLibrary, globalHeaderLibrary, settings);

            templateEntityEditor.ShowDialog();

            //SEntityTemplate entityTemplate = ZEntityTemplate.JoinTEMPAndTBLU(resource, headerLibrary, globalHeaderLibrary);

            //if (!ZEntityTemplate.ConvertTEMPAndTBLUToJSON(resource, out string jsonOutput))
            //{
            //    await this.ShowMessageAsync("HM5 Resource Tool", "Failed to convert TEMP and TBLU to JSON.");

            //    return;
            //}

            //textEditor.Text = jsonOutput;
        }

        private async void DisplayMATTAndMATBMenuItem_Click(object sender, RoutedEventArgs e)
        {
            ResourceItem? resourceItem = (ResourceItem?)lvResources.SelectedItem;

            if (resourceItem == null || resources == null)
            {
                return;
            }

            ZResource resource = resources[resourceItem.IndexInLibrary];

            if (!ZMaterial.ConvertRenderMaterialEntityAndBlueprintToJSON(resource, out string jsonOutput))
            {
                await this.ShowMessageAsync("HM5 Resource Tool", "Failed to convert MATT and MATB to JSON.");

                return;
            }

            textEditor.Text = jsonOutput;
        }

        private void TabControl_SelectionChanged(object sender, SelectionChangedEventArgs e)
        {
            switch (tabControl.SelectedIndex)
            {
                case 1:
                    DisplayHexBytesFromHeaderLibrary();
                    break;
                case 2:
                    DisplayHexBytesFromResourceLibrary();
                    break;
            }
        }

        private void ExportResourceMenuItem_Click(object sender, RoutedEventArgs e)
        {
            ResourceItem? resourceItem = (ResourceItem?)lvResources.SelectedItem;

            if (resourceItem == null || resources == null)
            {
                return;
            }

            ZResource resource = resources[resourceItem.IndexInLibrary];
            int indexInLibrary = resourceItem.IndexInLibrary;
            string? fileFormat = resourceItem.FileFormat;
            uint resourceDataSize = resource.GetResourceDataSize();

            if (!resource.IsResourceDataLoaded())
            {
                resourceLibraries[cbResourceLibraries.SelectedIndex].LoadResourceData((uint)resourceItem.IndexInLibrary);
                headerLibrary.LoadResourceReferences((uint)cbResourceLibraries.SelectedIndex, (uint)resourceItem.IndexInLibrary, globalHeaderLibrary);
            }

            byte[] resourceData = resource.GetResourceData();

            if (fileFormat == null)
            {
                return;
            }

            if (!DisplayFileDialog(out string outputFilePath, out uint filterIndex, true))
            {
                return;
            }

            switch (fileFormat)
            {
                case "TEMP":
                    ExportTemplateEntity(resource, indexInLibrary, outputFilePath);
                    break;
                case "TBLU":
                    ExportTemplateEntityBlueprint(resourceData, resourceDataSize, indexInLibrary, outputFilePath);
                    break;
                case "CPPT":
                    ExportCppEntity(resource, indexInLibrary, outputFilePath);
                    break;
                case "CBLU":
                    ExportCppEntityBlueprint(resourceData, resourceDataSize, indexInLibrary, outputFilePath);
                    break;
                case "AIBB":
                    ExportBehaviorTreeEntityBlueprint(resourceData, resourceDataSize, indexInLibrary, outputFilePath);
                    break;
                case "TEXT":
                    ExportTexture(resourceData, resourceDataSize, indexInLibrary, outputFilePath);
                    break;
                case "FSBF":
                    ExportFSBF(resourceData, resourceDataSize, indexInLibrary, outputFilePath);
                    break;
                case "FSBM":
                    ExportFSBM(resourceData, resourceDataSize, indexInLibrary, outputFilePath);
                    break;
                case "FSBS":
                    ExportFSBS(resourceData, resourceDataSize, indexInLibrary, outputFilePath);
                    break;
                case "WAVB":
                    ExportWaveBankData(resourceData, resourceDataSize, indexInLibrary, outputFilePath);
                    break;
                case "MUCB":
                    ExportMusicCompositionData(resourceData, resourceDataSize, indexInLibrary, outputFilePath);
                    break;
                case "SWFF":
                    ExportSWFF(resourceData, resourceDataSize, indexInLibrary, outputFilePath);
                    break;
                case "GFXF":
                    ExportGFXF(resourceData, resourceDataSize, indexInLibrary, outputFilePath);
                    break;
                case "TELI":
                    ExportTextList(resourceData, resourceDataSize, indexInLibrary, outputFilePath);
                    break;
                case "PRIM":
                    ExportModel(resourceData, resourceDataSize, indexInLibrary, outputFilePath);
                    break;
                case "MATI":
                    ExportMaterialInstance(resourceData, resourceDataSize, indexInLibrary, outputFilePath);
                    break;
                case "MATT":
                    ExportMaterialEntity(resourceData, resourceDataSize, indexInLibrary, outputFilePath);
                    break;
                case "MATB":
                    ExportMaterialEntityBlueprint(resourceData, resourceDataSize, indexInLibrary, outputFilePath);
                    break;
                default:
                    break;
            }

            outputFilePath = string.Format(@"{0}\{1}.meta.json", outputFilePath, indexInLibrary);

            File.WriteAllText(outputFilePath, resource.SerializeToJSON());
        }

        private async void ExportTemplateEntity(ZResource resource, int indexInLibrary, string outputFilePath)
        {
            outputFilePath = string.Format(@"{0}\{1}.json", outputFilePath, indexInLibrary);

            if (ZResourceExporter.ExportTEMP(resource, outputFilePath))
            {
                await this.ShowMessageAsync("HM5 Resource Tool", "TEMP successfully exported.");
            }
            else
            {
                await this.ShowMessageAsync("HM5 Resource Tool", "Failed to export TEMP.");
            }
        }

        private async void ExportTemplateEntityBlueprint(byte[] resourceData, uint resourceDataSize, int indexInLibrary, string outputFilePath)
        {
            outputFilePath = string.Format(@"{0}\{1}.json", outputFilePath, indexInLibrary);

            if (ZResourceExporter.ExportTBLU(resourceData, resourceDataSize, outputFilePath))
            {
                await this.ShowMessageAsync("HM5 Resource Tool", "TBLU successfully exported.");
            }
            else
            {
                await this.ShowMessageAsync("HM5 Resource Tool", "Failed to export TBLU.");
            }
        }

        private async void ExportCppEntity(ZResource resource, int indexInLibrary, string outputFilePath)
        {
            outputFilePath = string.Format(@"{0}\{1}.json", outputFilePath, indexInLibrary);

            if (ZResourceExporter.ExportCPPT(resource, outputFilePath))
            {
                await this.ShowMessageAsync("HM5 Resource Tool", "CPPT successfully exported.");
            }
            else
            {
                await this.ShowMessageAsync("HM5 Resource Tool", "Failed to export CPPT.");
            }
        }

        private async void ExportCppEntityBlueprint(byte[] resourceData, uint resourceDataSize, int indexInLibrary, string outputFilePath)
        {
            outputFilePath = string.Format(@"{0}\{1}.json", outputFilePath, indexInLibrary);

            if (ZResourceExporter.ExportCBLU(resourceData, resourceDataSize, outputFilePath))
            {
                await this.ShowMessageAsync("HM5 Resource Tool", "CBLU successfully exported.");
            }
            else
            {
                await this.ShowMessageAsync("HM5 Resource Tool", "Failed to export CBLU.");
            }
        }

        private async void ExportBehaviorTreeEntityBlueprint(byte[] resourceData, uint resourceDataSize, int indexInLibrary, string outputFilePath)
        {
            outputFilePath = string.Format(@"{0}\{1}.json", outputFilePath, indexInLibrary);

            if (ZResourceExporter.ExportAIBB(resourceData, resourceDataSize, outputFilePath))
            {
                await this.ShowMessageAsync("HM5 Resource Tool", "AIBB successfully exported.");
            }
            else
            {
                await this.ShowMessageAsync("HM5 Resource Tool", "Failed to export AIBB.");
            }
        }

        private async void ExportTexture(byte[] resourceData, uint resourceDataSize, int indexInLibrary, string outputFilePath)
        {
            List<string> extensions = new List<string>();

            extensions.Add("DDS");
            extensions.Add("TGA");
            extensions.Add("BMP");
            extensions.Add("PNG");

            ExportDialog exportDialog = new ExportDialog(extensions);

            if (exportDialog.ShowDialog() ?? true)
            {
                outputFilePath = string.Format(@"{0}\{1}{2}.", outputFilePath, indexInLibrary, extensions[(int)exportDialog.Option]);

                if (ZResourceExporter.ExportTEXT(resourceData, resourceDataSize, outputFilePath, exportDialog.Option))
                {
                    await this.ShowMessageAsync("HM5 Resource Tool", "TEXT successfully exported.");
                }
                else
                {
                    await this.ShowMessageAsync("HM5 Resource Tool", "Failed to export TEXT.");
                }
            }
        }

        private async void ExportFSBF(byte[] resourceData, uint resourceDataSize, int indexInLibrary, string outputFilePath)
        {
            outputFilePath = string.Format(@"{0}\{1}", outputFilePath, indexInLibrary);

            if (ZResourceExporter.ExportFSBF(resourceData, resourceDataSize, outputFilePath))
            {
                await this.ShowMessageAsync("HM5 Resource Tool", "FSBF successfully exported.");
            }
            else
            {
                await this.ShowMessageAsync("HM5 Resource Tool", "Failed to export FSBF.");
            }
        }

        private async void ExportFSBM(byte[] resourceData, uint resourceDataSize, int indexInLibrary, string outputFilePath)
        {
            outputFilePath = string.Format(@"{0}\{1}", outputFilePath, indexInLibrary);

            if (ZResourceExporter.ExportFSBM(resourceData, resourceDataSize, outputFilePath))
            {
                await this.ShowMessageAsync("HM5 Resource Tool", "FSBM successfully exported.");
            }
            else
            {
                await this.ShowMessageAsync("HM5 Resource Tool", "Failed to export FSBM.");
            }
        }

        private async void ExportFSBS(byte[] resourceData, uint resourceDataSize, int indexInLibrary, string outputFilePath)
        {
            outputFilePath = string.Format(@"{0}\{1}", outputFilePath, indexInLibrary);

            if (ZResourceExporter.ExportFSBS(resourceData, resourceDataSize, outputFilePath))
            {
                await this.ShowMessageAsync("HM5 Resource Tool", "FSBS successfully exported.");
            }
            else
            {
                await this.ShowMessageAsync("HM5 Resource Tool", "Failed to export FSBS.");
            }
        }

        private async void ExportWaveBankData(byte[] resourceData, uint resourceDataSize, int indexInLibrary, string outputFilePath)
        {
            outputFilePath = string.Format(@"{0}\{1}.json", outputFilePath, indexInLibrary);

            if (ZResourceExporter.ExportWAVB(resourceData, resourceDataSize, outputFilePath))
            {
                await this.ShowMessageAsync("HM5 Resource Tool", "WAVB successfully exported.");
            }
            else
            {
                await this.ShowMessageAsync("HM5 Resource Tool", "Failed to export WAVB.");
            }
        }

        private async void ExportMusicCompositionData(byte[] resourceData, uint resourceDataSize, int indexInLibrary, string outputFilePath)
        {
            outputFilePath = string.Format(@"{0}\{1}.json", outputFilePath, indexInLibrary);

            if (ZResourceExporter.ExportMUCB(resourceData, resourceDataSize, outputFilePath))
            {
                await this.ShowMessageAsync("HM5 Resource Tool", "MUCB successfully exported.");
            }
            else
            {
                await this.ShowMessageAsync("HM5 Resource Tool", "Failed to export MUCB.");
            }
        }

        private async void ExportSWFF(byte[] resourceData, uint resourceDataSize, int indexInLibrary, string outputFilePath)
        {
            outputFilePath = string.Format(@"{0}\{1}", outputFilePath, indexInLibrary);

            if (ZResourceExporter.ExportSWFF(resourceData, resourceDataSize, outputFilePath))
            {
                await this.ShowMessageAsync("HM5 Resource Tool", "SWFF successfully exported.");
            }
            else
            {
                await this.ShowMessageAsync("HM5 Resource Tool", "Failed to export SWFF.");
            }
        }

        private async void ExportGFXF(byte[] resourceData, uint resourceDataSize, int indexInLibrary, string outputFilePath)
        {
            outputFilePath = string.Format(@"{0}\{1}.gfx", outputFilePath, indexInLibrary);

            if (ZResourceExporter.ExportGFXF(resourceData, resourceDataSize, outputFilePath))
            {
                await this.ShowMessageAsync("HM5 Resource Tool", "GFXF successfully exported.");
            }
            else
            {
                await this.ShowMessageAsync("HM5 Resource Tool", "Failed to export GFXF.");
            }
        }

        private async void ExportTextList(byte[] resourceData, uint resourceDataSize, int indexInLibrary, string outputFilePath)
        {
            outputFilePath = string.Format(@"{0}\{1}", outputFilePath, indexInLibrary);

            if (ZResourceExporter.ExportTELI(resourceData, resourceDataSize, outputFilePath))
            {
                await this.ShowMessageAsync("HM5 Resource Tool", "TELI successfully exported.");
            }
            else
            {
                await this.ShowMessageAsync("HM5 Resource Tool", "Failed to export TELI.");
            }
        }

        private async void ExportModel(byte[] resourceData, uint resourceDataSize, int indexInLibrary, string outputFilePath)
        {
            outputFilePath = string.Format(@"{0}\{1}.gltf", outputFilePath, indexInLibrary);

            if (ZResourceExporter.ExportPRIM(resourceData, resourceDataSize, outputFilePath))
            {
                await this.ShowMessageAsync("HM5 Resource Tool", "PRIM successfully exported.");
            }
            else
            {
                await this.ShowMessageAsync("HM5 Resource Tool", "Failed to export PRIM.");
            }
        }

        private async void ExportMaterialInstance(byte[] resourceData, uint resourceDataSize, int indexInLibrary, string outputFilePath)
        {
            outputFilePath = string.Format(@"{0}\{1}.json", outputFilePath, indexInLibrary);

            if (ZResourceExporter.ExportMATI(resourceData, resourceDataSize, outputFilePath))
            {
                await this.ShowMessageAsync("HM5 Resource Tool", "MATI successfully exported.");
            }
            else
            {
                await this.ShowMessageAsync("HM5 Resource Tool", "Failed to export MATI.");
            }
        }

        private async void ExportMaterialEntity(byte[] resourceData, uint resourceDataSize, int indexInLibrary, string outputFilePath)
        {
            outputFilePath = string.Format(@"{0}\{1}.json", outputFilePath, indexInLibrary);

            if (ZResourceExporter.ExportMATT(resourceData, resourceDataSize, outputFilePath))
            {
                await this.ShowMessageAsync("HM5 Resource Tool", "MATT successfully exported.");
            }
            else
            {
                await this.ShowMessageAsync("HM5 Resource Tool", "Failed to export MATT.");
            }
        }

        private async void ExportMaterialEntityBlueprint(byte[] resourceData, uint resourceDataSize, int indexInLibrary, string outputFilePath)
        {
            outputFilePath = string.Format(@"{0}\{1}.json", outputFilePath, indexInLibrary);

            if (ZResourceExporter.ExportMATB(resourceData, resourceDataSize, outputFilePath))
            {
                await this.ShowMessageAsync("HM5 Resource Tool", "MATB successfully exported.");
            }
            else
            {
                await this.ShowMessageAsync("HM5 Resource Tool", "Failed to export MATB.");
            }
        }

        private async void ImportResourceMenuItem_Click(object sender, RoutedEventArgs e)
        {
            ResourceItem? resourceItem = (ResourceItem?)lvResources.SelectedItem;

            if (resourceItem == null)
            {
                return;
            }

            string? fileFormat = resourceItem.FileFormat;
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

            if (backupResourceLibrary)
            {
                MessageDialogResult messageDialogResult = await this.ShowMessageAsync("HM5 Resource Tool", "Do you want to backup header library and resource library?",
                    MessageDialogStyle.AffirmativeAndNegative);

                backupResourceLibrary = messageDialogResult == MessageDialogResult.Affirmative;
            }

            ZResourceImporter resourceImporter = new ZResourceImporter(metadataFilePath, settings.GamePath);

            resourceImporter.ImportResource(inputFilePath, backupResourceLibrary);
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
    }
}
