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
using System.Windows.Navigation;
using System.Windows.Shapes;
using System.IO;

namespace WPFDispatchMarketData
{
    /// <summary>
    /// Interaction logic for Window1.xaml
    /// </summary>
    public partial class MarketDataPathManagerWindow : Window
    {
        //c://TestData//HistoryMarketDataTotal   20131220.csv 
        //C://TestData//HistoryMarketDataInstrument   3320.csv

        private System.String m_strInitText_ButtonOneInstrumentOneFile = System.String.Empty;//OneInstrumentOneFile
        private System.String m_strClickText_ButtonOneInstrumentOneFile = System.String.Empty;//Run...
        private System.String m_strPathHistoryMarketDataTotal = System.String.Empty;//c://TestData//HistoryMarketDataTotal
        private System.String m_strMarketDataFileExtenName = System.String.Empty;//*.csv
        private System.String m_strPathHistoryMarketDataInstrument = System.String.Empty;//"C://TestData//HistoryMarketDataInstrument"

        private FileInfo[] m_lstAllFilesInPath;
        private int m_nlstAllFilesInPathSize = 0;
        private IDictionary<long, string> m_mapTimeAndFileName;
        private IDictionary<string, StreamWriter> m_mapInstrumentIDAndFileName;
        private System.ComponentModel.BackgroundWorker m_backgroundWorkerAnalystWoker;

        //log output
        private System.String m_strSysLog = System.String.Empty;
        private System.IO.StreamWriter m_SysLogHandle = null;


        public MarketDataPathManagerWindow()
        {
            InitializeComponent();

            InitializeData();
            InitializeEvent();
        }

        private void buttonOneInstrumentOneFile_Click(object sender, RoutedEventArgs e)
        {

            this.buttonOneInstrumentOneFile.IsEnabled = false;
            //this.buttonOneThreadOnefile.Background = Brushes.Red;
            this.buttonOneInstrumentOneFile.Content = m_strClickText_ButtonOneInstrumentOneFile;//"Run...";

            this.m_strPathHistoryMarketDataTotal = this.textPathHistoryMarketDataTotal.Text;
            this.m_strPathHistoryMarketDataInstrument = this.textPathHistoryMarketDataInstrument.Text;

            //lsl for test
            //m_strMarketDataPath = System.String.Empty;//c://HistoryMarketData
            //m_strMarketDataFileExtenName = System.String.Empty;//*.csv

            this.m_backgroundWorkerAnalystWoker.RunWorkerAsync();
        }


        /////////////////////////////////////////////////
        /////////////////////////////////////////////////
        private void MarketDataPathManagerWindow_Loaded(object sender, RoutedEventArgs e)
        {
            //this.buttonOneThreadOnefile.Background = Brushes.Green;
            this.buttonOneInstrumentOneFile.Content = m_strInitText_ButtonOneInstrumentOneFile;//"OneInstrumentOneFile";
            this.buttonOneInstrumentOneFile.IsEnabled = true;

            //获取应用程序的当前工作目录
            var varCurrentPath = AppDomain.CurrentDomain.BaseDirectory;
            this.textPathHistoryMarketDataTotal.Text = varCurrentPath.ToString();
            this.textPathHistoryMarketDataInstrument.Text = varCurrentPath.ToString();

            //c://TestData//HistoryMarketDataTotal   20131220.csv 
            //C://TestData//HistoryMarketDataInstrument   3320.csv

            this.textPathHistoryMarketDataTotal.Text = "c://TestData//HistoryMarketDataTotal";
            this.textPathHistoryMarketDataInstrument.Text = "C://TestData//HistoryMarketDataInstrument";
        }


        private void InitializeData()
        {
            this.m_strInitText_ButtonOneInstrumentOneFile = "OneInstrumentOneFile";
            this.m_strClickText_ButtonOneInstrumentOneFile = "Run...";
            this.m_strPathHistoryMarketDataTotal = "C://TestData//HistoryMarketDataTotal";
            this.m_strMarketDataFileExtenName = "*.csv";
            this.m_strPathHistoryMarketDataInstrument = "C://TestData//HistoryMarketDataInstrument";
            this.m_strSysLog = "SysLog.log";
            this.m_SysLogHandle = null;
            this.m_lstAllFilesInPath = null;
            this.m_mapTimeAndFileName = null;
            this.m_nlstAllFilesInPathSize = 0;
            this.m_mapInstrumentIDAndFileName = null;
            this.m_backgroundWorkerAnalystWoker = null;

            this.m_backgroundWorkerAnalystWoker = new System.ComponentModel.BackgroundWorker();

            m_mapTimeAndFileName = new Dictionary<long, string>();
            m_mapInstrumentIDAndFileName = new Dictionary<string, StreamWriter>();


        }//InitializeData

