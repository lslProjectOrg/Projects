#ifndef SQLTYPEADAPTER_H
#define SQLTYPEADAPTER_H
#include "core/DataAccess/src/DataAccessCommonData.h"
#include "core/DataAccess/src/SQLStatement.h"

NS_BEGIN(TA_Base_Core)

class SQLTypeAdapter
{
public:
	/// \brief Default constructor; empty string
	SQLTypeAdapter();

	/// \brief Copy ctor
	/// \param other the other SQLTypeAdapter object
	SQLTypeAdapter(const SQLTypeAdapter& other);

	/// \brief Create a copy of a C++ string
	SQLTypeAdapter(const std::string& str);

	/// \brief Create a copy of a null-terminated C string
	SQLTypeAdapter(const char* str);

	/// \brief Create a single-character string
	SQLTypeAdapter(char c);

	/// \brief Create a string representation of a short int value
	SQLTypeAdapter(short i);

	/// \brief Create a string representation of an unsigned
	/// \c short int value
	SQLTypeAdapter(unsigned short i);

	/// \brief Create a string representation of an int value
	SQLTypeAdapter(int i);

	/// \brief Create a string representation of an unsigned int
	/// value
	SQLTypeAdapter(unsigned i);

	/// \brief Create a string representation of a long int value
	SQLTypeAdapter(long i);

	/// \brief Create a string representation of an unsigned
	/// \c long int value
	SQLTypeAdapter(unsigned long i);

	/// \brief Create a string representation of a 64-bit value
	SQLTypeAdapter(long long i);	

	/// \brief Create a string representation of a float value
	SQLTypeAdapter(float i);

	/// \brief Create a string representation of a double value
	SQLTypeAdapter(double i);

	/// \brief Create a string representation of a SQLStatement struct
	///
	/// \see assign(const SQLStatement&) for details
	SQLTypeAdapter(const SQLStatement& rhs);

	/// \brief Standard assignment operator
	///
	/// \see assign(const SQLTypeAdapter&) for details
	SQLTypeAdapter& operator =(const SQLTypeAdapter& rhs);

	/// \brief Return pointer to raw data buffer
	const char* data(int nIndex) const;

	/// \brief Copies another SQLTypeAdapter's data buffer into this
	/// object.
	///
	/// \param sta Other object to copy
	///
	/// \retval *this
	SQLTypeAdapter& assign(const SQLTypeAdapter& sta);	

	/// \brief Returns the character at a given position within the
	/// string buffer.
	char at(size_t i, size_t uIndex) const;

	/// \brief Return number of bytes in data buffer
	size_t length(size_t nIndex) const;
	size_t size(size_t nIndex) const { return length(nIndex); } ///< alias for length()

	bool is_null(size_t nIndex) const;
	bool empty() const;

	/// \brief Compare the internal buffer to the given string
	///
	/// Works just like string::compare(const std::string&).
	int compare(const SQLTypeAdapter& other) const;
	int getDataCategory() const;

	/// \brief clear the internal buffer to the given string
	///
	/// Works just like string::clear(const std::string&).
	void clear();

private:
	void  _CopyData(const SQLTypeAdapter& other);

private:
	/// \brief Converts anything you can insert into a C++ stream to a
	/// \c std::string via \c std::ostringstream.
	template <class T>
	std::string stream2string(const T& object);
private:
	bool m_bEmpty;
	int  m_nDataCategory;
	std::string m_strParams[defSupportDBCount + 1];      // contains 3 items including: common, oracle, mysql
};

NS_END(TA_Base_Core)

#endif /// SQLTYPEADAPTER_H
