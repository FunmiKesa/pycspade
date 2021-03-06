#include "calcdb.h"

CalcDb::CalcDb(const string& infile, int buf_sz)
{
   fd = open(infile.c_str(), O_RDONLY);
   if (fd < 0){
      throw runtime_error("ERROR: InvalidFile");
   }
   buf_size = buf_sz;
   buf = new int [buf_sz];
   cur_buf_pos = 0;
   cur_blk_size = 0;
   readall = 0;
   endpos = lseek(fd,0,SEEK_END);
}
   
CalcDb::~CalcDb()
{
   delete [] buf;
   close(fd);
}

void CalcDb::get_next_trans_ext()
{
   // Need to get more items from file
   int res = cur_blk_size - cur_buf_pos;
   if (res > 0)
   {
      // First copy partial transaction to beginning of buffer
      memcpy((void *)buf,
             (void *)(buf + cur_buf_pos),
             res * ITSZ);
      cur_blk_size = res;
   }
   else
   {
      // No partial transaction in buffer
      cur_blk_size = 0;
   }

   res = read(fd, (void *)(buf + cur_blk_size),
              ((buf_size - cur_blk_size)*ITSZ));
   
   if (res < 0){
      throw runtime_error("reading in database");
   }
   cur_blk_size += res/ITSZ;
   cur_buf_pos = 0;
}




