.PHONY: all part1 part2 part3 tests clean clean_part1 clean_part2 clean_part3 clean_tests install remove verify run run_part2 run_part3_start run_part3_requests run_part3_stop run_syscall_test

all: part1 part2 part3 tests

part1:
	make -C part_1/src

part2:
	make -C part_2/src

part3:
	make -C part_3/src

tests:
	make -C part_3/tests/system-calls-test

run:
	@echo "--- Running all non-interactive parts (Part 1, Part 2, Syscall Test) ---"
	@echo "*** PART 1: Verification ***"
	$(MAKE) verify
	@echo "*** PART 2: Timer Module Test ***"
	$(MAKE) run_part2
	@echo "*** PART 3: System Call Test ***"
	$(MAKE) run_syscall_test
	@echo " "
	@echo "--- Part 3: Elevator Simulation (Manual Steps Required) ---"
	@echo "1. START ELEVATOR: Run 'make run_part3_start'"
	@echo "   (This loads the module and starts the consumer. You must then open a new terminal to run 'watch -n 0.5 cat /proc/elevator')"
	@echo " "
	@echo "2. SEND REQUESTS: After the watch is running, run 'make run_part3_requests'"
	@echo " "
	@echo "3. STOP ELEVATOR: After monitoring is complete, run 'make run_part3_stop'"

verify: part1
	make -C part_1/src verify

run_part2: part2
	sudo insmod part_2/src/my_timer.ko
	cat /proc/timer
	sleep 1
	cat /proc/timer
	-sudo rmmod my_timer 2>/dev/null || true

run_part3_start: part3
	sudo insmod part_3/src/elevator.ko
	part_3/src/consumer --start &
	@echo "Run 'watch -n 0.5 cat /proc/elevator' in another terminal."

run_part3_requests:
	part_3/src/producer 20

run_part3_stop:
	part_3/src/consumer --stop
	-sudo rmmod elevator 2>/dev/null || true

run_syscall_test: tests
	sudo insmod part_3/tests/system-calls-test/syscheck.ko
	make -C part_3/tests/system-calls-test run
	-sudo rmmod syscheck 2>/dev/null || true

clean_part1:
	make -C part_1/src clean

clean_part2:
	make -C part_2/src clean

clean_part3:
	make -C part_3/src clean

clean_tests:
	make -C part_3/tests/system-calls-test clean

clean: clean_part1 clean_part2 clean_part3 clean_tests

install: part2 part3 tests
	sudo insmod part_2/src/my_timer.ko || true
	sudo insmod part_3/src/elevator.ko || true
	sudo insmod part_3/tests/system-calls-test/syscheck.ko || true

remove:
	-sudo rmmod elevator 2>/dev/null || true
	-sudo rmmod my_timer 2>/dev/null || true
	-sudo rmmod syscheck 2>/dev/null || true
