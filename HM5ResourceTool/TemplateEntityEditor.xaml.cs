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
using System.Windows.Forms;
using System.IO;
using System.Xml;
using MahApps.Metro.Controls;
using HM5ResourceToolCLI;
using ICSharpCode.AvalonEdit.Highlighting;
using ICSharpCode.AvalonEdit.Highlighting.Xshd;
using System.Collections.ObjectModel;
using Newtonsoft.Json;

namespace HM5ResourceTool
{
    /// <summary>
    /// Interaction logic for TemplateEntityEditor.xaml
    /// </summary>
    public partial class TemplateEntityEditor : MetroWindow
    {
        private ZResource? resource;
        private ZHeaderLibrary? headerLibrary;
        private ZHeaderLibrary? globalHeaderLibrary;
        private ZEntityTemplate? entityTemplate;
        private SEntityTemplate? entityTemplate2;
        private TreeNode? previousTreeNode;
        private int count = 0;
        private System.Windows.Forms.TreeView? originalTreeView;
        private Dictionary<string, Dictionary<string, int>>? enums;
        private Settings? settings;
        private ObservableCollection<Property> data;

        public TemplateEntityEditor()
        {
            InitializeComponent();
        }

        public TemplateEntityEditor(ZResource resource, ZHeaderLibrary headerLibrary, ZHeaderLibrary globalHeaderLibrary, Settings? settings)
        {
            InitializeComponent();

            this.resource = resource;
            this.headerLibrary = headerLibrary;
            this.globalHeaderLibrary = globalHeaderLibrary;
            this.settings = settings;
            originalTreeView = new System.Windows.Forms.TreeView();
            data = new ObservableCollection<Property>();

            if (settings != null && settings.Theme != null)
            {
                bool hasLightTheme = settings.Theme.Split('/')[0].Equals("Light");

                if (hasLightTheme)
                {
                    treeView.ForeColor = System.Drawing.ColorTranslator.FromHtml("#000000");
                    treeView.BackColor = System.Drawing.ColorTranslator.FromHtml("#FFFFFF");
                }
                else
                {
                    treeView.ForeColor = System.Drawing.ColorTranslator.FromHtml("#FFFFFF");
                    treeView.BackColor = System.Drawing.ColorTranslator.FromHtml("#252525");
                }

                Utility.ChangeTheme(settings, this, false);
            }

            LoadJSONSyntaxHighlighter();
        }

        private void TemplateEntityEditorWindow_ContentRendered(object sender, EventArgs e)
        {
            LoadEnums();

            entityTemplate = new ZEntityTemplate();
            entityTemplate2 = entityTemplate.JoinTEMPAndTBLU(resource, headerLibrary, globalHeaderLibrary);
            MapNode rootMapNode = entityTemplate.GetGeometryMap(resource, headerLibrary, globalHeaderLibrary);

            SEntityTemplate.SSubEntityTemplate rootEntityTemplate = entityTemplate2.GetRootEntityTemplate();
            TreeNode rootNode = new TreeNode(rootEntityTemplate.GetEntityName());

            rootNode.Tag = rootEntityTemplate.GetIndex();

            AddTemplateEntitiesToTree(rootNode, rootEntityTemplate);

            treeView.Nodes.Add(rootNode);

            if (originalTreeView != null)
            {
                CopyTreeNodes(treeView, originalTreeView);
            }

            //List<DataGridCellData> list = new List<DataGridCellData>
            //{
            //     new DataGridCellData(false,"Content 3_1"),
            //     //new DataGridCellData(true,"Content 3_2")
            //};

            //ObservableCollection<Property> empData = new ObservableCollection<Property>
            //{
            //    new Property{Name="Entity1", Value="12",HeaderType="Entity Info",ControlType=ControlType.TextBox},
            //    new Property{Name="Property1", Value="12",HeaderType="Properties",ControlType=ControlType.TextBox},
            //    new Property{Name="Property2", Value=list,HeaderType="Properties",ControlType=ControlType.ListBox},
            //    new Property{Name="Property1", Value="15",HeaderType="Post Init Properties",ControlType=ControlType.TextBox }
            //};

            //ListCollectionView collection = new ListCollectionView(empData);

            //collection.GroupDescriptions.Add(new PropertyGroupDescription("HeaderType"));

            //dataGrid.ItemsSource = collection;
        }

