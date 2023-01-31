#pragma once
#include <iostream>
#include <istream>
#include <memory>
#include <ostream>

namespace FPGA_BASE {
struct IstreamDeletor {
  void operator()(std::istream *IstreamPtr) const;
};
struct OstreamDeletor {
  void operator()(std::ostream *OstreamPtr) const;
};

struct InputStreamCreator {
  std::unique_ptr<std::istream, IstreamDeletor>
  createInputStream(int argc, char **argv) const;
};
struct OutputStreamCreator {
  std::unique_ptr<std::ostream, OstreamDeletor>
  createOutputStream(int argc, char **argv) const;
};
} // namespace FPGA_BASE