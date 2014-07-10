#!/usr/bin/bash

first_interval="[0, 5]"
first_interval_ok=("[0, 5]" "[0, 3]" "[2, 5]" "[0, 0]" "[5, 5]" "[1, 3]")
first_interval_absent=("[-5, -2]" "[-3, 0]" "[-2, 1]" "[-4, 5]" "[-1, 6]" "[0, 8]" "[3, 7]" "[5, 10]" "[8, 17]")

second_interval="[-8,-15]"
second_interval_normalize="[-15, -8]"
second_interval_ok=("[-15, -8]" "[-10, -15]" "[-12, -8]" "[-15, -15]" "[-8, -8]" "[-13, -9]")
second_interval_absent=("[-25, -18]" "[-30, -15]" "[-21, -10]" "[-24, -8]" "[-19, -6]" "[-15, -5]" "[-11, -2]" "[-8, -7]" "[-7, -1]")

group_intervals=("[-1, 3]" "[0, 6]" "[3, 8]" "[-2, 1]" "[-17, -5]" "[-10, -4]")
#ordered_list="[-17, -5][-15, -8][-10, -4][-2, 1][-1, 3][0, 6][0, 5][3, 8]"
#ordered_list_structure="[-17, -5]->[-17, -5] | [-15, -8]->[-17, -5] | [-10, -4]->[-10, -4] | [-2, 1]->[-2, 1] | [-1, 3]->[-1, 3] | [0, 6]->[0, 6] | [0, 5]->[0, 6] | [3, 8]->[3, 8]"

intervals_one=("[-1, 0]" "[-1,2]")
intervals_two=("[2,5]" "[0,5]" "[1,6]")

intervals_three=("[3,4]" "[4,5]" "[4,6]" "[5,7]" "[8,8]")
intervals_four=("[-2,1]" "[-2,0]")
intervals_five=("[-13,-6]" "[-16,-10]" "[-15,-8]" "[-17,-6]")
intervals_six=("[-9,-9]" "[-10,-9]" "[-9,-6]" "[-9,-5]" "[-5,-4]")

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
	[[ $res == "ADD" ]] || crush
}

function RemoveInterval {
	local res=$(curl --silent -X REMOVE -d "$1" localhost:8080)
	[[ $? ]]|| crush
	[[ $res == "REMOVE" ]] || crush 
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
	
	for i in "${second_interval_ok[@]}"
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

function SecondIntervalTest {
	for i in "${second_interval_ok[@]}"
	do
		SearchInterval "$i"
		[[ "$result" == "$second_interval_normalize" ]]|| crush 
	done
	
	for i in "${second_interval_absent[@]}"
	do
		SearchInterval "$i"
		[[ $result == "[]" ]]|| crush 
	done
}

function IntervalsTest {
	for i in "${intervals_one[@]}"
	do
		SearchInterval "$i"
		[[ "$result" == "${group_intervals[0]}" ]]|| crush 
	done

	for i in "${intervals_two[@]}"
	do
		SearchInterval "$i"
		[[ "$result" == "${group_intervals[1]}" ]]|| crush 
	done
	
	for i in "${intervals_three[@]}"
	do
		SearchInterval "$i"
		[[ "$result" == "${group_intervals[2]}" ]]|| crush 
	done
	
	for i in "${intervals_four[@]}"
	do
		SearchInterval "$i"
		[[ "$result" == "${group_intervals[3]}" ]]|| crush 
	done
	
	for i in "${intervals_five[@]}"
	do
		SearchInterval "$i"
		[[ "$result" == "${group_intervals[4]}" ]]|| crush 
	done
	
	for i in "${intervals_six[@]}"
	do
		SearchInterval "$i"
		[[ "$result" == "${group_intervals[5]}" ]]|| crush 
	done
	
}


echo 
echo   "==============================================================="
echo   "                Test for HttpIntervalStorage"
echo   "==============================================================="
printf " SERVER START .........................................."

./HttpIntervalStorage &>/dev/null &
pid_server=$!
sleep 1
[[ $pid_server -ne 0 ]]&& echo $OK || crush
	
echo   "==============================================================="
printf " Empty storage ........................................."
#GetList
EmptySrorageTest
echo $OK

printf " Add first interval ...................................."
AddInterval "$first_interval"
echo $OK

printf " Single interval test .................................."
FirstIntervalTest
echo $OK
#
printf " Add second interval ..................................."
AddInterval "$second_interval"
echo $OK

printf " Add other intervals ..................................."
for i in "${group_intervals[@]}"
do
	AddInterval "$i"
done
echo $OK

printf " Multy interval order .................................."
echo $NOT_IMP
#
printf " Multy interval test ..................................."
IntervalsTest
echo $OK
#
printf " Remove intervals ......................................"
RemoveInterval "$first_interval"
for i in "${group_intervals[@]}"
do
	RemoveInterval "$i"
done
echo $OK
#
printf " Second single interval test ..........................."
SecondIntervalTest
echo $OK
#
printf " Remove last interval .................................."
RemoveInterval "$second_interval"
echo $OK
#
printf " Empty storage ........................................."
#GetList
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

