#!/bin/bash
pipe=/tmp/btctlpipe 
output_file=/tmp/btctl_output

if [[ ! -p $pipe ]]; then
  mkfifo $pipe
fi

trap terminate INT
function terminate()
{
  killall bluetoothctl &>/dev/null
  rm -f $pipe
}

function bleutoothctl_reader() 
{
  {
    while true
    do
      if read line <$pipe; then
          if [[ "$line" == 'exit' ]]; then
              break
          fi          
          echo $line
      fi
    done
  } | bluetoothctl > "$output_file"
}


function bleutoothctl_writer() 
{
  cmd=$1
  printf "$cmd\n\n" > $pipe
}

bleutoothctl_reader &
sleep 1
bleutoothctl_writer "scan on"
sleep 15
bleutoothctl_writer "scan off"
sleep 1
bleutoothctl_writer "devices"
sleep 1