#include "comparator.h"
#include "rm.h"
#include "rm_error.h"

#include <memory>

RM_FileScan::RM_FileScan() {
  // TODO(yye): do nothing now.
}

RM_FileScan::~RM_FileScan() {
  // TODO(yye): do nothing now.
}

RC RM_FileScan::OpenScan(const RM_FileHandle &fileHandler, AttrType attrType,
    int attrLength, int attrOffset, CompOp compOp, void* value,
    ClientHint pinHint) {
  if (opened_) {
    return RM_FILE_SCAN_OPENED;
  }
  handle_ = fileHandler;
  type_ = attrType;
  attr_length_ = attrLength;
  attr_offset_ = attrOffset;
  comp_op_ = compOp;
  value_ = value;
  hint_ = pinHint;
  curr_page_num_ = 0;
  curr_slot_num_ = handle_.file_header_.record_num_per_page;
  return OK_RC;
}

RC RM_FileScan::GetNextRec(RM_Record& rec) {
  if (!opened_) {
    return RM_FILE_SCAN_NOT_OPENED;
  }
  while (curr_page_num_ != handle_.file_header_.last_page_num &&
         curr_slot_num_ != handle_.file_header_.record_num_per_page) {
    if (curr_slot_num_ >= handle_.file_header_.record_num_per_page) {
      curr_page_num_++;
      curr_slot_num_ = 0;
      continue;
    }
    RID rid(curr_page_num_, curr_slot_num_);
    RM_Record record_content;
    RC ret_val;
    ret_val = handle_.GetRec(rid, record_content);
    if (ret_val != OK_RC) {
      // Moves to the next one
      curr_slot_num_++;
      continue;
    }
    char* raw_data;
    record_content.GetData(raw_data);
    std::unique_ptr<Comparator> comp;
    switch (type_) {
      case FLOAT:
        comp.reset(new FloatComparator(raw_data + this->attr_offset_,
              this->value_,
              this->attr_length_,
              this->comp_op_));
        break;
      case INT:
        comp.reset(new IntComparator(raw_data + this->attr_offset_,
              this->value_,
              this->attr_length_,
              this->comp_op_));
        break;
      case STRING:
        comp.reset(new StringComparator(raw_data + this->attr_offset_,
              this->value_,
              this->attr_length_,
              this->comp_op_));
        break;
      default:
        return RM_UNKNOWN_TYPE;
    }
    curr_slot_num_ ++;
    if (comp && comp->eval()) {
      rec = record_content;
      return OK_RC;
    }
  }
  return RM_EOF;
}

RC RM_FileScan::CloseScan() {
  opened_ = false;
  return OK_RC;
}