        private void InitializeEvent()
        {
            //create one thread to do work
            //register event  DoWork  RunWorkerCompleted

            this.m_backgroundWorkerAnalystWoker.DoWork +=
                new System.ComponentModel.DoWorkEventHandler(this.BackGroundWorkerAnalystWoker_DoWork);

            this.m_backgroundWorkerAnalystWoker.RunWorkerCompleted +=
                new System.ComponentModel.RunWorkerCompletedEventHandler(this.BackGroundWorkerAnalystWoker_DoWorkeCompleted);

        }//InitializeEvent











        /////////////////////////////////////////////////
        /////////////////////////////////////////////////



        private void BackGroundWorkerAnalystWoker_DoWork(object sender, System.ComponentModel.DoWorkEventArgs e)
        {
            //MessageBox.Show("Run begin");

            //CreateDirectory m_strSaveDataPath
            CreateDirectory(this.m_strPathHistoryMarketDataInstrument);

            if (null == this.m_SysLogHandle)
            {
                //System.IO.File.Delete(m_strSysLog);
                //this.m_SysLogHandle = new StreamWriter(File.Open(m_strSysLog, System.IO.FileMode.Append, FileAccess.Write));
                //this.m_SysLogHandle = new System.IO.StreamWriter(m_strSysLog);
            }

            //get all File name in path
            this.m_lstAllFilesInPath = GetALLFilesInPath(this.m_strPathHistoryMarketDataTotal, this.m_strMarketDataFileExtenName);
            if (null == this.m_lstAllFilesInPath)
            {
                return;
            }

            //get time in file name and sort by time
            SortFilesByTime(this.m_lstAllFilesInPath, this.m_mapTimeAndFileName);

            //Analyse all file 
            this.m_nlstAllFilesInPathSize = this.m_mapTimeAndFileName.Count;
            AnalyseAllFiles(this.m_mapTimeAndFileName, m_mapInstrumentIDAndFileName);

            this.m_mapTimeAndFileName.Clear();
            m_mapInstrumentIDAndFileName.Clear();

        }//BackgroundWorkerAnalystFile_DoWork

        private void BackGroundWorkerAnalystWoker_DoWorkeCompleted(object sender, System.ComponentModel.RunWorkerCompletedEventArgs e)
        {
            //this.buttonOneThreadOnefile.Background = Brushes.Green;
            this.buttonOneInstrumentOneFile.Content = m_strInitText_ButtonOneInstrumentOneFile;//"OneInstrumentOneFile";
            this.buttonOneInstrumentOneFile.IsEnabled = true;

            MessageBox.Show("Analyst Work end");
        }//BackgroundWorkerAnalystFile_RunWorkerCompleted



        //////////////////////////////////////////////////////////////////////////
        /// <summary>
        /// get all file names in Path
        /// </summary>
        /// <param name="strPath"></param>
        /// <param name="strFileType"></param>
        /// <returns></returns>
        private FileInfo[] GetALLFilesInPath(string strPath, string strFileType)
        {
            System.IO.DirectoryInfo dirInfo;
            FileInfo[] filelist = null;

            // Specify the directories you want to manipulate.
            dirInfo = new System.IO.DirectoryInfo(strPath);

            try
            {
                // Determine whether the directory exists.
                if (!dirInfo.Exists)
                {
                    // Indicate that the directory already exists.

                    System.String strLog = String.Format("Error: That path not exists  strPath ={0}", strPath);
                    //this.m_logHandle.WriteLine(strLog);
                    return null;
                }

                //get all file
                filelist = dirInfo.GetFiles(strFileType);//"*.csv"

            }
            catch
            {
                System.String strLog = String.Format("Error: System.IO.DirectoryInfo GetFiles failed!");
            }
            finally
            {
            }

            return filelist;

        }//getFiles


