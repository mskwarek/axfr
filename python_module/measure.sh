#!/bin/bash
0;136;0c
for x in {1..5}
do
    python axfrWrapper.py --output_folder tests/test_$x
done
