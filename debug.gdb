define dump_calls
	set $start=__rnk_initcalls_start
	while ($start < __rnk_initcalls_end)
		x/1xw $start
		set $start=$start+4
	end
end

define dump_mpu
	set $i = 0
	set $nregion = 8

	while($i < $nregion)
		set *0xE000ED98 = $i
		print "---------- "
		x/x 0xE000ED9C
		x/x 0xE000EDA0
		print "-----------"
		set $i = $i + 1

	end
end

define enable_swo
	monitor SWO EnableTarget 0 64000 1 0
end

define enable_semihosting
	monitor semihosting enable
end

define load_test
	if $argc == 0
		print "specify apps path"
	end

	if $argc == 1
		load tests/$arg0_tests/$arg0_test
		file tests/$arg0_tests/$arg0_test
	end
end

tar rem 127.0.0.1:2331
