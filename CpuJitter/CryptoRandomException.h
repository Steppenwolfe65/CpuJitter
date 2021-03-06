#ifndef _CEXENGINE_CRYPTORANDOMEXCEPTION_H
#define _CEXENGINE_CRYPTORANDOMEXCEPTION_H

#include "Config.h"

namespace CpuJitter
{

	/// <summary>
	/// Wraps exceptions thrown within Pseudo Random Number Generator operations
	/// </summary>
	struct CryptoRandomException : std::exception
	{
	private:
		std::string m_origin;
		std::string m_message;
		std::string m_details;

	public:

		/// <summary>
		/// Get/Set: The inner exception string
		/// </summary>
		std::string &Details() { return m_details; }

		/// <summary>
		/// Get/Set: The message associated with the error
		/// </summary>
		std::string &Message() { return m_message; }

		/// <summary>
		/// Get/Set: The origin of the exception in the format Class
		/// </summary>
		std::string &Origin() { return m_origin; }


		/// <summary>
		/// Instantiate this class with a message
		/// </summary>
		///
		/// <param name="Message">A custom message or error data</param>
		explicit CryptoRandomException(const std::string &Message)
			:
			m_message(Message)
		{
		}

		/// <summary>
		/// Instantiate this class with an origin and message
		/// </summary>
		///
		/// <param name="Origin">The origin of the exception</param>
		/// <param name="Message">A custom message or error data</param>
		explicit CryptoRandomException(const std::string &Origin, const std::string &Message)
			:
			m_message(Message),
			m_origin(Origin)
		{
		}

		/// <summary>
		/// Instantiate this class with an origin, message and inner exception
		/// </summary>
		///
		/// <param name="Origin">The origin of the exception</param>
		/// <param name="Message">A custom message or error data</param>
		/// <param name="Detail">The inner exception string</param>
		explicit CryptoRandomException(const std::string &Origin, const std::string &Message, const std::string &Detail)
			:
			m_details(Detail),
			m_message(Message),
			m_origin(Origin)
		{
		}
	};
}

#endif