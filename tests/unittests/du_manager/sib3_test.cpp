/*
 *
 * Copyright 2021-2025 Software Radio Systems Limited
 *
 * This file is part of srsRAN.
 *
 * srsRAN is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as
 * published by the Free Software Foundation, either version 3 of
 * the License, or (at your option) any later version.
 *
 * srsRAN is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * A copy of the GNU Affero General Public License can be found in
 * the LICENSE file in the top-level directory of this distribution
 * and at http://www.gnu.org/licenses/.
 *
 */

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
  for(auto num_ngbrs=0; num_ngbrs<2; num_ngbrs++)
  {
	intra_frequency_neigh_cell_info neighbor;
    neighbor.nr_pci = make_asn1_rrc_cell_sib3_buffer.neighbor[num_ngbrs].nr_pci;
    neighbor.q_offset_cell = make_asn1_rrc_cell_sib3_buffer.neighbor[num_ngbrs].q_offset_cell;
    sib3.intra_freq_neigh_cell_info.emplace_back(neighbor);
  }
  for(auto num_black_cells=0; num_black_cells<2; num_black_cells++)
  {
	pci_range _pci_range;
    _pci_range.nr_pci_start = make_asn1_rrc_cell_sib3_buffer.pci_range[num_black_cells].nr_pci_start;
	if(make_asn1_rrc_cell_sib3_buffer.pci_range[num_black_cells].range)
      _pci_range.range = make_asn1_rrc_cell_sib3_buffer.pci_range[num_black_cells].range;
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
  asn1::cbit_ref            bref(buf);
  asn1::rrc_nr::sib3_s sib3_decoded;
  asn1::SRSASN_CODE         ret = sib3_decoded.unpack(bref);
  EXPECT_EQ(ret, asn1::SRSASN_SUCCESS);

  // Check that the decoded sib3 matches the sib3 configuration used in the test
  EXPECT_TRUE(sib3_intra_freq_neigh_cell_info_present);
  EXPECT_EQ(sib3_decoded.intra_freq_neigh_cell_info, sib3.intra_freq_neigh_cell_info);
  EXPECT_TRUE(sib3_decoded.intra_freq_black_cell_info_present);
  EXPECT_EQ(sib3_decoded.intra_freq_black_cell_info,
            sib3.intra_freq_black_cell_info);
}
