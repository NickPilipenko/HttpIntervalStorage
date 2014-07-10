#!/usr/bin/bash

first_interval="[0, 5]"
first_interval_ok=("[0, 5]" "[0, 3]" "[2, 5]" "[0, 0]" "[5, 5]" "[1, 3]")
first_interval_absent=("[-5, -2]" "[-3, 0]" "[-2, 1]" "[-4, 5]" "[-1, 6]" "[0, 8]" "[3, 7]" "[5, 10]" "[8, 17]")



OK=".... OK"
ERROR=". ERROR"
NOT_IMP="NOT IMP"

set -e
function cleanup {
	if [[ $pid_server != "" ]];
	then
		kill -SIGINT $pid_server
	fi
}
trap cleanup EXIT

function crush {
	echo $ERROR
	echo FAILED
	exit 1
}

function AddInterval {
	local res=$(curl --silent -X ADD -d "$1" localhost:8080)
	[[ $? ]]|| crush
	[[ $res == "ADD" ]] 
}

function RemoveInterval {
	local res=$(curl --silent -X REMOVE -d "$1" localhost:8080)
	[[ $? ]]|| crush
	[[ $res == "REMOVE" ]] 
}

function SearchInterval {
	result=$(curl --silent -X SEARCH -d "$1" localhost:8080)
	[[ $? ]]|| crush
}

function EmptySrorageTest {
	for i in "${first_interval_ok[@]}"
	do
		SearchInterval "$i"
		[[ $result == "[]" ]]|| crush 
	done
}

function FirstIntervalTest {
	for i in "${first_interval_ok[@]}"
	do
		SearchInterval "$i"
		[[ "$result" == "$first_interval" ]]|| crush 
	done
	
	for i in "${first_interval_absent[@]}"
	do
		SearchInterval "$i"
		[[ $result == "[]" ]]|| crush 
	done
}

echo 
echo   "==============================================================="
echo   "                Test for HttpIntervalStorage"
echo   "==============================================================="
printf " SERVER START .........................................."

./HttpIntervalStorage &>/dev/null &
pid_server=$!
sleep 2
[[ $pid_server -ne 0 ]]&& echo $OK || crush
	
echo   "==============================================================="
printf " Empty storage ........................................."
EmptySrorageTest
echo $OK
#
printf " Single interval ......................................."
AddInterval "$first_interval"
[ $? ]||crush
FirstIntervalTest
echo $OK
#
printf " Multy interval order .................................."
echo $NOT_IMP
#
printf " Multy interval ........................................"
echo $NOT_IMP
#
printf " Remove intervals ......................................"
RemoveInterval "$first_interval"
[ $? ]||crush
echo $OK
#
printf " Second single interval ................................"
echo $NOT_IMP
#
printf " Remove last interval .................................."
echo $NOT_IMP
#
printf " Empty storage ........................................."
EmptySrorageTest
echo $OK
#

echo   "==============================================================="
printf " SERVER STOP ..........................................."
kill -SIGINT $pid_server
[[ $? -eq 0 ]]&& echo $OK || crush
pid_server=""
echo   "==============================================================="
echo   "SUCCESS"

