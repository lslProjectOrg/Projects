#include "MyTradeClient.h"

#include "ProjectUtilityFun.h"

#include "BoostLogger.h"
USING_BOOST_LOG;

NS_BEGIN(TA_Base_App)

CMyTradeClient::CMyTradeClient( const CClientLoginParam& clientLoginParam )
:TradeClient(clientLoginParam.m_strUserName.toStdString(),
			 clientLoginParam.m_strPassWord.toStdString(),
			 clientLoginParam.m_bEnableDebug)
{
	m_pClientLoginParam = NULL;
	m_pClientLoginParam = new CClientLoginParam();
	*m_pClientLoginParam = clientLoginParam;


	m_pUtilityFun = NULL;
	m_pUtilityFun = new CProjectUtilityFun();

}

CMyTradeClient::~CMyTradeClient()
{

	if (NULL != m_pUtilityFun)
	{
		delete m_pUtilityFun;
		m_pUtilityFun = NULL;
	}

	if (NULL != m_pClientLoginParam)
	{
		delete m_pClientLoginParam;
		m_pClientLoginParam = NULL;
	}



}

//////////////////////////////////////////////////////////////////////////

int CMyTradeClient::logonToServer()
{
	int nFunRes = 0;

	nFunRes = this->logon(m_pClientLoginParam->m_strServerIP.toStdString(),
		m_pClientLoginParam->m_nServerPort,
		m_pClientLoginParam->m_bSynchronous);
	
	return nFunRes;
}


//////////////////////////////////////////////////////////////////////////


void CMyTradeClient::placeOrder( Order &order )
{
	int nFunRes = 0;

	LOG_DEBUG<<"placeOrder"<<" ";
	//return nFunRes;
}

void CMyTradeClient::cancelOrder( unsigned int orderID )
{
	int nFunRes = 0;
	LOG_DEBUG<<"cancelOrder"<<" ";
	//return nFunRes;
}


void CMyTradeClient::subscribeMarketData( const Instrument &instrument )
{
	int nFunRes = 0;
	LOG_DEBUG<<"subscribeMarketData( const Instrument &instrument )"<<" ";

	//return nFunRes;
}

void CMyTradeClient::subscribeMarketData( unsigned int securityID )
{
	int nFunRes = 0;
	LOG_DEBUG<<"subscribeMarketData( unsigned int securityID )"<<" ";

	//return nFunRes;
}

void CMyTradeClient::unsubscribeMarketData( unsigned int securityID )
{
	int nFunRes = 0;
	LOG_DEBUG<<"unsubscribeMarketData( unsigned int securityID )"<<" ";

	//return nFunRes;
}

void CMyTradeClient::downloadHistoryData( const Instrument &instrument, BarType interval, unsigned int from, unsigned int to )
{
	int nFunRes = 0;
	LOG_DEBUG<<"downloadHistoryData( const Instrument &instrument, BarType interval, unsigned int from, unsigned int to )"<<" ";

	//return nFunRes;
}

void CMyTradeClient::downloadHistoryData( const Instrument &instrument, BarType interval, unsigned short number, bool subscribe )
{
	int nFunRes = 0;
	LOG_DEBUG<<"downloadHistoryData( const Instrument &instrument, BarType interval, unsigned short number, bool subscribe )"<<" ";

	//return nFunRes;
}

void CMyTradeClient::downloadHistoryData( const Instrument &instrument, BarType interval, unsigned int fromTime, unsigned short count, bool subscribe )
{
	int nFunRes = 0;
	LOG_DEBUG<<"downloadHistoryData( const Instrument &instrument, BarType interval, unsigned int fromTime, unsigned short count, bool subscribe )"<<" ";
	//return nFunRes;
}

void CMyTradeClient::onInstrumentDownloaded( const Instrument &instrument )
{
	BOOST_LOG_FUNCTION();
	int nFunRes = 0;
	std::string strLogInfo;
	strLogInfo = "onInstrumentDownloaded";
	m_pUtilityFun->log_Instrument_info(strLogInfo, instrument);

	//LOG_DEBUG<<"onInstrumentDownloaded( const Instrument &instrument )";
	//return nFunRes;
}

void CMyTradeClient::onAccountDownloaded( Account & account )
{
	int nFunRes = 0;
	//return nFunRes;
	LOG_DEBUG<<"onAccountDownloaded( Account & account )";

}

void CMyTradeClient::onCurrencyDownloaded( const Currency &currency )
{
	int nFunRes = 0;
	//return nFunRes;
	LOG_DEBUG<<"onCurrencyDownloaded( const Currency &currency )";

}

void CMyTradeClient::onMarketDataUpdate( const Instrument &instrument )
{
	int nFunRes = 0;
	//return nFunRes;
	LOG_DEBUG<<"onMarketDataUpdate( const Instrument &instrument )";

}

void CMyTradeClient::onOrderAccepted( const Order &order )
{
	int nFunRes = 0;
	//return nFunRes;
	LOG_DEBUG<<"onOrderAccepted( const Order &order )";

}

void CMyTradeClient::onOrderCanceled( const Order &order )
{
	int nFunRes = 0;
	//return nFunRes;
	LOG_DEBUG<<"onOrderCanceled( const Order &order )";

}

void CMyTradeClient::onOrderRejected( const Order &order )
{
	int nFunRes = 0;
	//return nFunRes;
	LOG_DEBUG<<"onOrderRejected( const Order &order )";


}	

void CMyTradeClient::onOrderFilled( const Order &order )
{
	int nFunRes = 0;
	//return nFunRes;
	LOG_DEBUG<<"onOrderFilled( const Order &order )";

}

void CMyTradeClient::onCancelReject( const Order &order )
{
	int nFunRes = 0;
	//return nFunRes;
	LOG_DEBUG<<"onCancelReject( const Order &order )";

}

void CMyTradeClient::amendOrderPrice( unsigned int orderID, double newPrice )
{
	int nFunRes = 0;
	//return nFunRes;
	LOG_DEBUG<<"amendOrderPrice( const Order &order )";

}

void CMyTradeClient::amendOrderVolume( unsigned int orderID, int newVolume )
{
	int nFunRes = 0;
	//return nFunRes;
	LOG_DEBUG<<"amendOrderVolume( const Order &order )";

}

void CMyTradeClient::onOrderAmended( const Order &order )
{
	int nFunRes = 0;
	//return nFunRes;
	LOG_DEBUG<<"onOrderAmended( const Order &order )";

}

void CMyTradeClient::onAmendRejected( const Order &order )
{
	int nFunRes = 0;
	//return nFunRes;
	LOG_DEBUG<<"onAmendRejected( const Order &order )";

}

void CMyTradeClient::onPositionDownloaded( Position &position )
{
	int nFunRes = 0;
	//return nFunRes;
	LOG_DEBUG<<"onPositionDownloaded";

}

void CMyTradeClient::onPositionChanged( Position &position )
{
	int nFunRes = 0;
	//return nFunRes;
	LOG_DEBUG<<"onPositionChanged";

}

void CMyTradeClient::onHistoryDataDownloaded( const Instrument &instrument, const BarArray &barArray )
{
	int nFunRes = 0;
	//return nFunRes;
	LOG_DEBUG<<"onHistoryDataDownloaded";

}

NS_END(TA_Base_App)












