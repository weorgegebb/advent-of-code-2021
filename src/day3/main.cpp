#include <bitset>
#include <cstddef>
#include <fstream>
#include <iostream>
#include <iterator>
#include <numeric>
#include <ostream>
#include <string>
#include <utility>
#include <vector>

namespace {

auto CheckInputIsValid(const int argc, char* argv[]) {
  if (argc != 2) {
    std::cout << "usage: " << argv[0] << " path-to-input" << std::endl;
    return false;
  }
  return true;
}

using ReportEntry = std::bitset<12>;
using Report = std::vector<ReportEntry>;
using GammaRate = ReportEntry;

auto GetInputData(const char* path) {
  std::ifstream input_file(path);
  std::string line;
  Report report;

  while (std::getline(input_file, line)) {
    if (line.size() > 0) {
      report.emplace_back(line);
    }
  }
  return report;
}

auto ReverseBitsetIndex(const size_t num_bits, const size_t index) {
  return num_bits - index - 1;
}

auto MostCommonBitForPosition(const Report& report, const int position) {
  auto num_of_zeros = 0;
  auto num_of_ones = 0;

  for (const auto& entry : report) {
    if (entry.test(ReverseBitsetIndex(entry.size(), position))) {
      ++num_of_ones;
    } else {
      ++num_of_zeros;
    }
  }

  return num_of_ones >= num_of_zeros ? true : false;
}

auto CalculateGammaRate(const Report& report) {
  GammaRate gamma_rate;
  for (auto i = 0; i < report.front().size(); ++i) {
    gamma_rate[i] = MostCommonBitForPosition(report, i);
  }
  return gamma_rate;
}

auto CalculatePowerConsumption(const Report& report) {
  auto gamma_rate = CalculateGammaRate(report);
  auto epsilon_rate = gamma_rate;
  epsilon_rate.flip();

  return gamma_rate.to_ullong() * epsilon_rate.to_ullong();
}

auto ApplyCriteraToReport(
    const Report& report,
    const std::function<Report(const Report&, const int)>& critera_fcn) {
  Report temp_report = report;
  
  for (auto i = 0; i < report.front().size(); ++i) {
    temp_report = critera_fcn(temp_report, i);
    if (temp_report.size() == 1) {
      return temp_report.front();
    }
  }

  return ReportEntry();
}

auto OxygenGeneratonCriterion(const Report& report, const int position) {
  const auto most_common_bit = MostCommonBitForPosition(report, position);
  const auto least_common_bit = !MostCommonBitForPosition(report, position);

  Report return_report;

  for (const auto& entry : report) {
    if (entry[ReverseBitsetIndex(entry.size(), position)] == most_common_bit) {
      return_report.push_back(entry);
    }
  }
  return return_report;
}

auto CalculateCO2ScrubberCriterion(const Report& report, const int position) {
  const auto most_common_bit = MostCommonBitForPosition(report, position);
  const auto least_common_bit = !MostCommonBitForPosition(report, position);

  Report return_report;

  for (const auto& entry : report) {
    if (entry[ReverseBitsetIndex(entry.size(), position)] == least_common_bit) {
      return_report.push_back(entry);
    }
  }
  return return_report;
}

auto CalculateOxygenGeneratorRating(const Report& report) {
  return ApplyCriteraToReport(report, OxygenGeneratonCriterion).to_ullong();
}

auto CalculateCO2ScrubberRating(const Report& report) {
  return ApplyCriteraToReport(report, CalculateCO2ScrubberCriterion)
      .to_ullong();
}

auto CalculateLifeSupportRating(const Report& report) {
  const auto oxygen_generator_rating = CalculateOxygenGeneratorRating(report);
  std::cout << "CalculateOxygenGeneratorRating: " << oxygen_generator_rating
            << std::endl;
  const auto co2_scrubber_rating = CalculateCO2ScrubberRating(report);
  std::cout << "CalculateCO2ScrubberRating: " << co2_scrubber_rating
            << std::endl;
  return oxygen_generator_rating * co2_scrubber_rating;
}

}  // namespace

auto main(int argc, char* argv[]) -> int {
  if (!CheckInputIsValid(argc, argv)) {
    return 0;
  }

  const auto& file_path = argv[1];
  const auto report = GetInputData(file_path);

  // Part 1
  const auto power_comsumption = CalculatePowerConsumption(report);
  std::cout << "Part 1: Power consumption is " << power_comsumption
            << std::endl;

  // Part 2
  const auto life_support_rating = CalculateLifeSupportRating(report);
  std::cout << "Part 2: Life support rating is " << life_support_rating
            << std::endl;

  return 0;
}
