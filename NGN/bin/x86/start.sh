#/bin/sh

#test()
#{
	rm /usr/local/ngn -r
	rm /usr/local/ngn-local/ -r
	killall -9 broker
	killall -9 om_worker
	killall -9 sys_worker
	DEBUG=1 ./broker/broker                    
	DEBUG=1 ./om_worker -s 1
	DEBUG=1 ./sys_worker -w sys_worker -s 6
	DEBUG=1 ./sys_worker -w if_worker -s 5
#	DEBUG=1 ./debug_cli -s 1 -o 22 -n $1
#}

#i=0
#while [ $i -lt $2 ]
#do
#	test $1
#	((i++))
#done

#exit 0
