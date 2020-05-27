#pragma once

#include <cstdint>
#include <vector>

namespace cbee
{
// +-------------------+------------------+------------------+
// | prependable bytes |  readable bytes  |  writable bytes  |
// |                   |     (CONTENT)    |                  |
// +-------------------+------------------+------------------+
// |                   |                  |                  |
// 0      <=      readerIndex   <=   writerIndex    <=     size
class Buffer
{
public:
   static const int kCheapPrepend = 8;
   static const int kInitialSize = 1024;

   Buffer();

   int readableBytes() const;
   int writableBytes() const;
   int prependableBytes() const;

   const char* find(const char* needle, int len) const;
   const char* peek() const;

   void clear();
   void erase(int numberOfBytes);
   void append(const char* data, int len);

   void prependInt64(int64_t x);
   void prependInt32(int32_t x);
   void prependInt16(int16_t x);
   void prependInt8(int8_t x);

   int64_t readInt64();
   int32_t readInt32();
   int16_t readInt16();
   int8_t readInt8();

   void writeInt64(int64_t x);
   void writeInt32(int32_t x);
   void writeInt16(int16_t x);
   void writeInt8(int8_t x);

private:
   Buffer(const Buffer&) = delete;
   Buffer& operator=(const Buffer&) = delete;

private:
   std::vector<char> buf;
   int readerIndex;
   int writerIndex;
};

}
