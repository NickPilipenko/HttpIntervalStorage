#!/usr/bin/bash

first_interval="[0,5]"
first_interval_ok=("[0,5]" "[0,3]" "[2,5]" "[0,0]" "[5,5]" "[1,3]")
first_interval_absent=("[-5,-2]" "[-3,0]" "[-2,1]" "[-4,5]" "[-1,6]" "[0,8]" "[3,7]" "[5,10]" "[8,17]")

OK="... OK"
ERROR=" ERROR"

function crush {
	echo $ERROR
	echo FAILED
	exit 1
}

function AddInterval {
	local res=$(curl -X ADD -d $1 localhost:8080)
	[[ $? -ne 0 ]]&& crush
	[[ $res == "ADD" ]] 
}

function RemoveInterval {
	local res=$(curl -X REMOVE -d $1 localhost:8080)
	[[ $? -ne 0 ]]&& crush
	[[ $res == "REMOVE" ]] 
}

function SearchInterval {
	result =$(curl -X SEARCH -d $1 localhost:8080)
	[[ $? -ne 0 ]]&& crush
}

function EmptySrorageTest {
	for i in first_interval_ok;
	do
		SearchInterval i
		[[ $result != "[]" ]]&& crush
	done;	
}

echo 
echo   "==============================================================="
echo   "                Test for HttpIntervalStorage"
echo   "==============================================================="
printf " SERVER START ..........................................."

./HttpIntervalStorage &>/dev/null &
pid_server=$!
sleep 2
[[ $pid_server -ne 0 ]]&& echo $OK || crush
	
echo   "==============================================================="
printf " Empty storage .........................................."
EmptySrorageTest
echo $OK
#
printf " Single interval ........................................"
echo $OK
#
printf " Multy interval order ..................................."
echo $OK
#
printf " Multy interval ........................................."
echo $OK
#
printf " Second single interval ................................."
echo $OK
#
printf " Empty storage .........................................."
echo $OK
#

echo   "==============================================================="
printf " SERVER STOP ............................................"
kill -SIGINT $pid_server
[[ $? -eq 0 ]]&& echo $OK || crush
echo   "==============================================================="
echo   "SUCCESS"

