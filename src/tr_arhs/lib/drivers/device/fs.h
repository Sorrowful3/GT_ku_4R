/*
 * fs.h
 *
 *  Created on: 2021Äê4ÔÂ7ÈÕ
 *      Author: admin
 */

#ifndef SRC_LIB_FS_FS_H_
#define SRC_LIB_FS_FS_H_

struct file
{
  int               f_oflags;   /* Open mode flags */
  int32_t           f_pos;      /* File position */
  void             *f_priv;     /* Per file driver private data */
};



#endif /* SRC_LIB_FS_FS_H_ */
