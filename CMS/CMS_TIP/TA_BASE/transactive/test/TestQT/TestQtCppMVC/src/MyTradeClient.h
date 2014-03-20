#ifndef __CLASS_MY_TRADE_CLIENT_HH__
#define __CLASS_MY_TRADE_CLIENT_HH__

#include "ProjectCommonData.h"

//qt sys
#include <QtCore/QChar>
#include <QtCore/QString>
#include <QtCore/QDir>
#include <QtCore/QVariant>
#include <QtCore/QMap>

#include "TradeClient.h"

#include "ClientLoginParam.h"

NS_BEGIN(TA_Base_App)

class CProjectUtilityFun;

class CMyTradeClient : public TradeClient
{ 
private:
	CClientLoginParam* m_pClientLoginParam;
	CProjectUtilityFun* m_pUtilityFun;
public:
	CMyTradeClient(const CClientLoginParam& clientLoginParam);
	virtual ~CMyTradeClient(); 
public:
	int logonToServer();
public:
	/// Place an order with an prepared order
	void placeOrder(Order &order);

	/// Cancel an working order with order id
	void cancelOrder(unsigned int orderID);

	/// Subscribe real-time market data for specific instrument
	void subscribeMarketData(const Instrument &instrument);

	/// Subscribe real-time market data by instrument id
	void subscribeMarketData(unsigned int securityID);

	/// Unsubscribe the market data
	void unsubscribeMarketData(unsigned int securityID);

	/// Download history data from server from a time span 
	void downloadHistoryData( const Instrument &instrument, BarType interval, unsigned int from, unsigned int to );

	/// Download the latest N bars and subscribe the following bars, the new bars will be pushed automatically after subscription 
	void downloadHistoryData( const Instrument &instrument, BarType interval, unsigned short number, bool subscribe );

	/// Download N bars since a time point, and subscribe the bar data from the time point when to subscribe. 
	void downloadHistoryData( const Instrument &instrument, BarType interval, unsigned int fromTime, unsigned short count, bool subscribe );

	/// Hook method when receiving instrument information 
	void onInstrumentDownloaded(const Instrument &instrument);

	/// Hook method when account downloaded
	void onAccountDownloaded(Account & account);

	/// Hook method when currency downloaded
	void onCurrencyDownloaded(const Currency &currency);

	/// Hook method when receive market data
	void onMarketDataUpdate(const Instrument &instrument);

	/// Hook method when the order is accepted by exchange.
	void onOrderAccepted(const Order &order);

	/// Hook method when order is canceled.
	void onOrderCanceled(const Order &order);

	/// Hook method when order is rejected.
	void onOrderRejected(const Order &order);

	/// Hook method when order filled
	void onOrderFilled(const Order &order);

	/// Hook method when cancel request get rejected
	void onCancelReject(const Order &order);

	/// Amend order price
	void amendOrderPrice( unsigned int orderID, double newPrice );

	/// Amend order quantity
	void amendOrderVolume( unsigned int orderID, int newVolume );

	// Hook method when order is amended successfully
	void onOrderAmended(const Order &order);

	// Hook method when amend request rejected
	void onAmendRejected(const Order &order);

	/// Hook method when SOD position downloaded
	void onPositionDownloaded(Position &position);

	/// Hook method when position is updated 
	void onPositionChanged(Position &position);

	/// Hook method when history data is downloaded
	void onHistoryDataDownloaded(const Instrument &instrument, const BarArray &barArray);
}; 

NS_END(TA_Base_App)




#endif//__CLASS_MY_TRADE_CLIENT_HH__



