input=$1
output=$2

array=`find $input -type f -size 25123c -exec ls {} \;`
for item in ${array[@]}; do
    mv $item $output/
done
