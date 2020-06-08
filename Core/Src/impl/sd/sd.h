/*
 * sd.h
 *
 *  Created on: May 4, 2020
 *      Author: bdouram
 */

#ifndef SRC_SD_SD_H_
#define SRC_SD_SD_H_

char* get_line_archive(void);

int connected_sd_card(void);
int open_archive(char *file, const int read_write);
int get_block_size(void);
int size_archive_to_read(void);
int write_archive(char* file);

void get_archive(void);
void close_archive(void);
void set_block(void);



#endif /* SRC_SD_SD_H_ */
