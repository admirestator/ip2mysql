#! /bin/bash
#Filename:	conviplib.sh
#Author:	admirestator <admirestator@gmail.com>
#Comment:	a script to convert charectorset of cz ip 
#			data libbrary form gbk to utf8.


echo "start convert..."
iconv -c -f gbk -t utf8 czip.txt -o czutf8
echo "convertion success! Watch file \"czutf8\""

