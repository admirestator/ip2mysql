#! /bin/bash
#Filename:	conviplib.sh
#Author:	admirestator <admirestator@gmail.com>
#Comment:	a script to convert charectorset of cz ip 
#			data libbrary form gbk to utf8.


#convert character sets and delete the quote
echo "start convert..."
iconv -c -f gbk -t utf8 czip.txt -o czutf8
sed "s/ CZ88.NET/FB/g" czutf8 > tmp
sed 's/"//g'  tmp > ipdbtmp
sed "s/'//g"  tmp > ipdb
rm -rf tmp rm czutf8 ipdbtmp
echo "convertion success! Watch file \"ipdb\""


#select the last 10 items;
#SELECT * FROM  qqwry ORDER BY id  DESC LIMIT 10;
