//
// rm.h
//
//   Record Manager component interface
//
// This file does not include the interface for the RID class.  This is
// found in rm_rid.h
//

#ifndef RM_H
#define RM_H

// Please DO NOT include any files other than redbase.h and pf.h in this
// file.  When you submit your code, the test program will be compiled
// with your rm.h and your redbase.h, along with the standard pf.h that
// was given to you.  Your rm.h, your redbase.h, and the standard pf.h
// should therefore be self-contained (i.e., should not depend upon
// declarations in any other file).

// Do not change the following includes
#include "redbase.h"
#include "rm_rid.h"
#include "pf.h"

#define RM_PAGE_FILE_HANDLE_CLOSED_WARN (START_RM_WARN + 1)
#define RM_PAGE_FILE_SIZE_EXCEED (START_RM_WARN + 2)

#define RM_FAILED_CLOSE_FILE (START_RM_ERR - 1)

const int RM_HEADER_SIZE = 1024;

//
// RM_Record: RM Record interface
//
class RM_Record {
 public:
  RM_Record();
  ~RM_Record();

  // Return the data corresponding to the record.  Sets *pData to the
  // record contents.
  RC GetData(char *&pData) const;

  // Return the RID associated with the record
  RC GetRid(RID &rid) const;

  // Set the properties inside the class.
  void Set(const char* data, const int record_szie, const RID rid);
 private:
  char* data_;
  int length;
  RID rid;
};

// The metadata of the file, including:
// The page number of the first free page.
//
class RM_FileHdr {
  public:
    void serialize(char* data);
    static RM_FileHdr deserialize(char* data);
    int record_num_per_page;
    int record_size;
    int first_free_page_num;
};

//
// RM_FileHandle: RM File interface
//
class RM_FileHandle {
 public:
  RM_FileHandle();
  ~RM_FileHandle();

  // Given a RID, return the record
  RC GetRec(const RID &rid, RM_Record &rec) const;

  RC InsertRec(const char *pData, RID &rid);       // Insert a new record

  RC DeleteRec(const RID &rid);                    // Delete a record
  RC UpdateRec(const RM_Record &rec);              // Update a record

  // Forces a page (along with any contents stored in this class)
  // from the buffer pool to disk.  Default value forces all pages.
  RC ForcePages(PageNum pageNum = ALL_PAGES);
  void SetPFFileHandle(PF_FileHandle& handle);
  bool GetHandleSet();
  RC GetPFHandle(PF_FileHandle& handle);
 private:
  bool handle_set;
  // The number of records each page stores.
  int record_num_per_page;
  // The nubmer of byte each record uses.
  int record_size;
  // The metadata of the file.
  RM_FileHdr file_header_;

  PF_FileHandle handle_;
  RC IsValid() const;
};

//
// RM_FileScan: condition-based scan of records in the file
//
class RM_FileScan {
 public:
  RM_FileScan();
  ~RM_FileScan();

  RC OpenScan(const RM_FileHandle &fileHandle, AttrType attrType,
              int attrLength, int attrOffset, CompOp compOp, void *value,
              ClientHint pinHint = NO_HINT);  // Initialize a file scan
  RC GetNextRec(RM_Record &rec);               // Get next matching record
  RC CloseScan();                             // Close the scan
};

//
// RM_Manager: provides RM file management
//
class RM_Manager {
 public:
  RM_Manager(PF_Manager &pfm);
  ~RM_Manager();

  RC CreateFile(const char *fileName, int recordSize);
  RC DestroyFile(const char *fileName);
  RC OpenFile(const char *fileName, RM_FileHandle &fileHandle);

  RC CloseFile(RM_FileHandle &fileHandle);
 private:
  PF_Manager* pf_manager_;
};

class Bitmap {
 public:
  Bitmap(int numBits);
  Bitmap(const char* buf, int numBits);
  ~Bitmap();
  // returns how many bits are in this bigmap.
  int GetSize();
  // returns how many bytes the bit array take.
  int GetSizeInBytes();
  // check whether the bit is set to 1 or not. 
  bool Test(int bitNumber);
  // set the value of the corresponding element in the array.
  void Set(int bitNumber, bool value);
  // returns the bitarray.
  char*& GetBitArray();
 private:
  int size;
  char* bitArray;
};

class RM_PageHdr {
 public:
  RM_PageHdr(int numSlots);
  ~RM_PageHdr();

  int nextFree;
  Bitmap* availableSlotMap;
  int numSlots;
  int numFreeSlots;

  // This method is used to calculate how many bytes we need in order to store
  // this header object.
  int size();
  // Serializes this object into a char array.
  void serialize(char* data);
  static RM_PageHdr deserialize(char* data, int numSlots);
};

//
// Print-error function
//
void RM_PrintError(RC rc);

#endif
