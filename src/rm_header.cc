#include "rm_header.h"

#include <string>

RM_FileHeader RM_FileHeader::BufToFileHeader(const char* array) {
  RM_FileHeader hdr;
  return hdr;
}

bool RM_FileHeader::ToBuf(char* buf, const int length) {
  return true;
}

RM_PageHeader::RM_PageHeader(unsigned int record_num) {
}

RM_PageHeader RM_PageHeader::BufToPageHeader(const char* array) {
  RM_PageHeader hdr(0);
  return hdr;
}

bool RM_PageHeader::ToBuf(char* buf, const int length) {
  return true;
}

int RM_PageHeader::GetFirstFreeSlot() {
  return -1;
}

void RM_PageHeader::SetBusy(int index) {
  bit_array[index] = true;
}

bool RM_PageHeader::IsAllFull() {
  return true;
}

int RM_PageHeader::GetTotalRecordNum() {
  return record_num;
}
