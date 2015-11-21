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
  header.first_free = root["firstfree"].asInt();
  header.number_records_per_page = root["number_records_per_page"].asInt();
  header.record_size = root["recordsize"].asInt();
  return header;
}

bool RM_FileHeader::ToBuf(char* buf, const int length) {
  Json::Value obj;
  obj["firstfree"] = this->first_free;
  obj["recordsize"] = this->record_size;
  obj["number_records_per_page"] = this->number_records_per_page;
  Json::Writer writer;
  std::string str = writer.write(obj);
  if (str.length() >= length) {
    return false;
  }
  memcpy(buf, str.c_str(), str.length());
  return true;
}

RM_PageHeader RM_PageHeader::BufToPageHeader(const char* array) {
  Json::Value root;
  Json::Reader reader;
  RM_PageHeader header;
  string str(array);
  bool is_success = reader.parse(str, root, false);
  if (!is_success) {
    return header;
  }
  header.record_num = root["record_num"].asInt();
  for (int i = 0; i < header.record_num; i++) {
    header.bit_array.push_back(root["record"][i].asBool());
  }
}

bool RM_PageHeader::ToBuf(char* buf, const int length) {
  Json::Value obj;
  Json::Value arrayObj;
  obj["record_num"] = this->record_num;
  for (int i = 0; i < this->record_num; i++) {
    arrayObj.append(this->bit_array[i]);
  }
  obj["bit_array"] = arrayObj;
  Json::Writer writer;
  std::string str = writer.write(obj);
  if (str.length() >= length) {
    return false;
  }
  memcpy(buf, str.c_str(), str.length());
  return true;
}
