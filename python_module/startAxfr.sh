#!/bin/bash 

python /home/mkoi/mgr/myDig/python_module/axfrWrapper.py --output_folder /home/mkoi/mgr/myDig/python_module/test_preprod3

cp -r /home/mkoi/mgr/myDig/python_module/test_preprod3 /home/mkoi/remote

rm -rf /home/mkoi/mgr/myDig/python_module/test_preprod3



