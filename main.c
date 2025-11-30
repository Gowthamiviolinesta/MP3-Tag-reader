#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include"mp3_tag_reader.h"

int main(int argc,char *argv[])
{
    mp3 mp3_data={0};
    if(argc<2)
    {
        printf("-----------------------------------------------------------------------\n");
        printf("Error: ./a.out : INVALID ARGUMENTS\n");
        printf("USAGE :\n");
        printf("To view please pass like: ./a.out -v mp3filename\n");
        printf("To edit please pass like: ./a.out -e -t\\-a\\-A\\-m\\-y\\-c changing_text mp3filename\n");
        printf("To get help pass like: ./a.out -h\n");
        printf("-----------------------------------------------------------------------\n");
        return 1;
    }
    if((strcmp(argv[1],"-v"))==0)
    {
        //open mp3 file
        mp3_data.fptr=fopen(argv[2],"rb");
        if(mp3_data.fptr==NULL)
        {
            printf("To view please pass like: ./a.out -v mp3filename\n");
            return 1;
        }
        //skip 10 bytes header
        fseek(mp3_data.fptr,10,SEEK_SET);

        view_tags(&mp3_data);
        // Print formatted output
        printf("-------------------------------------------------------------\n");
        printf("                  SELECTED VIEW DETAILS                      \n");
        printf("-------------------------------------------------------------\n\n");

        printf("             MP3 TAG READER AND EDITOR FOR ID3v2              \n\n");

        printf("-------------------------------------------------------------\n");

        printf("%-10s : %s\n", "TITLE", mp3_data.song ? mp3_data.song : "N/A");
        printf("%-10s : %s\n", "ARTIST", mp3_data.artist ? mp3_data.artist : "N/A");
        printf("%-10s : %s\n", "ALBUM", mp3_data.album ? mp3_data.album : "N/A");
        printf("%-10s : %s\n", "YEAR", mp3_data.year ? mp3_data.year : "N/A");
        printf("%-10s : %s\n", "MUSIC", mp3_data.content ? mp3_data.content : "N/A");
        printf("%-10s : %s\n", "COMMENT", mp3_data.comment ? mp3_data.comment : "N/A");

        printf("-------------------------------------------------------------\n\n");

        printf("               DETAILS DISPLAYED SUCCESSFULLY                \n");
        printf("-------------------------------------------------------------\n");

        free(mp3_data.song);
        free(mp3_data.artist);
        free(mp3_data.album);
        free(mp3_data.year);
        free(mp3_data.content);
        free(mp3_data.comment);


        fclose(mp3_data.fptr);
    }
    else if((strcmp(argv[1],"-h"))==0)
    {
        printf("\n-----------------HELP MENU-------------------\n");
        printf("1. -v -> to view mp3 file contents\n");
        printf("2. -e -> to edit mp3 file contents\n");
        printf("          2.1. -t -> to edit song title\n");
        printf("          2.2. -a -> to edit artist name\n");
        printf("          2.3. -A -> to edit album name\n");
        printf("          2.4. -y -> to edit year\n");
        printf("          2.5. -m -> to edit content\n");
        printf("          2.6. -c -> to edit comment\n");
        printf("--------------------------------------------------\n");
    }
    else if ((strcmp(argv[1], "-e")) == 0)
    {
        if (argc < 5)
        {
            printf("-----------------------------------------------------------------------\n");
            printf("Error: ./a.out : INVALID ARGUMENTS\n");
            printf("USAGE :\n");
            printf("./a.out -e -t\\-a\\-A\\-m\\-y\\-c changing_text mp3filename\n");
            printf("-----------------------------------------------------------------------\n");
            return 1;
        }

        // Open original MP3 file
        mp3_data.fptr = fopen(argv[4], "rb+");
        if (!mp3_data.fptr)
        {
            printf("Error: Original MP3 file '%s' not found.\n", argv[4]);
            return 1;
        }
        // Create default.mp3 as a copy
        /*mp3 *default_mp3 = malloc(sizeof(mp3));
        if (!default_mp3)
        {
            printf("Memory allocation failed\n");
            fclose(mp3_data.fptr);
            return 1;
        }

        default_mp3->fptr = fopen("default.mp3", "wb");
        if (!default_mp3->fptr)
        {
            printf("Error: Creating default mp3 file\n");
            fclose(mp3_data.fptr);
            free(default_mp3);
            return 1;
        }

        char ch;
        while (fread(&ch, 1, 1, mp3_data.fptr) == 1)
            fwrite(&ch, 1, 1, default_mp3->fptr);

        fclose(mp3_data.fptr);
        fclose(default_mp3->fptr);

        // Reopen default.mp3 in read+write mode
        default_mp3->fptr = fopen("default.mp3", "rb+");
        if (!default_mp3->fptr)
        {
            printf("Error: Re-opening default.mp3 for editing failed.\n");
            free(default_mp3);
            return 1;
        }*/

        printf("-------------SELECTED EDIT DETAILS-------------\n");

        char opt = argv[2][1]; // option character: t, a, A, m, y, c
        char *new_text = argv[3];
        int status = 0;

        switch (opt)
        {
            case 't': status = edit_frame(&mp3_data, "TIT2", new_text); break;
            case 'a': status = edit_frame(&mp3_data, "TPE1", new_text); break;
            case 'A': status = edit_frame(&mp3_data, "TALB", new_text); break;
            case 'y': status = edit_frame(&mp3_data, "TYER", new_text); break;
            case 'm': status = edit_frame(&mp3_data, "TCON", new_text); break;
            case 'c': status = edit_frame(&mp3_data, "COMM", new_text); break;
            default:
                printf("Invalid choice\nUSAGE: ./a.out -e -t\\-a\\-A\\-m\\-y\\-c changing_text mp3filename\n");
                break;
        }

        if (status)
            printf("Frame updated successfully sample.mp3\n");
        else
            printf("Frame not found or could not be updated\n");

        fclose(mp3_data.fptr);
    }

    else
    {
        printf("-----------------------------------------------------------------------\n");
        printf("Error: ./a.out : INVALID ARGUMENTS\n");
        printf("USAGE :\n");
        printf("To view please pass like: ./a.out -v mp3filename\n");
        printf("To edit please pass like: ./a.out -e -t\\-a\\-A\\-m\\-y\\-c changing_text mp3filename\n");
        printf("To get help pass like: ./a.out -h\n");
        printf("-----------------------------------------------------------------------\n");
        return 1;
    }
}