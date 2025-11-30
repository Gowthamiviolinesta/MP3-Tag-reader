#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "mp3_tag_reader.h"
// View ID3v2 tags
void view_tags(mp3 *data)
{
    // Loop through 6 expected tags
    for (int i = 0; i < 6; i++)
    {
        char tag_id[5] = {0};
        if (fread(tag_id, 1, 4, data->fptr) != 4)// Read 4-byte frame ID
            break;

        unsigned char size_tag[4];
        if (fread(size_tag, 1, 4, data->fptr) != 4)// Read 4-byte frame size
            break;
        // Convert 4 bytes from the frame header into a single 32-bit integer (big-endian)
        int size = (size_tag[0] << 24) |
                   (size_tag[1] << 16) |
                   (size_tag[2] << 8)  |
                    size_tag[3];

        fseek(data->fptr, 2, SEEK_CUR); // Skip 2-byte flags

        // Allocate memory for text
        char *text = malloc(size + 1);
        if (fread(text, 1, size, data->fptr) != size)
            break;
        // Replace null bytes with space for display
        for(int j = 0; j < size; j++)
        {
            if(text[j] == '\0')
                text[j] = ' ';
        }
        // Store text based on tag
        if(strcmp(tag_id, "TPE1") == 0)
            data->artist = text;
        else if(strcmp(tag_id, "TIT2") == 0)
            data->song = text;
        else if(strcmp(tag_id, "TALB") == 0)
            data->album = text;
        else if(strcmp(tag_id, "TYER") == 0)
            data->year = text;
        else if(strcmp(tag_id, "TCON") == 0)
            data->content = text;
        else if(strcmp(tag_id, "COMM") == 0)
            data->comment = text;
        else
            free(text);
    }

    
}
int edit_frame(mp3 *file, const char *frame_id, const char *new_text)
{
    // Length of new text
    int new_len=strlen(new_text);
    // Skip ID3 header
    fseek(file->fptr, 10, SEEK_SET);

    char tag_id[5] = {0};
    unsigned char size_tag[4];
    // Read frame ID
    while (fread(tag_id, 1, 4, file->fptr) == 4)
    {
        long frame_header_offset = ftell(file->fptr) - 4;// Offset of frame header
        printf("Checking frame '%s' at offset %ld\n", tag_id, frame_header_offset);

        if (fread(size_tag, 1, 4, file->fptr) != 4)
            break;

        int size = (size_tag[0] << 24) |
                   (size_tag[1] << 16) |
                   (size_tag[2] << 8)  |
                    size_tag[3];
        // Skip flags
        fseek(file->fptr, 2, SEEK_CUR);

        long data_start = ftell(file->fptr);// Start of frame data

        if (tag_id[0] == 'T' || tag_id[0] == 'C')// Only process text/comment frames
        {
            if (strcmp(tag_id, frame_id) == 0)// Check if target frame
            {
                if(new_len>size)// Check if new text fits
                {
                    printf("Error: The new text is too long for frame '%s'. Maximum allowed is %d bytes.\n",frame_id, size - 1);
                    return 0;
                }
                printf("FOUND %s at offset %ld (data starts %ld, size %d)\n",
                       tag_id, frame_header_offset, data_start, size);

                int write_len = strlen(new_text);// Length to write
                if (write_len > size - 1)
                    write_len = size - 1;

                fseek(file->fptr, data_start + 1, SEEK_SET);// Skip encoding byte
                fwrite(new_text, 1, write_len, file->fptr);// Write new text

                for (int i = write_len; i < size - 1; i++)// Pad remaining bytes
                    fputc('\0', file->fptr);

                printf("UPDATED %s successfully!\n", tag_id);
                return 1;
            }
        }

        fseek(file->fptr, data_start + size, SEEK_SET);// Move to next frame
    }
    // Target frame not found
    printf("Frame %s NOT FOUND\n", frame_id);
    return 0;
}
