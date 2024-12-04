build:
	gcc matrix_power.c safe_allocations.c -shared -o matrix_power.so -I /usr/include/python3.10 -fPIC
test:
	make build
	pytest -v