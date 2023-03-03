#!/usr/bin/expect -f

set srcfile [lindex $argv 0]
set timeout 360
spawn scp -P 30022  ${srcfile}  root@119.29.194.32:/data/home/user00/ftp_dir
expect {
"*yes/no*" { send "yes\r";exp_continue }
"password*" {send "D3RL5NLbU9DM2Pq\r" }
}
expect eof
