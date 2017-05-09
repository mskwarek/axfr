array=`find /home/marcin/output3 -type f`

for file in ${array[@]}; do
    echo $file | rev | cut -d '/' -f 1 | rev | cut -d _ -f 1 | rev | cut -d . -f 1 | rev >> /home/marcin/tlds.result
done
