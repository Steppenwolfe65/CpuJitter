#include "FileStream.h"

namespace CpuJitter
{

	void FileStream::Close()
	{
		if (_fileStream && _fileStream.is_open())
		{
			_fileStream.flush();
			_fileStream.close();
		}
	}

	void FileStream::Destroy()
	{
		if (!m_isDestroyed)
		{
			_filePosition = 0;
			Close();
			m_isDestroyed = true;
		}
	}

	void FileStream::Flush()
	{
		if (_fileStream)
			_fileStream.flush();
	}

	size_t FileStream::Read(std::vector<byte> &Buffer, size_t Offset, size_t Count)
	{
		if (_fileAccess == FileAccess::Write)
			throw;

		if (Offset + Count > _fileSize - _filePosition)
			Count = _fileSize - _filePosition;

		if (Count > 0)
		{
			// read the data:
			_fileStream.read((char*)&Buffer[Offset], Count);
			_filePosition += Count;
		}

		return Count;
	}

	byte FileStream::ReadByte()
	{
		if (_fileSize - _filePosition < 1)
			throw;
		if (_fileAccess == FileAccess::Write)
			throw;

		byte data(1);

		_fileStream.read((char*)&data, 1);
		_filePosition += 1;
		return data;
	}

	void FileStream::Reset()
	{
		_fileStream.seekg(0, std::ios::beg);
		_filePosition = 0;
	}

	void FileStream::SetLength(size_t Length)
	{
		if (_fileAccess == FileAccess::Read)
			throw;

		_fileStream.seekg(Length - 1, std::ios::beg);
		WriteByte(0);
		_fileStream.seekg(0, std::ios::beg);
	}

	void FileStream::Write(const std::vector<byte> &Buffer, size_t Offset, size_t Count)
	{
		if (_fileAccess == FileAccess::Read)
			throw;

		_fileStream.write((char*)&Buffer[Offset], Count);
		_filePosition += Count;
		_fileSize += Count;
	}

	void FileStream::WriteByte(byte Data)
	{
		if (_fileAccess == FileAccess::Read)
			throw;

		_fileStream.write((char*)&Data, 1);
		_filePosition += 1;
		_fileSize += 1;
	}

	bool FileStream::FileExists(const char* FileName)
	{
		try
		{
			std::ifstream infile(FileName);
			bool valid = infile.good();
			infile.close();
			return valid;
		}
		catch (...)
		{
			return false;
		}
	}

	std::ifstream::pos_type FileStream::FileSize(const char* FileName)
	{
		std::ifstream in(FileName, std::ifstream::ate | std::ifstream::binary);
		size_t size = (size_t)in.tellg();
		in.close();
		return size;
	}
}