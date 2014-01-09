using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Shapes;

namespace WPFDispatchMarketData
{
    /// <summary>
    /// Interaction logic for MenuWindow.xaml
    /// </summary>
    public partial class MenuWindow : Window
    {
        public MenuWindow()
        {
            InitializeComponent();
        }

        private void buttonMarketDataPathManagerWindow_Click(object sender, RoutedEventArgs e)
        {
            System.Windows.Window newWin = new WPFDispatchMarketData.MarketDataPathManagerWindow();
            newWin.Show();
        }
    }
}
