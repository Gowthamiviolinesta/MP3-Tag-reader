#ifndef MP3_TAG_READER_H
#define MP3_TAG_READER_H
#include<stdio.h>


typedef struct MP3_tag_reader
{
    FILE* fptr;
    FILE* fdef;
    char* file_name;
    char *song;
    char *artist;
    char *album;
    char *year;
    char *content;
    char *comment;
}mp3;

void view_tags(mp3 *data);
int edit_frame(mp3 *data, const char *frame_id, const char *new_text);
#endif