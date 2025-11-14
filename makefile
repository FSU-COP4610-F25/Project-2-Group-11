MAKEFLAGS += --no-print-directory

.PHONY: all part1 part2 part3 syscall_test elevator_test clean verify run run_part2 run_part3_start run_part3_requests run_part3_stop run_syscall_test

all: part1 part2 part3 syscall_test elevator_test

part1:
	$(MAKE) -C part_1

part2:
	$(MAKE) -C part_2

part3:
	$(MAKE) -C part_3

syscall_test:
	$(MAKE) -C part_3/tests/system-calls-test

elevator_test:
	$(MAKE) -C part_3/tests/elevator-test

run:
	@echo "--- Running all non-interactive parts (Part 1, Part 2, Syscall Test) ---"
	@echo
	@echo "*** PART 1: Verification ***"
	$(MAKE) verify
	@echo
	@echo "*** PART 2: Timer Module Test ***"
	$(MAKE) run_part2
	@echo
	@echo "*** PART 3: System Call Test ***"
	$(MAKE) run_syscall_test
	@echo
	@echo "--- Part 3: Elevator Simulation (Manual Steps Required) ---"
	@echo
	@echo "1. START ELEVATOR: Run 'sudo make run_part3_start'"
	@echo
	@echo "2. SEND REQUESTS: After the watch is running, run 'sudo make run_part3_requests'"
	@echo
	@echo "3. STOP ELEVATOR: After monitoring is complete, run 'sudo make run_part3_stop'"
	@echo

verify: part1
	$(MAKE) -C part_1 verify

run_part2: part2
	sudo insmod part_2/my_timer.ko || { echo "Failed to load my_timer.ko"; exit 1; }
	cat /proc/timer
	sleep 1
	cat /proc/timer
	-sudo rmmod my_timer 2>/dev/null || true

run_part3_start: part3 elevator_test
	sudo insmod part_3/elevator.ko || { echo "Failed to load elevator.ko"; exit 1; }
	part_3/tests/elevator-test/consumer --start &
	@echo "Run 'watch -n 0.5 cat /proc/elevator' in another terminal."

run_part3_requests: elevator_test
	part_3/tests/elevator-test/producer 20

run_part3_stop: elevator_test
	part_3/tests/elevator-test/consumer --stop
	-sudo rmmod elevator 2>/dev/null || true

run_syscall_test: syscall_test
	sudo insmod part_3/tests/system-calls-test/syscheck.ko || { echo "Failed to load syscheck.ko"; exit 1; }
	$(MAKE) -C part_3/tests/system-calls-test run
	-sudo rmmod syscheck 2>/dev/null || true

clean: 
	$(MAKE) -C part_1 clean
	$(MAKE) -C part_2 clean
	$(MAKE) -C part_3 clean
	$(MAKE) -C part_3/tests/system-calls-test clean
	$(MAKE) -C part_3/tests/elevator-test clean