        private void LoadEnums()
        {
            enums = new Dictionary<string, Dictionary<string, int>>();

            Newtonsoft.Json.Linq.JObject? jsonObject = Newtonsoft.Json.Linq.JObject.Parse(File.ReadAllText("Enums.json"));
            Newtonsoft.Json.Linq.JArray? jsonArray = (Newtonsoft.Json.Linq.JArray?)jsonObject["enums"];

            if (jsonArray != null)
            {
                foreach (var item in jsonArray)
                {
                    Newtonsoft.Json.Linq.JToken? token = item["name"];
                    Newtonsoft.Json.Linq.JArray? jsonArray2 = (Newtonsoft.Json.Linq.JArray?)item["items"];

                    if (token != null && jsonArray2 != null)
                    {
                        string name = token.ToString();
                        Dictionary<string, int> items = new Dictionary<string, int>();

                        foreach (var item2 in jsonArray2)
                        {
                            Newtonsoft.Json.Linq.JToken? token2 = item2["name"];
                            Newtonsoft.Json.Linq.JToken? token3 = item2["value"];

                            if (token2 != null && token3 != null)
                            {
                                items.Add(token2.ToString(), Convert.ToInt32(token3));
                            }
                        }

                        enums.Add(name, items);
                    }
                }
            }
        }

        private void AddTemplateEntitiesToTree(TreeNode parentNode, SEntityTemplate.SSubEntityTemplate subEntityTemplate)
        {
            List<SEntityTemplate.SSubEntityTemplate> children = subEntityTemplate.GetChildren();
            int subEntityTemplateCount = subEntityTemplate.GetChildren().Count;

            for (int i = 0; i < subEntityTemplateCount; ++i)
            {
                TreeNode childNode = new TreeNode(children[i].GetEntityName());

                childNode.Tag = children[i].GetIndex();

                AddTemplateEntitiesToTree(childNode, children[i]);

                parentNode.Nodes.Add(childNode);
            }
        }

        private void ScrollViewer_PreviewMouseWheel(object sender, MouseWheelEventArgs e)
        {
            ScrollViewer scrollViewer = (ScrollViewer)sender;

            scrollViewer.ScrollToVerticalOffset(scrollViewer.VerticalOffset - e.Delta);

            e.Handled = true;
        }

        private void TreeView_AfterSelect(object sender, TreeViewEventArgs e)
        {
            if (entityTemplate2 != null && e.Node != null)
            {
                if (count == 0)
                {
                    previousTreeNode = e.Node;
                }

                if (count == 2)
                {
                    previousTreeNode = e.Node;
                    count = 0;
                }

                int index = (int)e.Node.Tag;

                textEditor.Text = entityTemplate2.GetEntityDetails(index);
                txtEntityName.Content = e.Node.Text;

                DisplayProperties();

                ++count;

                lblReferences.Text = "";
                lblReferences2.Text = "";
                lblBackReferences.Text = "";
                lblBackReferences2.Text = "";

                List<string> references = entityTemplate2.GetEntityReferences(index);
                List<string> backReferences = entityTemplate2.GetEntityBackReferences(index);
                int referenceCount = references.Count;
                int backReferenceCount = backReferences.Count;

                for (int i = 0; i < referenceCount; i++)
                {
                    if (i % 2 == 0)
                    {
                        lblReferences.Text += references[i];

                        if (i < referenceCount - 1)
                        {
                            lblReferences.Text += "\n\n";
                        }
                    }
                    else
                    {
                        lblReferences2.Text += references[i];

                        if (i < referenceCount - 1)
                        {
                            lblReferences2.Text += "\n\n";
                        }
                    }
                }

                for (int i = 0; i < backReferenceCount; i++)
                {
                    if (i % 2 == 0)
                    {
                        lblBackReferences.Text += backReferences[i];

                        if (i < backReferenceCount - 1)
                        {
                            lblBackReferences.Text += "\n\n";
                        }
                    }
                    else
                    {
                        lblBackReferences2.Text += backReferences[i];

                        if (i < backReferenceCount - 1)
                        {
                            lblBackReferences2.Text += "\n\n";
                        }
                    }
                }
            }
        }

