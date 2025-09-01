/**
 * Copyright 2002 - 2025 Caphyon LTD.
 *
 */

#ifndef _HANDLE_STREAM_
#define _HANDLE_STREAM_

/*
 * Class for 2 way, 1 to 1, communication between 2 processes over std streams
 * - For server side the received handle must be a pipe.
 * - For client side the received handle should be a file but the standard fstream can be used
 * instead.
 */
class HandleStreamBuf : public std::streambuf
{
public:
  HandleStreamBuf(HANDLE pipeHandle);

protected:
  streamsize xsgetn(char * _Ptr, streamsize _Count) override;

  int_type __CLR_OR_THIS_CALL overflow(
    int_type c) override;  // Appends the to the put area of the buffer, reallocating if possible.

  int_type underflow() override;

  int sync() override;

private:
  HANDLE            mFileHandle;
  std::vector<char> mBuffer;  // we need enough buffer size since ReadFile will fail with
                              // ERROR_BROKEN_PIPE if it is to small

  void Reset(int aAlreadyUsed = 0);
};

class HandleStream : public std::iostream
{
public:
  HandleStream(HANDLE aPipeHandle);

private:
  HandleStreamBuf mHandleStreamBuf;
};

struct Serialization
{
  template <class T,
            typename = std::enable_if_t<std::is_trivially_copyable_v<T> && !is_pointer_v<T>>>
  static bool Serialize(const T aVar, std::ostream & aOutputStream)
  {
    aOutputStream.write(reinterpret_cast<const char *>(&aVar), sizeof(T));
    return aOutputStream.good();
  }

  template <class T,
            typename = std::enable_if_t<std::is_trivially_copyable_v<T> && !is_pointer_v<T>>>
  static bool Deserialize(T & aVar, std::istream & aInputStream)
  {
    aInputStream.read(reinterpret_cast<char *>(&aVar), sizeof(T));
    return aInputStream.good();
  }

  static bool Deserialize(std::wstring & aWstring, std::istream & aInputStream)
  {
    UINT size = 0;
    if (!Deserialize(size, aInputStream))
      return false;

    if (size > 0)
    {
      aWstring.clear();
      aWstring.resize(size);  // include the null character

      aInputStream.read(reinterpret_cast<char *>(&aWstring[0]), sizeof(wchar_t) * size);
      return aInputStream.good();
    }

    return true;
  }
};

#endif  // _HANDLE_STREAM_
