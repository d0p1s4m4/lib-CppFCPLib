#ifndef FCP_NODE_HPP_
#define FCP_NODE_HPP_

#include <cstddef>
#include <iterator>
#include <string>

namespace fcp {

class Node
{
public:
  struct Iterator
  {
    using difference_type = std::ptrdiff_t;
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

}

#endif // !FCP_NODE_HPP_