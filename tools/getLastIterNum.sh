ps -aux | grep a.out | cut -d _ -f 2 | cut -d ' ' -f 1 | cut -d . -f 2 | sort -nr | head -n1
