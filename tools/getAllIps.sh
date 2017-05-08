result_dir=$1
file_to_write=$2

fileArr=`find $result_dir -type f`

for file in ${fileArr[@]}; do

    cat $file | grep  -P '\t1\t' | cut -d$'\t' -f 4 | grep -E -o '(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\.(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\.(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\.(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)' | uniq >> $file_to_write

done
