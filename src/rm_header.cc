#include "rm_header.h"

#include <json/json.h>
#include <string>

RM_FileHeader RM_FileHeader::BufToFileHeader(const char* array) {
  Json::Value root;
  Json::Reader reader;
  RM_FileHeader header;
  string str(array);
  bool is_success = reader.parse(str, root, false);
  if (!is_success) {
    return header;
  }
  header.first_free = root["firstfree"].asInt64();
  header.number_records_per_page = root["number_records_per_page"].asInt();
  header.record_size = root["recordsize"].asInt();
  return header;
}

bool RM_FileHeader::ToBuf(char* buf, const int length) {
  Json::Value obj;
  obj["firstfree"] = static_cast<long long int>(this->first_free);
  obj["recordsize"] = this->record_size;
  obj["number_records_per_page"] = this->number_records_per_page;
  Json::FastWriter writer;
  std::string str = writer.write(obj);
  if (str.length() >= length) {
    return false;
  }
  memcpy(buf, str.c_str(), str.length());
  return true;
}

RM_PageHeader::RM_PageHeader(unsigned int record_num) {
  this->bit_array.clear();
  for (int i = 0; i < record_num; i++) {
    bit_array.push_back(false);
  }
  this->next_free = kInvalidPageNum;
}

RM_PageHeader RM_PageHeader::BufToPageHeader(const char* array) {
  Json::Value root;
  Json::Reader reader;

  string str(array);
  bool is_success = reader.parse(str, root, false);
  if (!is_success) {
    return RM_PageHeader(0);
  }
  int total_size = root["bit_array"].size();
  RM_PageHeader header(total_size);
  for (int i = 0; i < total_size; i++) {
    header.bit_array.push_back(root["bit_array"][i].asBool());
  }
  header.next_free = root["next_free"].asInt64();
  return header;
}

bool RM_PageHeader::ToBuf(char* buf, const int length) {
  Json::Value obj;
  Json::Value arrayObj;
  for (int i = 0; i < bit_array.size(); i++) {
    arrayObj.append(Json::Value(static_cast<int>(this->bit_array[i])));
  }
  obj["bit_array"] = arrayObj;
  obj["next_free"] = static_cast<long long int>(this->next_free);
  Json::FastWriter writer;
  std::string str = writer.write(obj);
  if (str.length() >= length) {
    return false;
  }
  memcpy(buf, str.c_str(), str.length());
  return true;
}

int RM_PageHeader::GetFirstFreeSlot() {
  for (int i = 0; i < bit_array.size(); i++) {
    if (bit_array[i] == false) {
      return i;
    }
  }
  return -1;
}

void RM_PageHeader::SetBusy(int index) {
  bit_array[index] = true;
}

bool RM_PageHeader::IsAllFull() {
  for (int i = 0; i < this->bit_array.size(); i++) {
    if (bit_array[i] == false) {
      return false;
    }
  }
  return true;
}

int RM_PageHeader::GetTotalRecordNum() {
  return bit_array.size();
}
