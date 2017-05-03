array=`find . -type f -size 25123c -exec ls {} \;`
for item in ${array[@]}; do
    mv $item ./unused/
done
