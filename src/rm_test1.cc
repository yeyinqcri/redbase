/*
 * TODO (yye@) migrate these test cases to gtest in the future.
 * rm_test1.cc
 *
 *  Created on: 2015年11月21日
 *      Author: yye
 *
 */
#include "rm_header.h"

#include <iostream>
#include <vector>
using namespace std;

void TestRMFileHeader_Serialization() {
  RM_FileHeader header;
  header.first_free = 1;
  header.number_records_per_page = 10;
  header.record_size = 100;
  char result[100];
  header.ToBuf(result, 100);
  cout << result << endl;
}

void TestRMPageHeader_Serialization() {
  RM_PageHeader header(10);
  header.SetBusy(0);
  header.SetBusy(5);
  char result[100];
  memset(result, 0, sizeof(char) * 100);
  header.ToBuf(result, 100);
  cout << result << endl;
}

void TestRMFileHeader_DeSerialization() {
  char str[] =
      "{\"firstfree\":1,\"number_records_per_page\":10,\"recordsize\":100}";
  RM_FileHeader header = RM_FileHeader::BufToFileHeader(str);
  cout << "header.firstfree:" << header.first_free << endl;
  cout << "header.number_records_per_page:" << header.number_records_per_page
       << endl;
  cout << "header.recordsize:" << header.record_size << endl;
}

void TestRMPageHeader_DeSerialization() {
  char str[] = "{\"bit_array\":[1,0,0,0,0,1,0,0,0,0],\"next_free\":2}";
  RM_PageHeader header = RM_PageHeader::BufToPageHeader(str);
  cout << "header.bit_array" << endl;
  for (int i = 0; i < header.bit_array.size(); i++) {
    cout << header.bit_array[i] << ",";
  }
  cout << endl;
  cout << "header.next_free" << header.next_free << endl;
}

int main() {
  TestRMFileHeader_Serialization();
  TestRMPageHeader_Serialization();
  TestRMFileHeader_DeSerialization();
  TestRMPageHeader_DeSerialization();
}
