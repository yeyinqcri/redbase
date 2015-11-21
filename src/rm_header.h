/*
 * rm_header.h
 *
 *  Created on: 2015年11月16日
 *      Author: yye
 */

#ifndef RM_HEADER_H_
#define RM_HEADER_H_

#include "rm.h"
#include "linkedlist.h"

#include <string>
using namespace std;

//
// Metadata of the records in the file. Must be stored in the first page of the file.
//
class RM_FileHeader {
 public:
  PageNum first_free;
  int record_size;
  int number_records_per_page;
  // This function is used to deserialize FileHeader from string to object.
  static RM_FileHeader BufToFileHeader(const char* array);
  // This function is used to serialize the object.
  bool ToBuf(char* ret_value, const int length);
};

//
// Metadata of the records in the page, Must be stored in the first lines of each page.
//
class RM_PageHeader {
 public:
  vector<bool> bit_array;
  int record_num;
  // This function is used to deserialize FileHeader from string to object.
  static RM_PageHeader BufToPageHeader(const char* header_content);
  // This function is used to serialize the object.
  bool ToBuf(char* ret_value, const int length);
};

#endif /* RM_HEADER_H_ */