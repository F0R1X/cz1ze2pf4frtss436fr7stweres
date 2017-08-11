#!/bin/bash

kill -9 `ps -ef | grep 'chenzhouphz -f'|grep -v 'grep' |awk 'NR==1{print $2}'`
kill -9 `ps -ef | grep 'chenzhou3phz -f'|grep -v 'grep' |awk 'NR==1{print $2}'`

kill -9 `ps -ef | grep 'jiahephz -f'|grep -v 'grep' |awk 'NR==1{print $2}'`
kill -9 `ps -ef | grep 'linwuphz -f'|grep -v 'grep' |awk 'NR==1{print $2}'`
kill -9 `ps -ef | grep 'matianphz -f'|grep -v 'grep' |awk 'NR==1{print $2}'`
kill -9 `ps -ef | grep 'yizhangphz -f'|grep -v 'grep' |awk 'NR==1{print $2}'`
kill -9 `ps -ef | grep 'yongxingmhz -f'|grep -v 'grep' |awk 'NR==1{print $2}'`
kill -9 `ps -ef | grep 'guiyangzpphz -f'|grep -v 'grep' |awk 'NR==1{print $2}'`
