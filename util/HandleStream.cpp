/**
 * Copyright 2002 - 2023 Caphyon LTD.
 *
 */

#include "stdafx.h"
#include "HandleStream.h"

HandleStream::HandleStream(HANDLE aHandle)
  : std::iostream(&mHandleStreamBuf)
  , mHandleStreamBuf(aHandle)
{
}

HandleStreamBuf::HandleStreamBuf(HANDLE pipeHandle)
  : mFileHandle(pipeHandle)
{
  mBuffer.resize(100);
  Reset();
}

streamsize HandleStreamBuf::xsgetn(char * _Ptr, streamsize _Count)
{
  auto ret = std::streambuf::xsgetn(_Ptr, _Count);

  if (ret == _Count && _Gnavail() == 0)
  {
    // all info read, reset
    Reset();
  }

  return ret;
}

HandleStreamBuf::int_type HandleStreamBuf::overflow(int_type c)
{
  auto space = _Pnavail();

  if (!space)
  {
    auto alreadyUsed = pptr() - pbase();
    mBuffer.resize(mBuffer.size() * 2);
    mBuffer[alreadyUsed] = (char)c;

    Reset((int)alreadyUsed + 1);
  }
  return int_type();
}

HandleStreamBuf::int_type HandleStreamBuf::underflow()
{
  DWORD cbBytesRead;
  // static_assert(std::is_trivially_copyable_v<std::wstring &>);
  //  in case this is a pipe (we are a server) need to wait for connection before blocking in read
  //  to avoid some race conditions
  ConnectNamedPipe(mFileHandle, nullptr);

  while (true)
  {
    BOOL fSuccess = ReadFile(mFileHandle,          // handle to pipe
                             pptr(),               // buffer to receive data
                             (DWORD)(_Pnavail()),  // size of buffer
                             &cbBytesRead,         // number of bytes read
                             nullptr);             // not overlapped I/O

    if (!fSuccess || cbBytesRead == 0)
    {
      if (cbBytesRead != 0)
      {
        auto alreadyUsed = pptr() - pbase() + cbBytesRead;
        mBuffer.resize(mBuffer.size() * 2);
        Reset((int)alreadyUsed);
      }
      else
        return traits_type::eof();
    }
    else
      break;
  }

  unsigned char ret = *pptr();

  pbump(cbBytesRead);             // update put zone
  setg(eback(), gptr(), pptr());  // update get zone

  return ret;
}

int HandleStreamBuf::sync()
{
  // synchronize with external agent
  DWORD bytesToWriteCount = (DWORD)(pptr() - pbase());

  DWORD bytesWritten;
  BOOL  flg = WriteFile(mFileHandle, pbase(), bytesToWriteCount, &bytesWritten, nullptr);

  if (!flg || bytesWritten != bytesToWriteCount)
    return traits_type::eof();

  pbump((int)(pbase() - pptr()));
  return ERROR_SUCCESS;
}

void HandleStreamBuf::Reset(int aAlreadyUsed /*= 0*/)
{
  if (aAlreadyUsed)
    std::streambuf::setp(mBuffer.data(), mBuffer.data() + aAlreadyUsed,
                         mBuffer.data() + mBuffer.size());
  else
    std::streambuf::setp(mBuffer.data(), mBuffer.data() + mBuffer.size());
  std::streambuf::setg(mBuffer.data(), mBuffer.data(), mBuffer.data());
}
