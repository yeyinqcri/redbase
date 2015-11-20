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

void RM_FileHeader::ToBuf(char* buf) {
  Json::Value obj;
  obj["firstfree"] = this->first_free;
  obj["recordsize"] = this->record_size;
  obj["number_records_per_page"] = this->number_records_per_page;
  Json::StyledWriter writer;
  std::string str = writer.write(obj);
  memcpy(buf, str.c_str(), str.length() + 1);
}