        /// <summary>
        /// SortFilesByTime
        /// AnalyseAllFileName in filelist, and add in map
        /// eg. 11	"c://HistoryMarketData//20131220.csv"
        /// </summary>
        /// <param name="filelist"></param>
        /// <param name="logIndexFileMap"></param>
        /// <returns></returns>
        private int SortFilesByTime(FileInfo[] filelist, IDictionary<long, string> mapTimeAndFileName)
        {
            int nFunRes = 0;
            System.String strFileFullPath = "";//"c://HistoryMarketData//20131220.csv"
            System.String strFileName = "";   //20131220.csv  
            System.String strFileNameTime = "";//20131220
            long nFileNameTimeToLong = -1;//2
            int nfindSunStr = -1;


            try
            {

                foreach (FileInfo item in filelist)
                {
                    nfindSunStr = -1;
                    strFileFullPath = item.FullName;
                    strFileName = item.Name;

                    //get time
                    //20131220.csv
                    if (strFileName.Length > 0)
                    {
                        nfindSunStr = -1;
                        nfindSunStr = strFileName.IndexOf(".");
                    }
                    if (nfindSunStr > 0)
                    {
                        //20131220
                        strFileNameTime = strFileName.Substring(0, nfindSunStr);//get index
                        nfindSunStr = -1;
                    }

                    //add to map
                    if (strFileNameTime.Length > 0)
                    {
                        //20131220
                        //"c://HistoryMarketData//20131220.csv"
                        nfindSunStr = -1;
                        System.DateTime nFileTime = System.DateTime.ParseExact(strFileNameTime, "yyyyMMdd", System.Globalization.CultureInfo.CurrentCulture);
                        nFileNameTimeToLong = nFileTime.ToFileTime();//January 1, 1601 C.E. UTC
                        mapTimeAndFileName.Add(nFileNameTimeToLong, strFileFullPath);
                    }


                }//foreach

            }
            catch
            {
                Console.WriteLine("SortFilesByTime error!");
                nFunRes = -1;
            }
            finally
            {
                //
                //Console.WriteLine("finally");
            }

            return nFunRes;
        }//getLogIndexFileMap


        /// <summary>
        /// AnalyseAllFiles
        /// </summary>
        /// <param name="logIndexFileMap"></param>
        /// <param name="mapThreadNumAndFileName"></param>
        /// <returns></returns>
        private int AnalyseAllFiles(IDictionary<long, string> mapTimeAndFileName,
            IDictionary<string, StreamWriter> mapInstrumentIDAndFileName)
        {
            int nFunRes = 0;
            List<long> lstTimeForSort = null;
            System.String strFileName = "";
            StreamWriter dstFileHandle = null;

            //sort file index
            lstTimeForSort = new List<long>(mapTimeAndFileName.Keys);
            lstTimeForSort.Sort();

            //analyse file one by one
            foreach (long nFileNameTimeToLong in lstTimeForSort)
            {
                strFileName = mapTimeAndFileName[nFileNameTimeToLong];
                AnalyseOneFile(strFileName, mapInstrumentIDAndFileName);
            }

            //close all file handle
            foreach (KeyValuePair<string, StreamWriter> item in mapInstrumentIDAndFileName)
            {
                dstFileHandle = item.Value;
                dstFileHandle.Flush();
                dstFileHandle.Close();
            }
            lstTimeForSort.Clear();
            mapInstrumentIDAndFileName.Clear();

            return nFunRes;
        }//AnalyseAllFiles



        /// <summary>
        /// AnalyseFile
        /// Analyse each File
        /// </summary>
        /// <param name="strFileName"></param>
        /// <param name="mapThreadNumAndFileName"></param>
        /// <returns></returns>
        private int AnalyseOneFile(System.String strFileName,
            IDictionary<string, StreamWriter> mapInstrumentIDAndFileName)
        {
            int nFunRes = 0;
            System.IO.StreamReader fileHander = null;

            try
            {
                // Create an instance of StreamReader to read from a file.
                // The using statement also closes the StreamReader.
                fileHander = new System.IO.StreamReader(strFileName);

                String strGetLine;
                // Read and display lines from the file until the end of 
                // the file is reached.
                while ((strGetLine = fileHander.ReadLine()) != null)
                {
                    AnalyseOneLine(strGetLine, mapInstrumentIDAndFileName);
                }

                fileHander.Close();

            }
            catch (Exception e)
            {
                // Let the user know what went wrong.
                //this.m_logHandle.WriteLine("The file could not be read:");
                //this.m_logHandle.WriteLine(e.Message);
                MessageBox.Show(e.Message);
                nFunRes = -1;
            }

            return nFunRes;
        }//AnalyseFile




