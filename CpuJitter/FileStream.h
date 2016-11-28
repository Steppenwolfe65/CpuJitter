#ifndef _CEXENGINE_FILESTREAM_H
#define _CEXENGINE_FILESTREAM_H

#include <iostream>
#include <fstream>
#include "Config.h"

namespace CpuJitter
{
	/// <summary>
	/// Write data values to a file
	/// </summary>
	class FileStream
	{
	public:
		/// <summary>
		/// File access type flags
		/// </summary>
		enum class FileAccess : int
		{
			Read = std::ios::in,
			ReadWrite = std::ios::out | std::ios::in,
			Write = std::ios::out
		};

		/// <summary>
		/// File operation mode flags
		/// </summary>
		enum class FileMode : int
		{
			Append = std::ios::app,
			AtEnd = std::ios::ate,
			Binary = std::ios::binary,
			Truncate = std::ios::trunc
		};

	private:
		static constexpr uint BLOCK_SIZE = 4096;

		bool m_isDestroyed;
		const char* _filename;
		size_t _filePosition;
		size_t _fileSize;
		std::fstream _fileStream;
		FileAccess _fileAccess;
		FileMode _fileMode;

		FileStream() {}

	public:

		//~~~Properties~~~//

		/// <summary>
		/// Get: The stream can be read
		/// </summary>
		const bool CanRead() { return _fileAccess != FileAccess::Write; }

		/// <summary>
		/// Get: The stream is seekable
		/// </summary>
		const bool CanSeek() { return true; }

		/// <summary>
		/// Get: The stream can be written to
		/// </summary>
		const bool CanWrite() { return _fileAccess != FileAccess::Read; }

		/// <summary>
		/// Get: The stream length
		/// </summary>
		const size_t Length() { return _fileSize; }

		/// <summary>
		/// Get: The streams current position
		/// </summary>
		const size_t Position() { return _filePosition; }

		/// <summary>
		/// Get: The underlying stream
		/// </summary>
		std::fstream &Stream() { return _fileStream; }

		//~~~Constructor~~~//

		/// <summary>
		/// Instantiate this class
		/// </summary>
		///
		/// <param name="FileName">The full path to the file</param>
		/// <param name="Access">The level of access requested</param>
		/// <param name="Mode">The file processing mode</param>
		explicit FileStream(const std::string &FileName, FileAccess Access = FileAccess::ReadWrite, FileMode Mode = FileMode::Binary)
			:
			_fileAccess(Access),
			_fileMode(Mode),
			_filename(0),
			m_isDestroyed(false),
			_filePosition(0),
			_fileSize(0)
		{
			_filename = FileName.c_str();

			if (Access == FileAccess::Read && !FileExists(_filename))
				throw;

			_fileSize = (size_t)FileSize(_filename);

			try
			{
				_fileStream.open(_filename, (int)Access | (int)Mode);
				_fileStream.unsetf(std::ios::skipws);
			}
			catch (std::exception& ex)
			{
				throw;
			}
		}

		//~~~Public Methods~~~//

		/// <summary>
		/// Finalize objects
		/// </summary>
		~FileStream()
		{
			Destroy();
		}

		//~~~Public Methods~~~//

		/// <summary>
		/// Close and flush the stream
		/// </summary>
		void Close();

		/// <summary>
		/// Release all resources associated with the object
		/// </summary>
		void Destroy();

		/// <summary>
		/// Write the stream to disk
		/// </summary>
		void Flush();

		/// <summary>
		/// Reads a portion of the stream into the buffer
		/// </summary>
		///
		/// <param name="Buffer">The output buffer receiving the bytes</param>
		/// <param name="Offset">Offset within the output buffer at which to begin</param>
		/// <param name="Count">The number of bytes to read</param>
		///
		/// <returns>The number of bytes processed</returns>
		size_t Read(std::vector<byte> &Buffer, size_t Offset, size_t Count);

		/// <summary>
		/// Read a single byte from the stream
		/// </summary>
		///
		/// <returns>The byte value</returns>
		/// 
		/// <exception cref="Exception::CryptoProcessingException">Thrown if the stream is too short or the file is write only</exception>
		byte ReadByte();

		/// <summary>
		/// Reset and initialize the underlying stream to zero
		/// </summary>
		void Reset();

		/// <summary>
		/// Set the length of the stream
		/// </summary>
		/// 
		/// <param name="Length">The desired length</param>
		/// 
		/// <exception cref="Exception::CryptoProcessingException">Thrown if the file is read only</exception>
		void SetLength(size_t Length);

		/// <summary>
		/// Writes a buffer into the stream
		/// </summary>
		///
		/// <param name="Buffer">The output buffer to write to the stream</param>
		/// <param name="Offset">Offset within the output buffer at which to begin</param>
		/// <param name="Count">The number of bytes to write</param>
		///
		/// <returns>The number of bytes processed</returns>
		/// 
		/// <exception cref="Exception::CryptoProcessingException">Thrown if the file is read only</exception>
		void Write(const std::vector<byte> &Buffer, size_t Offset, size_t Count);

		/// <summary>
		/// Write a single byte from the stream
		/// </summary>
		///
		/// <returns>The byte value</returns>
		/// 
		/// <exception cref="Exception::CryptoProcessingException">Thrown if the file is read only</exception>
		void WriteByte(byte Data);

	private:
		bool FileExists(const char* FileName);

		std::ifstream::pos_type FileSize(const char* FileName);
	};

}
#endif