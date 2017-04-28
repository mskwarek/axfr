arr=`find /home/marcin/output3 -type f -printf "%s %p\n" | sort -nr | cut -d\  -f2-`

for file in ${arr[@]}; do 
    FILESIZE=$(stat -c%s "$file")
    echo "$file $FILESIZE"
done;
