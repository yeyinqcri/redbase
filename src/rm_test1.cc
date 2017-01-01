/*
 * TODO (yye@) migrate these test cases to gtest in the future.
 * rm_test1.cc
 *
 *  Created on: 2015年11月21日
 *      Author: yye
 *
 */
#include "rm.h"

#include <cassert>
#include <iostream>
#include <vector>
using namespace std;

void TestRMFileHeader_Serialization() {
  RM_FileHdr header;
  header.first_free_page_num = 1;
  header.record_num_per_page = 10;
  header.record_size = 100;
  char result[100];
  header.serialize(result);
}

void TestRMPageHeader_Serialization() {
  RM_PageHdr header(10);
  header.nextFree = 0;
  header.numSlots = 0;
  header.numFreeSlots = 0;
  char result[100];
  header.serialize(result);
}

void TestRMFileHeader_DeSerialization() {
  RM_FileHdr header;
  header.first_free_page_num = 1;
  header.record_num_per_page = 10;
  header.record_size = 100;
  char result[100];
  header.serialize(result);
  RM_FileHdr deserialized_hdr = RM_FileHdr::deserialize(result);
  assert(deserialized_hdr.first_free_page_num == header.first_free_page_num);
  assert(deserialized_hdr.record_num_per_page == header.record_num_per_page);
  assert(deserialized_hdr.record_size == header.record_size);
  cout << "FILEHEADER_DESERIALIZATION TESTED!" << endl;
}

void TestRMPageHeader_DeSerialization() {
  RM_PageHdr header(10);
  header.nextFree = 3;
  header.numSlots = 30;
  header.numFreeSlots = 300;
  char result[100];
  header.serialize(result);
  RM_PageHdr deserialized_hdr = RM_PageHdr::deserialize(result, 10);
  assert(deserialized_hdr.nextFree == header.nextFree);
  assert(deserialized_hdr.numSlots == header.numSlots);
  assert(deserialized_hdr.numFreeSlots == header.numFreeSlots);
  cout << "RMPAGEHEADER_DESERIALIZATION TESTED!" << endl;
}

int main() {
  TestRMFileHeader_Serialization();
  TestRMPageHeader_Serialization();
  TestRMFileHeader_DeSerialization();
  TestRMPageHeader_DeSerialization();
}
