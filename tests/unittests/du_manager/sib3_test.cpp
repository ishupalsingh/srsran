#include "lib/du/du_high/du_manager/converters/asn1_sys_info_packer.h"
#include "srsran/asn1/rrc_nr/sys_info.h"
#include "srsran/ran/sib/system_info_config.h"
#include <gtest/gtest.h>
#include <string>

using namespace srsran;
using namespace srs_du;

TEST(srs_sib3_test, make_asn1_rrc_cell_sib3_buffer)
{
  sib3_info sib3;
  // Create two neighbors with example values
  for (int i = 0; i < 2; ++i) {
    intra_frequency_neigh_cell_info neighbor;
    neighbor.nr_pci = 100 + i;
    neighbor.q_offset_cell = i; // Example values
    sib3.intra_freq_neigh_cell_info.emplace_back(neighbor);
  }
  // Create two black cells with example values
  for (int i = 0; i < 2; ++i) {
    pci_range _pci_range;
    _pci_range.nr_pci_start = 200 + i;
    _pci_range.range = i + 1; // Example range
    sib3.intra_freq_black_cell_info.emplace_back(_pci_range);
  }

  // Call the function being tested
  std::string js_str;
  auto buf = asn1_packer::pack_sib3(sib3, &js_str);

  // Check that the buffer is not empty
  EXPECT_FALSE(buf.empty());

  // Check that the JSON string is not empty
  EXPECT_FALSE(js_str.empty());

  // Decode the buffer to verify its contents
  asn1::cbit_ref bref(buf);
  asn1::rrc_nr::sib3_s sib3_decoded;
  asn1::SRSASN_CODE ret = sib3_decoded.unpack(bref);
  EXPECT_EQ(ret, asn1::SRSASN_SUCCESS);

  // Check that the decoded sib3 matches the sib3 configuration used in the test
  EXPECT_FALSE(sib3_decoded.intra_freq_neigh_cell_list.empty());
  EXPECT_EQ(sib3_decoded.intra_freq_neigh_cell_list.size(), sib3.intra_freq_neigh_cell_info.size());
  EXPECT_FALSE(sib3_decoded.intra_freq_excluded_cell_list.empty());
  EXPECT_EQ(sib3_decoded.intra_freq_excluded_cell_list.size(), sib3.intra_freq_black_cell_info.size());
}
