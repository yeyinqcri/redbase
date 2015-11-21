/*
 * rm_test1.cc
 *
 *  Created on: 2015年11月21日
 *      Author: yye
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

}

void TestRMPageHeader_DeSerialization() {

}

int main() {
  TestRMFileHeader_Serialization();
  TestRMPageHeader_Serialization();
  TestRMFileHeader_DeSerialization();
  TestRMPageHeader_DeSerialization();
}
