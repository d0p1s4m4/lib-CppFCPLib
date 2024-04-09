/*
 * Copyright (c) 2024 d0p1 <contact@d0p1.eu>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of mosquitto nor the names of its
 *    contributors may be used to endorse or promote products derived from
 *    this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */
#ifndef FCP_NODE_HPP_
#define FCP_NODE_HPP_

#include <string>
#include <vector>

namespace fcp {

class Node
{
public:
  class Note
  {
  public:
    enum class Type
    {
      Private = 1
    };

    Note();
    virtual ~Note() = default;

  private:
    std::string mText;
    Type mType;
  };

  enum class CompressionCodec
  {
    GZIP,
    BZIP2,
    LZMA,
    LZMA_NEW,
  };

  enum class Trust
  {
    Low,
    Normal,
    High
  };

  enum class Visibility
  {
    No,
    NameOnly,
    Yes
  };

  Node() = default;
  virtual ~Node() = default;

  private:
  std::vector<CompressionCodec> mCompressionCodec;
};

inline std::string
to_string(Node::Trust trust)
{
  switch (trust) {
    case Node::Trust::Low:
      return "LOW";

    case Node::Trust::Normal:
      return "NORMAL";

    case Node::Trust::High:
      return "HIGH";
  }
}

inline std::string
to_string(Node::Visibility visibility)
{
  switch (visibility) {
    case Node::Visibility::No:
      return "NO";
    case Node::Visibility::NameOnly:
      return "NAME_ONLY";
    case Node::Visibility::Yes:
      return "YES";
  }
}

inline std::string
to_string(bool boolean)
{
  if (boolean) {
    return "true";
  }
  return "false";
}

inline std::string
to_string(Node::CompressionCodec codec)
{
  switch (codec) {

    case Node::CompressionCodec::GZIP:
      return "GZIP(0)";
    case Node::CompressionCodec::BZIP2:
      return "BZIP2(1)";
    case Node::CompressionCodec::LZMA:
      return "LZMA(2)";
    case Node::CompressionCodec::LZMA_NEW:
      return "LZMA_NEW(3)";
  }
}

}

#endif // !FCP_NODE_HPP_