        private void TemplateEntityEditorWindow_Closed(object sender, EventArgs e)
        {
            if (entityTemplate != null)
            {
                entityTemplate.Dispose();
            }

            if (entityTemplate2 != null)
            {
                entityTemplate2.Dispose();
            }
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

        private void TxtContent_TextChanged(object sender, TextChangedEventArgs e)
        {
            if (entityTemplate2 == null || chkExactSearch.IsChecked == null || originalTreeView == null)
            {
                return;
            }

            TreeNode parent, parent2;

            treeView.BeginUpdate();
            treeView.Nodes.Clear();

            if (txtContent.Text.Length == 0)
            {
                parent = originalTreeView.Nodes[0];
                parent2 = new TreeNode(parent.Text);

                parent2.Tag = parent.Tag;

                ResetTree(parent, parent2);
            }
            else
            {
                List<int> indices = entityTemplate2.SearchContent(txtContent.Text, (bool)chkExactSearch.IsChecked);

                if (indices.Count > 0)
                {
                    parent = originalTreeView.Nodes[0];
                    parent2 = new TreeNode(parent.Text);

                    parent2.Tag = parent.Tag;

                    FilterTree(parent, parent2, indices);
                }
                else
                {
                    parent = originalTreeView.Nodes[0];
                    parent2 = new TreeNode(parent.Text);

                    parent2.Tag = parent.Tag;

                    ResetTree(parent, parent2);
                }
            }

            treeView.Nodes.Add(parent2);
            treeView.EndUpdate();
        }

        private void FilterTree(TreeNode parent, TreeNode parent2, List<int> indices)
        {
            TreeNode newTreeNode = new TreeNode();

            foreach (TreeNode treeNode in parent.Nodes)
            {
                int index = (int)treeNode.Tag;

                if (indices.Contains(index))
                {
                    newTreeNode.Text = treeNode.Text;
                    newTreeNode.Tag = treeNode.Tag;

                    parent2.Nodes.Add(newTreeNode);
                }

                FilterTree(treeNode, newTreeNode, indices);
            }
        }

        private void CopyTreeNodes(System.Windows.Forms.TreeView treeview1, System.Windows.Forms.TreeView treeview2)
        {
            TreeNode newTreeNode;

            foreach (TreeNode treeNode in treeview1.Nodes)
            {
                newTreeNode = new TreeNode(treeNode.Text);
                newTreeNode.Tag = treeNode.Tag;

                CopyChildren(newTreeNode, treeNode);
                treeview2.Nodes.Add(newTreeNode);
            }
        }

        private void CopyChildren(TreeNode parent, TreeNode original)
        {
            TreeNode newTreeNode;

            foreach (TreeNode treeNode in original.Nodes)
            {
                newTreeNode = new TreeNode(treeNode.Text);
                newTreeNode.Tag = treeNode.Tag;

                parent.Nodes.Add(newTreeNode);
                CopyChildren(newTreeNode, treeNode);
            }
        }

        private void ResetTree(TreeNode parent, TreeNode parent2)
        {
            TreeNode newTreeNode;

            foreach (TreeNode treeNode in parent.Nodes)
            {
                newTreeNode = new TreeNode(treeNode.Text);
                newTreeNode.Tag = treeNode.Tag;

                parent2.Nodes.Add(newTreeNode);

                ResetTree(treeNode, newTreeNode);
            }
        }

        private void TxtContent_GotFocus(object sender, RoutedEventArgs e)
        {
            if (txtContent.Text.Equals("Search query"))
            {
                txtContent.Clear();
            }
        }

        private void TxtContent_LostFocus(object sender, RoutedEventArgs e)
        {
            if (txtContent.Text.Length == 0)
            {
                txtContent.Text = "Search query";
            }
        }

        private void BtnCollapseTree_Click(object sender, RoutedEventArgs e)
        {
            treeView.CollapseAll();
        }

        private void BtnGoToPreviousEntity_Click(object sender, RoutedEventArgs e)
        {
            if (previousTreeNode != null)
            {
                treeView.SelectedNode = previousTreeNode.PrevNode;
                treeView.Focus();
            }
        }

        private void DisplayProperties()
        {
            if (entityTemplate2 != null && treeView.SelectedNode != null)
            {
                data.Clear();

                int index = (int)treeView.SelectedNode.Tag;
                Dictionary<string, Dictionary<string, List<string>>> properties = entityTemplate2.GetPropertyTypesAndValues(index);
                Dictionary<string, Dictionary<string, List<string>>> postInitProperties = entityTemplate2.GetPostInitPropertyTypesAndValues(index);
                List<string> propertyAliases = entityTemplate2.GetPropertyAliases(index);

                string? entityName = txtEntityName.Content.ToString();

                if (entityName != null)
                {
                    List<BaseType> values = new List<BaseType>
                    {
                        new BaseType(entityName)
                    };

                    List<BaseType> values2 = new List<BaseType>
                    {
                        new BaseType(entityTemplate2.GetTemplateEntityResourceID(index))
                    };

                    List<BaseType> values3 = new List<BaseType>
                    {
                        new BaseType(entityTemplate2.GetTemplateEntityBlueprintResourceID(index))
                    };

                    data.Add(new Property { Name = "Name", Value = values, HeaderType = "Entity Info", TypeName = "BaseType" });
                    data.Add(new Property { Name = "Entity Template", Value = values2, HeaderType = "Entity Info", TypeName = "BaseType" });
                    data.Add(new Property { Name = "Entity Blueprint", Value = values3, HeaderType = "Entity Info", TypeName = "BaseType" });
                }

                AddPropertiesToDataGrid(properties, data, "Properties");
                AddPropertiesToDataGrid(postInitProperties, data, "Post Init Properties");
                AddPropertyAliasesToDataGrid(propertyAliases, data, "Property Aliases");

                ListCollectionView collection = new ListCollectionView(data);

                collection.GroupDescriptions.Add(new PropertyGroupDescription("HeaderType"));

                dataGrid.ItemsSource = collection;
            }
        }

        private void TabControl_SelectionChanged(object sender, SelectionChangedEventArgs e)
        {
            switch (tabControl.SelectedIndex)
            {
                case 0:
                    propertyButtonsGrid.Visibility = Visibility.Collapsed;
                    break;
                case 1:
                    propertyButtonsGrid.Visibility = Visibility.Visible;
                    break;
            }
        }

        private void AddPropertiesToDataGrid(Dictionary<string, Dictionary<string, List<string>>> properties, ObservableCollection<Property> data, string headerType)
        {
            foreach (KeyValuePair<string, Dictionary<string, List<string>>> pair in properties)
            {
                Dictionary<string, List<string>> typeNames = pair.Value;

                foreach (KeyValuePair<string, List<string>> pair2 in typeNames)
                {
                    switch (pair2.Key)
                    {
                        case "bool":
                            {
                                List<bool> values = new List<bool>
                                {
                                    Convert.ToBoolean(Convert.ToInt32(pair2.Value[0]))
                                };

                                data.Add(new Property { Name = pair.Key, Value = values, HeaderType = headerType, TypeName = "Bool" });
                                break;
                            }
                        case "int8":
                        case "char":
                        case "uint8":
                        case "int16":
                        case "uint16":
                        case "int32":
                        case "uint32":
                        case "int64":
                        case "uint64":
                        case "float32":
                        case "float64":
                            {
                                List<BaseType> values = new List<BaseType>
                                {
                                    new BaseType(pair2.Value[0])
                                };

                                data.Add(new Property { Name = pair.Key, Value = values, HeaderType = headerType, TypeName = "BaseType" });
                                break;
                            }
                        case string s when s.StartsWith("Enum#"):
                            {
                                if (enums != null)
                                {
                                    Dictionary<string, int> values = enums[pair2.Key[5..]];
                                    Enum enum1 = new Enum(pair2.Value[0], values[pair2.Value[0]], values.Keys.ToList());
                                    //List<Enum> values2 = new List<Enum>();

                                    //foreach (KeyValuePair<string, int> pair3 in values)
                                    //{
                                    //    values2.Add(new Enum(pair3.Key, pair3.Value));
                                    //}

                                    List<Enum> values2 = new List<Enum>
                                    {
                                        enum1
                                    };

                                    data.Add(new Property { Name = pair.Key, Value = values2, HeaderType = headerType, TypeName = "Enum" });
                                }

                                break;
                            }
                        case "SVector2":
                            {
                                string[] values = pair2.Value[0].Split('|');
                                SVector2 vector = new SVector2(values[0], values[1]);

                                List<SVector2> values2 = new List<SVector2>
                                {
                                    vector
                                };

                                data.Add(new Property { Name = pair.Key, Value = values2, HeaderType = headerType, TypeName = "SVector2" });
                                break;
                            }
                        case "SVector3":
                            {
                                string[] values = pair2.Value[0].Split('|');
                                SVector3 vector = new SVector3(values[0], values[1], values[2]);

                                List<SVector3> values2 = new List<SVector3>
                                {
                                    vector
                                };

                                data.Add(new Property { Name = pair.Key, Value = values2, HeaderType = headerType, TypeName = "SVector3" });
                                break;
                            }
                        case "SMatrix43":
                            {
                                string[] values = pair2.Value[0].Split('#');
                                string[] rotationValues = values[0].Split('|');
                                string[] postionValues = values[1].Split('|');

                                SVector3 rotationVector = new SVector3(rotationValues[0], rotationValues[1], rotationValues[2]);
                                SVector3 positionVector = new SVector3(postionValues[0], postionValues[1], postionValues[2]);
                                SMatrix43 transform = new SMatrix43(rotationVector, positionVector);

                                List<SMatrix43> values2 = new List<SMatrix43>
                                {
                                    transform
                                };

                                data.Add(new Property { Name = pair.Key, Value = values2, HeaderType = headerType, TypeName = "SMatrix43" });
                                break;
                            }
                        case "SColorRGB":
                            {
                                string[] values = pair2.Value[0].Split('|');
                                SColorRGB colorRGB = new SColorRGB(values[0], values[1], values[2]);

                                List<SColorRGB> values2 = new List<SColorRGB>
                                {
                                    colorRGB
                                };

                                data.Add(new Property { Name = pair.Key, Value = values2, HeaderType = headerType, TypeName = "SColorRGB" });
                                break;
                            }
                        case "SColorRGBA":
                            {
                                string[] values = pair2.Value[0].Split('|');
                                SColorRGBA colorRGBA = new SColorRGBA(values[0], values[1], values[2], values[3]);

                                List<SColorRGBA> values2 = new List<SColorRGBA>
                                {
                                    colorRGBA
                                };

                                data.Add(new Property { Name = pair.Key, Value = values2, HeaderType = headerType, TypeName = "SColorRGBA" });
                                break;
                            }
                        case "SEntityTemplateReference":
                            {
                                if (entityTemplate2 != null)
                                {
                                    string[] values = pair2.Value[0].Split('|');
                                    string entityName = entityTemplate2.GetEntityName(Convert.ToInt32(values[0]));
                                    SEntityTemplateReference entityTemplateReference = new SEntityTemplateReference(values[0], entityName, values[1]);

                                    List<SEntityTemplateReference> values2 = new List<SEntityTemplateReference>
                                    {
                                        entityTemplateReference
                                    };

                                    data.Add(new Property { Name = pair.Key, Value = values2, HeaderType = headerType, TypeName = "SEntityTemplateReference" });
                                }

                                break;
                            }
                        case "SBodyPartDamageMultipliers":
                            break;
                        case "ZRuntimeResourceID":
                            {
                                if (resource == null)
                                {
                                    return;
                                }

                                ulong hash = Convert.ToUInt64(pair2.Value[0]);
                                List<ZResource> references = resource.GetReferences();
                                int referenceCount = references.Count;
                                ZRuntimeResourceID? runtimeResourceID = null;

                                for (int i = 0; i < referenceCount; ++i)
                                {
                                    if (references[i].GetRuntimeResourceID().GetID() == hash)
                                    {
                                        string flags = string.Format("0x{0}", references[i].GetResourceReferenceFlags().ToString("X"));

                                        runtimeResourceID = new ZRuntimeResourceID(references[i].GetResourceID(), flags);

                                        break;
                                    }
                                }

                                if (runtimeResourceID != null)
                                {
                                    List<ZRuntimeResourceID> values2 = new List<ZRuntimeResourceID>
                                    {
                                        runtimeResourceID
                                    };

                                    data.Add(new Property { Name = pair.Key, Value = values2, HeaderType = headerType, TypeName = "ZRuntimeResourceID" });
                                }

                                break;
                            }
                    }
                }
            }
        }

        private void AddPropertyAliasesToDataGrid(List<string> propertyAliases, ObservableCollection<Property> data, string headerType)
        {
            if (entityTemplate2 != null)
            {
                List<SEntityTemplatePropertyAlias> values2 = new List<SEntityTemplatePropertyAlias>();

                foreach (var property in propertyAliases)
                {
                    string[] values = property.Split('|');
                    string entityName = entityTemplate2.GetEntityName(Convert.ToInt32(values[1]));

                    values2.Add(new SEntityTemplatePropertyAlias(values[0], entityName, values[2]));
                }

                if (values2.Count > 0)
                {
                    data.Add(new Property { Name = "propertyAliases", Value = values2, HeaderType = headerType, TypeName = "SEntityTemplatePropertyAlias" });
                }
            }
        }

        private void BtnAddProperty_Click(object sender, RoutedEventArgs e)
        {
            PropertyDialog propertyDialog = new PropertyDialog(settings);

            if (propertyDialog.ShowDialog() ?? true)
            {

            }
            List<BaseType> values = new List<BaseType>
                    {
                        new BaseType("100")
                    };

            data.Add(new Property { Name = "New property", Value = values, HeaderType = "Properties", TypeName = "BaseType" });
            //(dataGrid.ItemsSource as ListCollectionView).Refresh();
        }

        private void BtnRemoveProperty_Click(object sender, RoutedEventArgs e)
        {

        }

        private void BtnAddPropertyAlias_Click(object sender, RoutedEventArgs e)
        {
            //Add alias to list without displaying dialog
        }

        private void BtnRemovePropertyAlias_Click(object sender, RoutedEventArgs e)
        {

        }
    }
}
