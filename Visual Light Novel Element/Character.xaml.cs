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
using System.Windows.Navigation;
using System.Windows.Shapes;
using System.IO;
using Newtonsoft.Json.Linq;
using System.Windows.Threading;
using System.ComponentModel;

namespace Visual_Light_Novel_Element
{
    /// <summary>
    /// Character.xaml에 대한 상호 작용 논리
    /// </summary>

    public partial class Character : Page, IElement
    {
        public Character()
        {
            InitializeComponent();
        }

        public JObject GetJson()
        {
            var json = new JObject();

            json.Add("TYPE", "CHARACTER");
            json.Add("NAME", Name.Text);
            json.Add("EXPLAIN", new TextRange(Explain.Document.ContentStart, Explain.Document.ContentEnd).Text);

            var state = new JArray();
            foreach (Tuple<string, string> item in StateList.Items)
            {
                var list = new JObject();
                list.Add("NAME", item.Item1);
                list.Add("PATH", item.Item2);
                state.Add(list);
            }

            json.Add("STATE", state);

            return json;
        }

        public void SetJson(JObject json)
        {
            Name.Text = (string)json["NAME"];
            Explain.Document.Blocks.Clear();
            Explain.Document.Blocks.Add(new Paragraph(new Run((string)json["EXPLAIN"])));

            foreach (var item in json["STATE"])
            {
                string state = (string)item["NAME"];
                string path = (string)item["PATH"];
                Tuple<string, string> data = new Tuple<string, string>(state, path);
                StateList.Items.Add(data);
            }
        }

        private void Add_Click(object sender, RoutedEventArgs e)
        {
            Microsoft.Win32.OpenFileDialog dialog = new Microsoft.Win32.OpenFileDialog();

            dialog.Filter = "Image (*.png, *.jpg)|*.png;*.jpg";
            dialog.InitialDirectory = AppDomain.CurrentDomain.BaseDirectory;

            bool? result = dialog.ShowDialog();

            if (result == true)
            {
                string fileName = dialog.FileName;
                fileName = fileName.Remove(0, dialog.InitialDirectory.Length);
                InputDialogue input = new InputDialogue("상태 이름");
                if (input.ShowDialog() == true)
                {
                    string stateName = input.Text;
                    Tuple<string, string> data = new Tuple<string, string>(stateName, fileName);
                    StateList.Items.Add(data);
                }
            }
        }

        private void Delete_Click(object sender, RoutedEventArgs e)
        {
            StateList.Items.Remove(StateList.SelectedItem);
        }

        private void StateList_SelectionChanged(object sender, SelectionChangedEventArgs e)
        {
            if (StateList.SelectedItem == null)
                return;

            Tuple<string, string> select = (Tuple<string, string>)StateList.SelectedItem;
            
            var group = new DrawingGroup();
            foreach (Tuple<string, string> item in StateList.Items)
            {
                if (select.Item1 == item.Item1)
                {
                    string path = AppDomain.CurrentDomain.BaseDirectory + item.Item2;

                    BitmapImage image = new BitmapImage();
                    image.BeginInit();
                    image.UriSource = new Uri(path);
                    image.EndInit();
                    group.Children.Add(new ImageDrawing(image, new Rect(0, 0, image.Width, image.Height)));

                    
                }
            }
            Preview.Source = new DrawingImage(group);
        }
    }
}
