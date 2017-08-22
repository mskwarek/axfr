result_dir=$1
file=$2


function processDir()
{
    file_to_write=$1
    processedDir=$2
    arr=`ls $processedDir`
 
    #echo $1 $2
    #echo "ARRAY:", ${arr[@]}
    for file in ${arr[@]}; do
        
        cat $processedDir/$file | grep  -P '\t1\t' | cut -d$'\t' -f 4 | grep -E -o '(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\.(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\.(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\.(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)' | uniq >> $file_to_write

    done 
}

fileArr=`ls $result_dir`

i=0
for dir in $fileArr; 
do
    #echo "DIR", $dir
    resul=${file}_${i}
    #echo $resul
    processDir $resul $result_dir/$dir &
    i=$[$i+1]
done
