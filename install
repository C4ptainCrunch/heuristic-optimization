sudo dnf install gcc make git

run=$(date "+%d-%m-%y-%Hh")

echo "Starting run $run" > $run.log

touch result.zip

tmpfile=$( mktemp -t transferXXXXX );
curl --upload-file "result.zip" "http://partou.se/upload/jemhW/run" >> $tmpfile;
cat $tmpfile >> $run.log
rm -f $tmpfile;

echo "End of run run $run" >> $run.log