        /// <summary>
        /// AnalyseLogFileLine
        /// Analyse one line in file
        /// 3620,1387500039,4294966511,3,2375.2,2342.4,2368.2,2378,2337.4,2340.8,4,216326,2340.6,2340.6,2340.4,2340.2,2340,2340.8,2341.2,2341.6,2341.8,2342,1,1,1,2,14,3,2,38,17,43
        /// </summary>
        /// <param name="strLogFileLine"></param>
        /// <param name="mapThreadNumAndFileName"></param>
        /// <returns></returns>
        private int AnalyseOneLine(System.String strLineInFile,
            IDictionary<string, StreamWriter> mapInstrumentIDAndFileName)
        {
            int nFunRes = 0;
            System.String strInstrumentID = "";
            System.String strDstFileName = "";
            StreamWriter dstFileHandle = null;

            //check line contain string "[Thrd:"
            if (!checkLogFileLine(strLineInFile))
            {
                nFunRes = -1;
                return nFunRes;
            }

            //get string 3320
            strInstrumentID = getInstrumentIDFromOneLine(strLineInFile);
            if (strInstrumentID.Length <= 0)
            {
                nFunRes = -1;
                return nFunRes;
            }

            //get dst file name "C://SaveDataPath//3320.csv"
            strDstFileName = getDstFileNameByInstrumentID(strInstrumentID);
            if (strDstFileName.Length <= 0)
            {
                nFunRes = -1;
                return nFunRes;
            }

            //get or new one StreamWriter
            if (mapInstrumentIDAndFileName.ContainsKey(strInstrumentID))
            {
                dstFileHandle = mapInstrumentIDAndFileName[strInstrumentID];
            }
            else
            {
                dstFileHandle = new StreamWriter(strDstFileName);
                mapInstrumentIDAndFileName.Add(strInstrumentID, dstFileHandle);
            }

            //write to dst file
            if (null != dstFileHandle)
            {
                dstFileHandle.WriteLine(strLineInFile);
            }

            return nFunRes;
        }//AnalyseFile



        /// <summary>
        /// checkLogFileLine
        /// check one line in log file contain string "[Thrd:"
        /// </summary>
        /// <param name="strLogFileLine"></param>
        /// <returns></returns>
        private Boolean checkLogFileLine(System.String strLogFileLine)
        {
            Boolean nFunRes = false;
            int nfindSunStr = -1;

            if (strLogFileLine.Length > 0)
            {
                nfindSunStr = -1;
                nfindSunStr = strLogFileLine.IndexOf(",");
            }

            if (nfindSunStr >= 0)
            {
                nFunRes = true;
            }

            return nFunRes;
        }


        /// <summary>
        /// getInstrumentIDFromOneLine
        /// eg. get "3320" from one line in one line
        /// </summary>
        /// <param name="strLogFileLine"></param>
        /// <returns></returns>
        private System.String getInstrumentIDFromOneLine(System.String strLineInFile)
        {
            System.String strInstrumentID = "";
            int nfindSunStr = -1;


            //3320,****
            if (strLineInFile.Length > 0)
            {
                nfindSunStr = -1;
                nfindSunStr = strLineInFile.IndexOf(",");
            }
            if (nfindSunStr > 0)
            {
                //3320,****
                strInstrumentID = strLineInFile.Substring(0, nfindSunStr);
                nfindSunStr = -1;
            }

            return strInstrumentID;
        }//AnalyseFile


        /// <summary>
        ///	getDstFileNameByInstrumentID   eg. is line contains: 3320 then dest file: ./DstLogPath/3320.csv
        /// </summary>
        /// <param name="strThrdNum"></param>
        /// <returns></returns>
        private System.String getDstFileNameByInstrumentID(System.String strInstrumentID)
        {
            System.String strDstFileName = "";

            strDstFileName = m_strPathHistoryMarketDataInstrument + "//" + strInstrumentID + ".csv";

            return strDstFileName;
        }//AnalyseFile



        /// <summary>
        /// delete all files and SubDirectory in this path
        /// </summary>
        /// <param name="targetDir"></param>
        /// <param name="delSubDir"></param> 		
        public void DeleteFiles(System.String targetDir, Boolean delSubDir)
        {
            foreach (System.String fileName in Directory.GetFiles(targetDir))
            {
                File.SetAttributes(fileName, FileAttributes.Normal);
                File.Delete(fileName);
            }

            if (delSubDir)
            {
                DirectoryInfo dir = new DirectoryInfo(targetDir);
                foreach (DirectoryInfo subDi in dir.GetDirectories())
                {
                    DeleteFiles(subDi.FullName, true);
                    subDi.Delete();
                }

            }
        }


        /// <summary>
        /// create Directory, if Directory is exists then delete all files and SubDirectory in this path
        /// </summary>
        /// <param name="targetDir"></param>
        public void CreateDirectory(string targetDir)
        {
            DirectoryInfo dir = new DirectoryInfo(targetDir);
            if (!dir.Exists)
            {
                //
                dir.Create();
            }
            else
            {
                DeleteFiles(targetDir, true);
            }
        }




        //////////////////////////////////////////////////////////////////////////



    }//public partial class MarketDataPathManagerWindow : Window
}//namespace WPFDispatchMarketData
