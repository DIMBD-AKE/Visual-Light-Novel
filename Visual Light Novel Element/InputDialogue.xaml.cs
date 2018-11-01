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

namespace Visual_Light_Novel_Element
{
    /// <summary>
    /// InputDialogue.xaml에 대한 상호 작용 논리
    /// </summary>
    public partial class InputDialogue : Window
    {
        public InputDialogue(string explain)
        {
            InitializeComponent();
            Explain.Text = explain;
        }

        public string Text
        {
            get { return Input.Text; }
        }

        private void Button_Click(object sender, RoutedEventArgs e)
        {
            DialogResult = true;
        }
    }
}
