# MP3-Tag-reader
View, edit, and manage MP3 metadata including title, artist, album, year,music, comments.
git clone https://github.com/username/MP3TagEditor.git
gcc *.c
./a.out -v sample.mp3//for view
./a.out -h //for help
./a.out -e -y 2025 sample.mp3//edit YEAR
./a.out -e -t "sonny sonny" sample.mp3 //to edit TITLE
./a.out -e -a "mayara" sample.mp3 //to edit ARTIST
./a.out -e -A "BOLLY WOOD" sample.mp3 //to edit ALBUM
./a.out -e -m "pani pani" sample.mp3 //to edit MUSIC
./a.out -e -c "english language" sample.mp3 //to edit COMMENT

