#ifndef RENTRAK_GENERIC_DATASTORE
#define RENTRAK_GENERIC_DATASTORE

#include <cstdio>
#include <string.h>
#include <unistd.h>

template <typename T>
class GenericDataStore {
    private:
        char          FileName[100];
        FILE mutable *fp;
        bool mutable  bIsOpen;

    public:
        //~~~~~ function default constructor ~~~~~~~~~~~~~~~~~~
        // initializes FileName to ""
        // arguments: null
        // null
        // returns null
        GenericDataStore() {
            bIsOpen = false;
            strcpy(FileName,"");
        }
        //~~~~~~ end of function default constructor~~~~~~~~~~~~~~~~~


        //~~~~~ function overloaded constructor ~~~~~~~~~~~~~~~~~~
        // initializes filename
        // arguments: const char*
        // copied to file name
        // returns null
        GenericDataStore(const char* fnm) {
            bIsOpen = false;
            strcpy(FileName,fnm);
        }
        //~~~~~~ end of function oveloaded constructor~~~~~~~~~~~~~~~~~


        //~~~~~ function SetFileName ~~~~~~~~~~~~~~~~~~
        // sets a new file name
        // arguments: const char*
        // arg is copied to FileName
        // returns void
        void SetFileName(const char* fnm) {
            strcpy(FileName,fnm);
        }
        //~~~~~~ end of function SetFileName~~~~~~~~~~~~~~~~~

        //~~~~~ function IsOpen ~~~~~~~~~~~~~~~~~~
        // Returns true if file is open.
        // returns bool
        bool IsOpen() const {
            return bIsOpen;
        }

        //~~~~~ function Open ~~~~~~~~~~~~~~~~~~
        // opens a file in read/write binary mode
        // arguments: null
        // null
        // returns true if file open succeeds
        bool Open() const {
            if(bIsOpen == false) {
                fp = fopen(FileName,"rb+");
                if(fp == NULL) {
                    fp = fopen(FileName,"wb+");
                }
                if(fp == NULL) {
                    bIsOpen = false;
                    return false;
                }
                bIsOpen = true;
                return true;
            } else {
                    return true;
            }
        }
        //~~~~~~ end of function Open~~~~~~~~~~~~~~~~~

        //~~~~~ function AddRecord ~~~~~~~~~~~~~~~~~~
        // appends a record to the file
        // arguments: T
        // arg is the variable of an unspecified class
        // returns true on success
        bool AddRecord(T t) {
            fseek(fp,0,SEEK_END);
            if(fwrite((char*)&t,sizeof(T),1,fp)) {
                return true;
            }else {
                return false;
            }
        }
        //~~~~~~ end of function AddRecord~~~~~~~~~~~~~~~~~

        //~~~~~ function FindRecord ~~~~~~~~~~~~~~~~~~
        // searches for a record in the file
        // arguments: unsigned long
        // arg is the index position of the record in file
        // returns record if found
        T FindRecord(unsigned long index) const {
            rewind(fp);
            unsigned long pos = index * sizeof(T);
            fseek(fp,pos,SEEK_CUR);
            T t;
            fread((char*)&t,sizeof(T),1,fp);
            return t;
        }
        //~~~~~~ end of function FindRecord~~~~~~~~~~~~~~~~~

        //~~~~~ function ModifyRecord ~~~~~~~~~~~~~~~~~~
        // modifies a record in file
        // arguments: T,unsigned long
        // write the first arg to position specified by 2nd arg
        // returns true on success
        bool ModifyRecord(T newT,unsigned long index) {
            rewind(fp);
            unsigned long pos = index * sizeof(T);
            fseek(fp,pos,SEEK_CUR);
            if(fwrite((char*)&newT,sizeof(T),1,fp)) {
                return true;
            }else {
                return false;
            }
        }
        //~~~~~~ end of function ModifyRecord~~~~~~~~~~~~~~~~~

        //~~~~~ function DeleteRecord ~~~~~~~~~~~~~~~~~~
        // deletes a record form file
        // arguments: unsigned long
        // arg specifies the index of record to delete
        // returns true on success
        bool DeleteRecord(unsigned long index) {
            rewind(fp);
            FILE* tmp;
            char tmp_fnm[36];
            strcpy(tmp_fnm,"tmp.pln");
            //strcat(tmp_fnm,FileName);
            tmp = fopen(tmp_fnm,"wb");
            if(tmp == NULL) {
                return false;
            }
            unsigned long idx = 0;
            T t;
            while((fread((char*)&t,sizeof(T),1,fp)) == 1) {
                if(idx != index) {
                    fwrite((char*)&t,sizeof(T),1,tmp);
                }//else {
                //  printf("Index: %d\n",index);
                //}
                idx++;
            }
            fclose(fp);
            fclose(tmp);
            tmp = fopen(tmp_fnm,"rb");
            fp = fopen(FileName,"wb+");
            if(fp != NULL) {
                rewind(fp);
                rewind(tmp);
                while((fread((char*)&t,sizeof(T),1,tmp)) == 1) {
                    fwrite((char*)&t,sizeof(T),1,fp);
                }
                fclose(tmp);
                fclose(fp);
                fopen(FileName,"rb+");
                unlink(tmp_fnm);
                return true;
            }else {
                return false;
            }
        }
        //~~~~~~ end of function DeleteRecord~~~~~~~~~~~~~~~~~

        //~~~~~ function GetRecordCount ~~~~~~~~~~~~~~~~~~
        // counts number of record in file
        // arguments: null
        // 0
        // returns record count
        unsigned long GetRecordCount() const {
            fseek(fp,0,SEEK_END);
            unsigned long len = ftell(fp);
            unsigned long recs = len / sizeof(T);
            return recs;
        }
        //~~~~~~ end of function GetRecordCount~~~~~~~~~~~~~~~~~

        //~~~~~ function Close ~~~~~~~~~~~~~~~~~~
        // closes the file
        // arguments: 0
        // 0
        // returns void
        void Close() const {
            if(bIsOpen == true) {
                fclose(fp);
                bIsOpen = false;
            }
        }
        //~~~~~~ end of function Close~~~~~~~~~~~~~~~~~

        //~~~~~ function GetNextRecordNo ~~~~~~~~~~~~~~~~~~
        // finds the maximum id + 1 to uniquely identify a record
        // arguments: 0
        // 0
        // returns next id
        unsigned long GetNextRecordNo() {
            rewind(fp);
            unsigned long ret = 0;
            T t;
            while((fread((char*)&t,sizeof(T),1,fp)) == 1) {
                if(t.id > ret) {
                    ret = t.id;
                }
            }
            ret += 1;
            return ret;
        }
        //~~~~~~ end of function GetNextRecordNo~~~~~~~~~~~~~~~~~
        
        ~GenericDataStore() {
            if(bIsOpen == true) {
                Close();
            }
        }
};

#endif //RENTRAK_GENERIC_DATASTORE

