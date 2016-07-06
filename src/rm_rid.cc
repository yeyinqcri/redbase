#include "rm_rid.h"

RID::RID() {
  this->page_num_ = kInvalidPageNum;
  this->slot_num_ = kInvalidSlotNum;
}

RID::RID(const RID& rid) {
  this->page_num_ = rid.page_num_;
  this->slot_num_ = rid.slot_num_;
}

// Override = operator.
RID& RID::operator=(const RID &rid) {
  if (this != &rid) {
    // Just copy the members since there is no memory allocation involved
    this->page_num_ = rid.page_num_;
    this->slot_num_= rid.slot_num_;
  }
  // Return a reference to this
  return (*this);
}

RID::RID(PageNum pageNum, SlotNum slotNum) {
  this->page_num_ = pageNum;
  this->slot_num_ = slotNum;
}

RID::~RID() {
}

RC RID::GetPageNum(PageNum &pageNum) const {
  if (this->page_num_ == kInvalidPageNum) {
    return RM_INVALIDPAGE;
  }
  pageNum = this->page_num_;
  return OK_RC;
}

RC RID::GetSlotNum(SlotNum &slotNum) const {
  if (this->slot_num_ == kInvalidSlotNum) {
    return RM_INVALIDSLOT;
  }
  slotNum = this->slot_num_;
  return OK_RC;
}
